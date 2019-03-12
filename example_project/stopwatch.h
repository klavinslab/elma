#ifndef _ELMA_STOP_WATCH_H
#define _ELMA_STOP_WATCH_H

#include "elma.h"
#include "on.h"
#include "off.h"
#include "user_interface.h"

namespace stopwatch {

    using namespace std::chrono;
    using namespace elma;    

    class StopWatch : public StateMachine {

        public:
        StopWatch();

        void begin();
        void reset();
        void stop();
        void lap();
        high_resolution_clock::duration value();

        const vector<high_resolution_clock::duration>& laps() { return _laps; }

        private:
        OnState on;
        OffState off;
        bool _running;

        high_resolution_clock::time_point _start_time;
        high_resolution_clock::duration _elapsed;
        vector<high_resolution_clock::duration> _laps;

    };

}

#endif