#include <iostream>
#include <chrono>
#include "elma.h"

//! \file

using namespace std::chrono;
using namespace elma;

namespace basic_example {

    //! Example: A very basic process class. See the file examples/basic.cc for usage.

    //! This process does nothing except print out its name and
    //! the amount of time it has been running since it was started.
    class BasicProcess : public Process {
        public:

        //! The constructor just wraps the base constructor.
        //! \param name The name of the process
        BasicProcess(std::string name) : Process(name) {}

        //! Nothing to do to initialize
        void init() {}

        //! Nothing to do to start
        void start() {}

        //! The update method just prints out some useful information
        void update() { 
            std::cout << name() << ": " 
                    << milli_time()
                    << "ms\n";
        }

        //! Nothing to do to stop
        void stop() {}
    };

}

int main() {

    Manager m; // Make a manager
    basic_example:: BasicProcess p("A"), q("B"); // Make two processes

    m.schedule(p, 1_ms) // Schedule the first to run every millisecond
     .schedule(q, 5_ms) // Schedule the second to run every 5 milliseconds
     .init()            // Initialize the manager (which calls init for the processes)
     .run(11_ms);       // Run for 11 milliseconds
     
}