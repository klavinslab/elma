#include <iostream>
#include <chrono>
#include <vector>

#include "stopwatch.h"

using namespace std::chrono;
using namespace elma;
using namespace stopwatch;

StopWatch&OnState:: stopwatch() { return (StopWatch&) state_machine(); }

void OnState::exit(const Event& e) {
    if ( e.name() == "start/stop" ) {
        stopwatch().stop();
    } else if ( e.name() == "lap" ) {
        stopwatch().lap();
    }
}
