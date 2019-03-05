#include <iostream>
#include <chrono>
#include "elma.h"

//! \file

using namespace std::chrono;
using std::vector;
using namespace elma;

namespace feedback_example {

    //! Example: A car simulation process. See examples/feedback.cc.

    //! See the file examples/feedback.cc for usage.
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
            std::cout << "t: "  << milli_time() << " ms\t" 
                    << " u: " << force        << " N\t"
                    << " v: " << velocity     << " m/s\n";
        }

        //! Nothing to do to stop
        void stop() {}

        private:
        double velocity;
        double force;
        const double k = 0.02;
        const double m = 1000;
    };  

    //! Example: A cruise controller for a Car process. See examples/feedback.cc.

    //! See the file examples/feedback.cc for usage.
    class CruiseControl : public Process {

        public:

        //! Wrap the base process class
        //! \param name The name of the controller    
        CruiseControl(std::string name) : Process(name) {}

        //! Nothing to do to initialize
        void init() {}

        //! Nothing to do to start    
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
        const double desired_speed = 50.0,
                        KP = 314.15;
                        vector<double> _v;

    };   

}

int main() {

    Manager m;

    feedback_example::Car car("Car");
    feedback_example::CruiseControl cc("Control");
    Channel throttle("Throttle");
    Channel velocity("Velocity");

    m.schedule(car, 10_ms)
    .schedule(cc, 10_ms)
    .add_channel(throttle)
    .add_channel(velocity)
    .init()
    .run(1000_ms);

}