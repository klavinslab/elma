#include <stdexcept>
#include <iostream>
#include <algorithm>
#include "elma.h"

namespace elma {

    //! Add a Process to the manager, to be run at a certain frequency.
    //! \param process The process to be scheduled, usually derived from the Process abstract base class
    //! \param period The desired duration of time between updates
    //! \return A reference to the manager, for chaining
    Manager& Manager::schedule(
        Process& process, 
        high_resolution_clock::duration period) {

        process._period = period;
        _processes.push_back(&process); 
        process._manager_ptr = this;            

        if (Priority_min > process._priority || process._priority > Priority_max ){
            throw Exception("Priority must be between -5(low priority) and 15(high priority)");
        }    

        return *this;

    }

    //! Add a channel to the manager
    //! \param The channel to be added
    //! \return A reference to the manager, for chaining
    Manager& Manager::add_channel(Channel& channel) {
        _channels[channel.name()] = &channel;
        return *this;
    }

    //! Retrieve a reference to an existing channel. Throws an error if no such channel exists.
    //! \return The channel requested.
    Channel& Manager::channel(string name) {
        if ( _channels.find(name) != _channels.end() ) {
          return *(_channels[name]);
        } else {
            throw Exception("Tried to access an unregistered or non-existant channel.");
        }
    }    

    //! Watch for an event associated with the given name.
    //! For watching events, you would typically register event handlers in your process'
    //! init() method. For example,
    //! @code
    //!     watch("velocity", [this](Event& e) {
    //!         std::cout << "got velocity " << e.value() << std::endl;
    //!     })
    //! @endcode
    //! \param event_name The name of the event
    //! \handler A function or lambda that takes an event and returns nothing.
    Manager& Manager::watch(std::string event_name, std::function<void(Event&)> handler) {
        event_handlers[event_name].push_back(handler);
        return *this;
    }

    //! Emit an event associated with a name.
    //! Typically, a process would emit events in its update() method using something like
    //! the following code"
    //! @code
    //!     emit(Event("name", value));
    //! @endcode
    //! where value is any jsonable value. For example, you can write
    //! @code
    //!     emit(Event("velocity", 3.41));
    //! @endcode
    //! \param event The Event to be emitted
    //! \return A reference to the manager for chaining.
    Manager& Manager::emit(const Event& event) {
        Event e = event; // make a copy so we can change propagation
        if ( event_handlers.find(event.name()) != event_handlers.end() ) {
            for ( auto handler : event_handlers[event.name()] ) {
                if ( e.propagate() ) {
                  handler(e);
                }
            }
        }
        return *this;
    }

    //! Apply a function to all processes.
    //! \param f The function to apply. It should take a reference to a process and return void.
    //! \return A reference to the manager, for chaining
    Manager& Manager::all(std::function< void(Process&) > f) {
        for(auto process_ptr : _processes) {
            f(*process_ptr);
        }
        return *this;
    }

    //! Initialize all processes. Usually called before run()
    //! \return A reference to the manager, for chaining
    Manager& Manager::init() {
        sort_processes();  
        return all([](Process& p) { p._init();});
    }

    //! Start all processes. Usually not called directly.
    //! \return A reference to the manager, for chaining
    Manager& Manager::start() {
        _running = true;
        return all([this](Process& p) { p._start(_elapsed) ;});
    }    

    //! Stop all processes. Usually not called directly.
    //! \return A reference to the manager, for chaining
    Manager& Manager::stop() {
        _running  = false;
        return all([](Process& p) { p._stop(); });
    }    

    //! Update all processes if enough time has passed. Usually not called directly.
    //! \return A reference to the manager, for chaining
    Manager& Manager::update() {
        _client.process_responses();
        return all([this](Process& p) {
            if ( _elapsed >= p.last_update() + p.period() ) {
                p._update(_elapsed);
            }
        });
    }

    //! sort _Processes based on _priority to ensure higher priority process are updated first.
    //! \return A reference to the manager, for chaining
    Manager& Manager::sort_processes() {

        std::sort(_processes.begin(), _processes.end(),[](const Process * lhs, const Process * rhs){
            return lhs->_priority > rhs->_priority;
        });  

        return *this;
    }

    //! Sets if the manager will run in simulated time.
    //! In simulated time, the next schefuled process will immediately run at the completion of the last.
    //! \return A reference to the manager, for chaining
    Manager& Manager::use_simulated_time() { 
        _simulated_time = true; 
        return *this;
    };

    //! Sets if the manager will run in real time.
    //! In real time, the manager will continually advance time until the next process needs to update.
    //! \param If the manager runs in simulated time.
    //! \return A reference to the manager, for chaining
    Manager& Manager::use_real_time() { 
        _simulated_time = false; 
        return *this;
    };
    
    //! Set Process Priority and sort _Processes to ensure higher priority are updated first. 
    //! Priority may be set -5 (low priority) to 15 (high priority)
    //! This should allow priority adjustment while running.
    //! \param process The process you want to adjust priority level.
    //! \param priority, a integer between -5 and 15 
    //! \return A reference to the manager, for chaining
    Manager& Manager::set_priority(Process& process, int priority) {

        if (Priority_min <= priority && priority <= Priority_max  ){
            process._priority = priority;
            sort_processes();
        } else {
            throw Exception("Priority must be between -5(low priority) and 15(high priority)");
        }    
        return *this;
    }

    //! Run the manager for the specified amount of time.
    //! \param The desired amount of time to run
    //! \return A reference to the manager, for chaining
    Manager& Manager::run(high_resolution_clock::duration runtime) {

        return run([&]() { return _elapsed < runtime; });

    }

    //! Run the manager indefinitely or until a process calls halt().
    //! \return A reference to the manager, for chaining
    Manager& Manager::run() {

        return run([&]() { return _running; });

    }  

    //! Run the manager until the provided contion is true or until a process calls halt().
    //! \param The condition, a function returning a boolean and taking no arguments.
    //! \return A reference to the manager, for chaining
    Manager& Manager::run(std::function<bool()> condition)  {

        _start_time = high_resolution_clock::now();
        _elapsed = high_resolution_clock::duration::zero();
        start();        

        while ( condition() ) {
            update();
            update_elapsed_time();
        }

        stop();

        return *this;

    }

    //! Updates the elapsed time of the manager.
    //! In normal mode, elapsed is the time since starting the manager.
    //! In simulated mode, elapsed is set to the time of the next scheduled event.
    //! If no processes are scheduled, will always runs in normal mode.
    void Manager::update_elapsed_time() {

        // Only run in simulated time if we have the flag set and have processes.
        if(_simulated_time && !_processes.empty()) {

            // Find the process that has the smallest time till next update.
            auto min_iter = std::min_element(_processes.begin(), _processes.end() ,[](Process * lhs, Process * rhs) {
                auto lhsTime = lhs->last_update() + lhs->period();
                auto rhsTime = rhs->last_update() + rhs->period();
                return lhsTime < rhsTime;
            });

            Process* nextup = *min_iter;

            auto newTime = nextup->last_update() + nextup->period();

            _elapsed = newTime;

        } else {

            _elapsed = high_resolution_clock::now() - _start_time;

        }
    }

}