#include <iostream>
#include <chrono>
#include <vector>

#include "stopwatch.h"

using namespace std::chrono;
using namespace elma;
using namespace stopwatch;

StopWatch& OffState:: stopwatch() { return (StopWatch&) state_machine(); }

void OffState::exit(const Event& e) {
    if ( e.name() == "start/stop" ) {
        stopwatch().begin();
    } else if ( e.name() == "reset" ) {
        stopwatch().reset();
    }
}
