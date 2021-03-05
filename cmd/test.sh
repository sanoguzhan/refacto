#!/usr/bin/env bash

SCRIPT_DIR=`dirname "$(realpath $0)"`
CODE_DIR="$(realpath $SCRIPT_DIR/..)"

pushd $SCRIPT_DIR/build
make install
cd $CODE_DIR/tests/build
make
#ctest
popd
 
<<<<<<< HEAD
./tests/build/bin/test_dtables
./tests/build/bin/test_csvparser
./tests/build/bin/test_table
=======

$CODE_DIR/tests/build/bin/test_dtables
$CODE_DIR/tests/build/bin/test_csvparser
$CODE_DIR/tests/build/bin/test_table
$CODE_DIR/tests/build/bin/test_csv_cases
>>>>>>> e8132b0... Refactor cmd scripts to be independent of running directory
