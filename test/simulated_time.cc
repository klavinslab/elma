#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "gtest/gtest.h"
#include "elma.h"

namespace {

    using namespace elma;
    using std::vector;

    class SimProcess : public Process {
        public:
        SimProcess(string name) : Process(name) {}
        void init() {}
        void start() {}
        void update() {}
        void stop() {}
    };    

    TEST(SimulatedTime, normal) {
        SimProcess tp("Normal Process");
        Manager m;
        m.schedule(tp, 10_ms)
        .init();

        auto start = std::chrono::high_resolution_clock::now();
        m.run(1s);
        auto end = std::chrono::high_resolution_clock::now();

        auto runtime = end - start;

        EXPECT_TRUE(runtime > 900ms);
        EXPECT_TRUE(runtime < 1100ms);
    }

    TEST(SimulatedTime, Simulated) {
        SimProcess tp("Simulated Process");
        Manager m;
        m.schedule(tp, 10ms)
        .init()
        .use_simulated_time();

        auto start = std::chrono::high_resolution_clock::now();
        m.run(1s);
        auto end = std::chrono::high_resolution_clock::now();

        auto runtime = end - start;

        EXPECT_TRUE(runtime > 0s);
        EXPECT_TRUE(runtime < 100ms);
    }

}