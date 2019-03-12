#include <chrono>
#include <vector>

#include "stopwatch.h"

using namespace std::chrono;
using namespace elma;
using namespace stopwatch;

StopWatch::StopWatch() : StateMachine("stopwatch") {
    set_initial(off);
    set_propagate(false);
    add_transition("start/stop", off, on);
    add_transition("reset", off, off);            
    add_transition("start/stop", on, off);
    add_transition("lap",   on, on);
    reset();
}

high_resolution_clock::duration StopWatch::value() {
    if ( current().name() == "on" ) {
        return high_resolution_clock::now() - _start_time + _elapsed;
    } else {
        return _elapsed;
    }
}

void StopWatch::begin() {
    _start_time = high_resolution_clock::now();
}

void StopWatch::reset() {
    _elapsed = high_resolution_clock::duration::zero();
    _laps.clear();
}

void StopWatch::stop() {
    _elapsed += high_resolution_clock::now() - _start_time;
}

void StopWatch::lap() {
    _laps.insert(_laps.begin(), value());
}