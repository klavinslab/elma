#include <iostream>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "elma.h"

namespace {

    using namespace elma;
    using std::vector;

    const int Priority_min = -5, Priority_max = 15;
    static vector<string> test_output;    
    static vector<string> ans1 = {"lily", "bobby", "james", "lily", "bobby", "james", "lily", "bobby", "james" };  

    class  Tester: public elma::Process {

        public: 
        Tester(string name, int n = 0) : Process(name, n) {}
        void init() {}
        void start() {}
        void update() {
            test_output.push_back(name());
        }
        void stop() {}

    };    

    TEST(Priority, basic) {              
    
        elma::Manager m;
        Tester james("james",0);
        Tester lily("lily", 2);
        Tester bobby("bobby", 1);

        test_output = {};
        m.schedule(james, 30_ms)
         .schedule(lily, 30_ms)
         .schedule(bobby, 30_ms)
         .init()
         .run([]() { return test_output.size() < 9; });

        EXPECT_EQ(test_output, ans1);
        
    }

    TEST(Priority, SetPriorityMethod) {

        elma::Manager m;
        Tester james("james");
        Tester lily("lily");
        Tester bobby("bobby");

        test_output = {};
        m.schedule(james, 30_ms)
         .schedule(lily, 30_ms)
         .schedule(bobby, 30_ms)
         .set_priority(james, 0)
         .set_priority(lily, 2)
         .set_priority(bobby, 1)
         .init()
         .run([]() { return test_output.size() < 9; });

        EXPECT_EQ(test_output, ans1);
        
    }
    
    TEST(Priority, OutOfBounds) {

        class  Tester: public elma::Process {
        public: 
            Tester(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {}
            void stop() {}

        };

        elma::Manager m;
        Tester james("james",19);
        EXPECT_ANY_THROW( m.schedule(james, 30_ms));
        Tester bob("bob");
        EXPECT_ANY_THROW(m.set_priority(bob, 23)); 
    }

}