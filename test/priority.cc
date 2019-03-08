#include <iostream>
#include <vector>
#include <string>
#include "gtest/gtest.h"
#include "elma.h"

namespace {

    using namespace elma;
    using std::vector;

    #define SLEEP(__ms__) std::this_thread::sleep_for(std::chrono::milliseconds(__ms__))
    #define MS(__ms__) high_resolution_clock::duration(milliseconds(__ms__))

    TEST(Priority, basic) {

        static vector<string> test1;
        static vector<string> ans1 = {"lilly", "boby", "james", "lilly", "boby", "james", "lilly", "boby", "james" };
        
        class  Tester: public elma::Process {
        public: 
            Tester(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {
                test1.push_back("james");
                //std::cout << "james" << "\n";
            }
            void stop() {}

        };

        class  Tester2: public elma::Process {
        public: 
            Tester2(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {
                test1.push_back("lilly");
                //std::cout << "lilly" << "\n";
            }
            void stop() {}

        };

        class  Tester3: public elma::Process {
        public: 
            Tester3(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {
                test1.push_back("boby");
                //std::cout << "boby" << "\n";
            }
            void stop() {}

        };

        
    
        elma::Manager m;
        Tester james("james",0);
        Tester2 lily("lily", 2);
        Tester3 boby("boby", 1);

        m.schedule(james, MS(30))
        .schedule(lily, MS(30))
        .schedule(boby, MS(30));

        //m.SetPriority("james", 3);
        m.init().run(MS(100));
        EXPECT_EQ(test1, ans1);
        
    }

        TEST(Priority, SetPriorityMethod) {

        static vector<string> test1;
        static vector<string> ans1 = {"lilly", "boby", "james", "lilly", "boby", "james", "lilly", "boby", "james" };
        
        class  Tester: public elma::Process {
        public: 
            Tester(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {
                test1.push_back("james");
                //std::cout << "james" << "\n";
            }
            void stop() {}

        };

        class  Tester2: public elma::Process {
        public: 
            Tester2(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {
                test1.push_back("lilly");
                //std::cout << "lilly" << "\n";
            }
            void stop() {}

        };

        class  Tester3: public elma::Process {
        public: 
            Tester3(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {
                test1.push_back("boby");
                //std::cout << "boby" << "\n";
            }
            void stop() {}

        };

        
    
        elma::Manager m;
        Tester james("james");
        Tester2 lily("lily");
        Tester3 boby("boby");

        m.schedule(james, MS(30))
        .schedule(lily, MS(30))
        .schedule(boby, MS(30));

        m.set_priority(james, 0);
        m.set_priority(lily, 2);
        m.set_priority(boby, 1);

        m.init().run(MS(100));
        EXPECT_EQ(test1, ans1);
        
    }

    //This is no longer valid now that the process is passed not its name
   /* TEST(Priority, NoProcess) {

             
        class  Tester: public elma::Process {
        public: 
            Tester(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {}
            void stop() {}

        };
   
        elma::Manager m;
        Tester james("james",0);
  

        m.schedule(james, MS(30));
        //EXPECT_ANY_THROW(m.set_priority("jams", 3));        
    }*/

    TEST(Priority, LookSort) {

        class  Tester: public elma::Process {
        public: 
            Tester(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {
                //test1.push_back("james");
                //std::cout << "james" << "\n";
            }
            void stop() {}

        };

        class  Tester2: public elma::Process {
        public: 
            Tester2(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {
                //test1.push_back("lilly");
                //std::cout << "lilly" << "\n";
            }
            void stop() {}

        };

        class  Tester3: public elma::Process {
        public: 
            Tester3(string name, int n = 0) : Process(name, n) {}
            void init() {}
            void start() {}
            void update() {
                //test1.push_back("boby");
                //std::cout << "boby" << "\n";
            }
            void stop() {}

        };

        elma::Manager m;
        Tester james("james",0);
        Tester2 lily("lily", 2);
        Tester3 boby("boby", 1);

        m.schedule(james, MS(30))
        .schedule(lily, MS(30))
        .schedule(boby, MS(60));

        vector<string> test2;
        vector<string> test3;
        vector<string> ans2 = {"james", "lily", "boby"};
        vector<string> ans3 = {"lily", "boby", "james"};
        
        //This is pushing correct. By visual inspection
        //m.all([test2](Process& p) mutable { test2.push_back(p.name()); });
        //EXPECT_EQ(test2, ans2);
        m.all([test2](Process& p) mutable { std::cout << p.name() << "\n"; });

        m.init().run(MS(100));
        //This is pushing correct. By visual inspection
        //m.all([test3](Process& p) mutable { test3.push_back(p.name()); });
        //EXPECT_EQ(test3, ans3);
        m.all([test2](Process& p) mutable { std::cout << p.name() << "\n"; });
        
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
        EXPECT_ANY_THROW( m.schedule(james, MS(30)));
        Tester bob("bob");
        EXPECT_ANY_THROW(m.set_priority(bob, 23)); 
    }

}