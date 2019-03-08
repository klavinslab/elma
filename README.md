Elma
===

Elma is an event loop and process manager for embedded and reactive systems. Elma is structured as a C++ library of classes on top of which you build your own C++ project. It keeps track of processes, finite state machines, events, and communication channels, executing them at specified frequencies. Elma was developed as part of the University of Washington's [ECE P 520 course](https://github.com/klavins/ECEP520) on advanced embedded systems programming. More information on how Elma was designed can be found in the links below:
- [Event loop management](https://github.com/klavins/ECEP520/tree/master/week_6)
- [Events and finite state machines](https://github.com/klavins/ECEP520/tree/master/week_7)
- [HTTP in event loops](https://github.com/klavins/ECEP520/blob/master/week_8)

Installation
===

The source code for Elma [is on Github](https://github.com/klavinslab/elma).

From Dockerhub
---

To get started, you will need a C++ build environment. We have provided one with the Docker container `elma`, which you can use as follows:

    git clone https://github.com/klavinslab/elma.git
    cd elma
    docker run -v $PWD:/source -it klavins/ecep520:elma bash
    make
    examples/bin/basic

From the Dockerfile
---

You can also build the docker environment, described in env/Dockerfile, yourself, with the following commands:

    git clone https://github.com/klavinslab/elma.git
    cd elma
    docker build -t myelma env
    docker run -v $PWD:/source -it myelma bash
    make
    examples/bin/basic


Manual Installation
---

To install Elma without using Docker, you will need to install the following tools:
- [gcc](https://gcc.gnu.org/) 4.9 or an equivalent C/C++ compiler that supports at least C++14
- Make (if you want to use the makefile. Alternatively, you can use some other build manager)
- [Doxygen](http://www.doxygen.nl/)
- [Google Test](https://github.com/google/googletest)
- Neils Lohmann's JSON library: https://github.com/nlohmann/json
- [My fork](https://github.com/klavins/cpp-httplib.git) of [yhirose](https://github.com/yhirose)'s HTTP library

Usage
===
See the examples in the `examples` directory for how to build new event loops with Elma.

License
===

This software is open source and uses the MIT license. Details can be found [here](https://github.com/klavinslab/elma).
