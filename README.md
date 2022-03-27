# Refacto & Parsing Library

[![PyPI implementation](https://img.shields.io/pypi/implementation/ansicolortags.svg)](https://pypi.python.org/pypi/ansicolortags/)
[![Documentation Status](https://readthedocs.org/projects/ansicolortags/badge/?version=latest)](http://ansicolortags.readthedocs.io/?badge=latest)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)


Parsing library for re-structuring datasets of pv plants. Parsing Library consist of two packages parsing python library as an extension and refacto c++ library.



## Prerequisites

CMake v3.15+ - found at https://cmake.org/

C++ Compiler - needs to support at least the C++17 standard, i.e. MSVC, GCC, Clang

----

## Env Setup 

### VSCode 

Build dev container 

### Terminal

```sh
    cd docker
    docker-compose build
    docker-compose run refacto bash
```

---

## Run Project 

### Build

```sh
    mkdir build/ && cd build/
    cmake .. -DCMAKE_INSTALL_PREFIX=/absolute/path/to/custom/install/directory
    cmake --build . --target install 
```
Note: to disable to build  test add `-DRefacto_ENABLE_UNIT_TESTING=OFF` flag

### Run Tests

```sh 
    cd build          # if not in the build directory already
    ctest -C Release  # or `ctest -C Debug` or any other configuration you wish to test
```


This will compile all the code, build the c++ library and run the tests.

Modules:

| Name                                                                                          |       Type        |            Library |
| --------------------------------------------------------------------------------------------- | :---------------: | -----------------: |
| [XMLParser](https://github.com/sanoguzhan/refacto/blob/master/include/refacto/xmlparser.hpp) |     XML files     |  Refacto & Parsing |
| [CSVParser](https://github.com/sanoguzhan/refacto/blob/master/include/refacto/csvparser.hpp) |     CSV files     |  Refacto & Parsing |
| [Decompressdir](https://github.com/sanoguzhan/refacto/blob/master/include/refacto/cod.hpp)   | Compressed files  | Refactor & Parsing |
| [Compressdir](https://github.com/sanoguzhan/refacto/blob/master/include/refacto/cod.hpp)     | XML and CSV files | Refactor & Parsing |
| [Cleandir](https://github.com/sanoguzhan/refacto/blob/master/include/refacto/cod.hpp)        |    Clean files    | Refactor & Parsing |

---


## Extension


### Python

Python Package QParsing is located interface/py3

*To build the package*

```sh
    python3 setup.py install_library build_ext sdist bdist_wheel
```