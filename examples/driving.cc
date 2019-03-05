#include <iostream>
#include <chrono>
#include "elma.h"

//! \file

using namespace std::chrono;
using std::vector;
using namespace elma;

namespace driving_example {

    //! Example: Another car simulation process. See examples/driving.cc.

    //! See the file examples/driving.cc for usage.
    class Car : public Process {
        public:

        //! Wrap the base process class
        //! \param name The name of the car    
        Car(std::string name) : Process(name) {}

        //! Nothing to do to initialize
        void init() {}

        //! To start a new simulation, this process sets
        //! the car's velocity to zero kph.    
        void start() {
            velocity = 0;
        }

        //! The update method gets the latest force from the 
        //! Throttle Channel, if any. Then it updates the 
        //! car's velocity, and sends it out on the Velocity
        //! Channel.     
        void update() {
            if ( channel("Throttle").nonempty() ) {
                force = channel("Throttle").latest();
            }
            velocity += ( delta() / 1000 ) * ( - k * velocity + force ) / m;
            channel("Velocity").send(velocity);
            std::cout << milli_time() << ","
                    << velocity << " \n";
        }

        //! Nothing to do to stop    
        void stop() {}

        private:
        double velocity;
        double force;
        const double k = 0.02;
        const double m = 1000;
    };  

    //! Example: A cruise controller for a Car process.  See examples/driving.cc.

    //! See the file examples/driving.cc for usage.
    class CruiseControl : public Process {

        public:

        //! Wrap the base process class
        //! \param name The name of the controller      
        CruiseControl(std::string name) : Process(name) {}

        //! Watch for events that change the desired speed.
        void init() {
            watch("desired speed", [this](Event& e) {
                desired_speed = e.value();
            });
        }

        //! Nothing to do to initialize    
        void start() {}

        //! Get the velocity from the Velocity Channel, compute
        //! a simple proportional control law, and send the result
        //! to the Throttle channel.    
        void update() {
            if ( channel("Velocity").nonempty() ) {
                speed = channel("Velocity").latest();
            }
            channel("Throttle").send(-KP*(speed - desired_speed));
        }

        //! Nothing to do to stop    
        void stop() {}

        private:
        double speed = 0;
        double desired_speed = 0.0;
        const double KP = 314.15;
                    vector<double> _v;
    };

    //! Example: A simulated driver, who keeps cycling between 50 and 60 kph.  See examples/driving.cc.
    class Driver : public Process {

        public: 

        //! Wrap the base process class
        //! \param name The name of the controller       
        Driver(std::string name) : Process(name) {}

        //! initialize the desired speed
        void init() {
            desired_speed = 50;
        }

        //! Nothing to do to start
        void start() {}

        //! If the desired speed is 50, change to 60,
        //! otherwise change to 50.
        void update() {
            if ( desired_speed == 50 ) {
                desired_speed = 60;
            } else {
                desired_speed = 50;
            }
            emit(Event("desired speed", desired_speed));
        }

        //! Nothing to do to stop
        void stop() {}

        private:
        double desired_speed;

    };

}

int main() {

    Manager m;

    driving_example::Car car("Car");
    driving_example::CruiseControl cc("Control");
    driving_example::Driver driver("Steve");
    Channel throttle("Throttle");
    Channel velocity("Velocity");

    m.schedule(car, 100_ms)
    .schedule(cc, 100_ms)
    .schedule(driver, 5_s)
    .add_channel(throttle)
    .add_channel(velocity)
    .init()
    .run(40_s);

}