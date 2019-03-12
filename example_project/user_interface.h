#include <iostream>
#include <chrono>
#include <vector>
#include <ncurses.h>

#include "stopwatch.h"

namespace stopwatch {

    using namespace std::chrono;
    using namespace elma;
    using namespace stopwatch;

    class UserInterface : public Process {

        public:
        UserInterface(StopWatch& sw);

        void init() {}
        void start() {}
        void show_time(int x, int y, high_resolution_clock::duration d);
        void update();
        void stop() {}

        private:
        StopWatch& _stopwatch;

    };

}