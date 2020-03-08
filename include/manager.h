#ifndef _ELMA_MANAGER_H
#define _ELMA_MANAGER_H

#include <vector>
#include <map>
#include <chrono>
#include <functional>
#include <mutex>

#include "elma.h"

namespace elma {

    using std::string;
    using std::vector;
    using std::map;
    using namespace std::chrono;

    class Channel;
    class Process;

    //! The Process Manager class. 

    //! Example usage:
    //! \include examples/basic.cc

    class Manager {

        public: 

        //! Default constructor
        Manager() : _running(false), _simulated_time(false), _nice_sleep_amount(1_us) {}
        
        Manager& schedule(Process& process, high_resolution_clock::duration period);
        Manager& remove(Process& process);
        Manager& all(std::function<void(Process&)> f);

        Manager& set_priority(Process& process, int priority);
        Manager& sort_processes();

        Manager& init();
        Manager& start();
        Manager& update();
        Manager& stop();

        Manager& use_simulated_time();
        Manager& use_real_time();

        Manager& run(high_resolution_clock::duration runtime);
        Manager& run();
        Manager& run(std::function<bool()> condition);

        //! Getter
        //! \return The time the Manager was most recently started
        inline high_resolution_clock::time_point start_time() { return _start_time; }

        //! Getter
        //! \return The duration of time since the manager was most recently started
        inline high_resolution_clock::duration elapsed() { return _elapsed; }

        //! Getter
        //! \return The mutex used for the update method. Useful if some other
        //! non-elma process needs to act on shared data.
        inline std::mutex& get_update_mutex() { return _update_mutex; }

        //! Setter
        //! \return A reference to the manager, for chaining
        inline Manager& set_niceness(high_resolution_clock::duration n) {
            _nice_sleep_amount = n;
            return *this;
        }

        // Channel Interface
        Manager& add_channel(Channel&);
        Channel& channel(string);

        // Event Interface
        Manager& watch(string event_name, std::function<void(Event&)> handler);
        Manager& emit(const Event& event);
        Client& client() { return _client; }

        private:

        void update_elapsed_time();

        const int Priority_min = -5, Priority_max = 15;
        vector<Process *> _processes;
        map<string, Channel *> _channels;
        map<string, vector<std::function<void(Event&)>>> event_handlers;
        high_resolution_clock::time_point _start_time;
        high_resolution_clock::duration _elapsed;
        Client _client;
        bool _running;
        bool _simulated_time;

        std::mutex _update_mutex;
        high_resolution_clock::duration _nice_sleep_amount;

    };

}

#endif