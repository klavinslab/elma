#include <unistd.h>
#include "stopwatch.h"

using namespace stopwatch;

UserInterface::UserInterface(StopWatch& sw) : Process("user input"), _stopwatch(sw) {
    initscr();
    timeout(1);
    noecho();
    curs_set(0);        
};

void UserInterface::show_time(int x, int y, high_resolution_clock::duration d) {
    mvprintw(x,y,"%d:%02d:%02d", 
        std::chrono::duration_cast<std::chrono::minutes>(d).count(),
        std::chrono::duration_cast<std::chrono::seconds>(d).count()%60,
        (std::chrono::duration_cast<std::chrono::milliseconds>(d).count()%1000)/10
    );
}

void UserInterface::update() {

    int c = getch();

    switch ( c ) {            
        case 's':
            emit(Event("start/stop"));
            break;
        case 'r':
            emit(Event("reset"));
            clear();
            break;
        case 'l':
            emit(Event("lap"));
            break;
        case 'q':
            std::cout << "halting\n";
            halt();
            break;
    }

    show_time(1,1,_stopwatch.value());
    mvprintw(3,1,"start/stop(s), lap(l), reset(r), quit(q)");
    for ( int i=0; i<_stopwatch.laps().size(); i++ ) {
        mvprintw(5+i, 1, "Lap %d", _stopwatch.laps().size()-i);
        show_time(5+i, 10, _stopwatch.laps()[i]);
    }

    usleep(10000);

}