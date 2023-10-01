# Application Components Framework

---

Implementation of dependency injection container with few ready to go components. Made in free time to try to
figure out how a good design for an easy-to-use implementation should look like. Short term goal is to provide
everything needed to enable rapid development of simple services (REST as a start) with minimal external dependencies
needed.

> :warning: Treat this code as a hobby project and use at your own risk - I do not guarantee it works in your case
> or on your system. It might be not thread safe or have very poor performance. It is only a small hobby project and I
> work on it in my (limited) free time. Currently, I'm building it only on Ubuntu 22.04 using LLVM 14.0.0 toolchain - it
> will be made to work on other platforms when I'll have more time. 

Uses only Boost libraries as dependencies:
 - core - for type names demangling (could be made optional later)
 - callable_traits - for callable return type deduction (could be imported to codebase)
 - log - for optional BoostLogger component (will be moved to a separate repository with other boost dependant
components)
 - test - for unit testing

## Building

You need a C++ compiler that has support for C++20 features and to build everything from this repository you also have
to install [Bazel](https://bazel.build/) build system.

Building all executables:

`bazel build //...`

Running all unit tests:

`bazel test //...`

Running example application:

`bazel run //examples:printer_service`

## Documentation

Sorry - this section is work in progress. For now please see `examples/printer_service.cpp` for an example of simple
application written using this framework and unit tests in `registry` directory to see possible ways of components
registering and creation. 
