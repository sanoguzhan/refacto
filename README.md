
# Refacto & Parsing Library

[![PyPI implementation](https://img.shields.io/pypi/implementation/ansicolortags.svg)](https://pypi.python.org/pypi/ansicolortags/)
[![PyPI pyversions](https://img.shields.io/pypi/pyversions/ansicolortags.svg)](https://pypi.python.org/pypi/ansicolortags/)
[![Documentation Status](https://readthedocs.org/projects/ansicolortags/badge/?version=latest)](http://ansicolortags.readthedocs.io/?badge=latest)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com)

[Quickstart](docs/quickstart.md)

Parsing library for re-structuing datasets. Parsing Library consist of two packages parsing python library as frontent and refacto c++ library as backend.

Goal of the project is to decrease the time needed to format data into standarized database structure. It is designed for [Quantom SAS](https://quantom.solar/fr/).
Receiving bug amount of data in various formats such as xml, zip, csv etc. creates big human resources loss in order to utilize the data for analysis algorithms. For this reason, backend implemented in C++ to achieve high performance meanhwhile, all packaged can be used from python for simplicity.

Run the project:

```sh
    cd docker
    docker-compose build
    docker-compose run clang bash
    ./qparsing/core/cmd/build.sh
```

For Tests:

```sh
    ./qparsing/core/cmd/test.sh 
```

This will compile all the code, build the c++ library and run the tests.

Modules:

| Name          | Type           | Library
| ------------- |:-------------:|-------------:|
| [XMLParser](https://github.com/sanoguzhan/refacto/blob/master/src/core/include/xmlparser.hpp)    |  XML files       | Refacto & Parsing
| [CSVParser](https://github.com/sanoguzhan/refacto/blob/master/src/core/include/csvparser.hpp)     | CSV files      |   Refacto & Parsing 
| [Decompressdir](https://github.com/sanoguzhan/refacto/blob/master/src/core/include/cod.hpp) | Compressed files | Refactor & Parsing
| [Compressdir](https://github.com/sanoguzhan/refacto/blob/master/src/core/include/cod.hpp) | XML and CSV files | Refactor & Parsing
| [Cleandir](https://github.com/sanoguzhan/refacto/blob/master/src/core/include/cod.hpp) | Clean files | Refactor & Parsing
------
## Getting started


To get started, install the library with 

```sh
    ./install_lib.sh
```
  

## Example 


Import the modules and run

```python

    >>> from parsing.engine import xmlparser
    >>> xml_parser = XMLParser(...)
    >>> xml_parser(kwargs)

```

see code examples at [example.py](https://github.com/sanoguzhan/refacto/blob/master/docs/example.py)