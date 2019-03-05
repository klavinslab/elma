#include <iostream>
#include <chrono>
#include "gtest/gtest.h"
#include "elma.h"

//! \file

using namespace std::chrono;
using namespace elma;

namespace microwave_example {

    //! A class the models the power component of a microwave oven. See examples/microwave.cc.
    class Power : public Process {

        public:

        //! Create a new power component
        Power() : Process("power") {}

        //! Set up watches for three events: "on", "off", and "set power".
        //! The "set power" watcher expects events whose values are numerical.
        void init() {
            watch("on", [this](Event& e) {
                running = true;
            });
            watch("off", [this](Event &e) {
                running = false;
            });
            watch("set power", [this](Event& e) {
                power_level = e.value();
            });
        }

        //! Set running to false and power_level to zero
        void start() {
            running = false;
            power_level = 0;
        }

        //! Nothing to do to update
        void update() {}

        //! Nothing to do to stop
        void stop() {}

        //! Keep track of whether the power is on
        bool running;

        //! Keep track of the power level
        double power_level;

    };

    //! The state in which the oven's door is closed and the oven is off. See examples/microwave.cc.
    class DoorClosedOff : public State {

        public:
        //! Make a new state
        DoorClosedOff() : State("Door closed off") {}

        //! If this state is entered via a "power level set" event
        //! from the user, then emit a "set power" event (to be)
        //! consumed by the power component.
        //! \param e A numerically valued event
        void entry(const Event& e) {
            if ( e.name() == "power level set" ) {
                emit(Event("set power", e.value()));
            }
        }

        //! Nothing to do in during
        void during() {} 

        //! If this state is left via an "on button pressed" event,
        //! then emit a "on" event.
        //! \param e An event with no value
        void exit(const Event& e) {
            if ( e.name() == "on button pressed" ) {
                emit(Event("on"));
            }        
        }

    };

    //! The state in which the oven's door is closed and the oven is on. See examples/microwave.cc.
    class DoorClosedOn : public State {
        public:
        //! Make a new state
        DoorClosedOn() : State("Door closed on") {}

        //! Nothing to do upon entry        
        void entry(const Event& e) {}

        //! Nothing to do in during 
        void during() {} 

        //! As this state is left, turn off the oven.
        //! \param e An event with no value        
        void exit(const Event& e) {
            emit(Event("off"));
        }
    };

    //! The state in which the oven's door is open and the oven is off. See examples/microwave.cc.
    class DoorOpen : public State {
        public:
        //! Make a new state
        DoorOpen() : State("Door open") {}

        //! Upon entry, 
        void entry(const Event& e) {
            if ( e.name() == "power level set" ) {
                emit(Event("set power", e.value()));
            }           
        }
        //! Nothing to do during this state
        void during() {} 

        //! Nothing to do upon exiting this state
        void exit(const Event& e) {}
    };

}

TEST(Microwave, Safety) {

    Manager m;

    microwave_example::Power power;

    microwave_example::DoorClosedOff closed_off;
    microwave_example::DoorClosedOn closed_on;
    microwave_example::DoorOpen open;        

    StateMachine microwave;

    microwave
      .set_initial(closed_off)
      .add_transition("on button pressed", closed_off, closed_on)
      .add_transition("door opened", closed_off, open)
      .add_transition("power level set", closed_off, closed_off)

      .add_transition("off button pressed", closed_on, closed_off)
      .add_transition("door opened", closed_on, open)

      .add_transition("door closed", open, closed_off)
      .add_transition("power level set", open, open);

    m.schedule(power, 10_ms)
      .schedule(microwave, 10_ms)
      .init();

    m.start();
    m.emit(Event("on button pressed"));
    ASSERT_EQ(true, power.running);

    m.start();
    m.emit(Event("door opened"));    
    m.emit(Event("on button pressed"));
    ASSERT_EQ(false, power.running); 

    // INSERT GRADING TESTS HERE 

}

GTEST_API_ int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}