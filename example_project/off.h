#ifndef _ELMA_STOP_WATCH_OFF_H
#define _ELMA_STOP_WATCH_OFF_H

#include "stopwatch.h"

namespace stopwatch {

    using namespace std::chrono;
    using namespace elma;

    class StopWatch;

    class OffState : public State {

        public:
        OffState() : State("off") {}
        void entry(const Event& e) {}
        void during() {}
        void exit(const Event& e);

        StopWatch& stopwatch();

    };

}

#endif