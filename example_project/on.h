#ifndef _ELMA_STOP_WATCH_ON_H
#define _ELMA_STOP_WATCH_ON_H

#include "stopwatch.h"

namespace stopwatch {

    using namespace std::chrono;
    using namespace elma;

    class StopWatch;

    class OnState : public State {        

        public:
        OnState() : State("on") {}
        void entry(const Event& e) {}
        void during() {}
        void exit(const Event&);

        StopWatch& stopwatch();        

    };

}

#endif