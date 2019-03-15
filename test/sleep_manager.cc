#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "gtest/gtest.h"
#include "elma.h"

namespace {

    using namespace elma;

    class TestManager : public Manager {
        public:
        TestManager() : Manager() {}

        using Manager::_update_time_calls;
    };

    class SimProcess : public Process {
        public:
        SimProcess(string name) : Process(name) {}
        void init() {}
        void start() {}
        void update() {}
        void stop() {}
    };    

    TEST(SleepManager, Realtime) {
        SimProcess tp("Realtime Process");
        TestManager m;
        m.schedule(tp, 10_ms)
        .init()
        .run([&](){return tp.num_updates() <= 100;});
        
        EXPECT_GE(m._update_time_calls, 99);
        EXPECT_LE(m._update_time_calls, 101);
    }

    TEST(SleepManager, Simulated) {
        SimProcess tp("Simulated Process");
        TestManager m;
        m.schedule(tp, 10_ms)
        .init()
        .use_simulated_time()
        .run([&](){return tp.num_updates() <= 100;});
        
        EXPECT_GE(m._update_time_calls, 99);
        EXPECT_LE(m._update_time_calls, 101);
    }
    
    TEST(SleepManager, NoProcesses) {
        TestManager m;
        m.init()
        .use_simulated_time()
        .run(100ms);
        
        EXPECT_GT(m._update_time_calls, 1000);
    }

}