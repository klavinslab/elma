#include <iostream>
#include <chrono>
#include <vector>
#include <ncurses.h>

#include "stopwatch.h"

using namespace std::chrono;
using namespace elma;
using namespace stopwatch;

int main() {

    Manager m;
    StopWatch stopwatch;
    UserInterface ui(stopwatch);

    m.schedule(ui, 10_ms)
     .schedule(stopwatch, 10_ms)     
     .init()
     .run();

    endwin();     
     
}