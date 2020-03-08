#include <iostream>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "elma.h"

namespace {

    using namespace elma;
    using std::vector;

    class TestProcess : public Process {
        public:
        TestProcess(std::string name) : Process(name) {}
        void init() {}     
        void start() {}
        void update() { 
            std::cout << "updating " << name() << "\n";
        }
        void stop() {
            std::cout << "stopping " << name() << "\n";
        }
    };

    TEST(Event,SendRecv) {
        TestProcess p("P"), q("Q");
        Manager m;
        m.schedule(p,10_ms)
         .schedule(q,10_ms)
         .init();
        
        m.run(100_ms);
        m.remove(q);
        m.run(100_ms);

    }

}