#!/usr/bin/env bash

SCRIPT_DIR=`dirname "$(realpath $0)"`
CODE_DIR="$(realpath $SCRIPT_DIR/..)"

pushd $SCRIPT_DIR/build
make install
cd $CODE_DIR/tests/build
make
#ctest
 
cd $CODE_DIR
for script in tests/build/bin/*; do $script; done

# tests/build/bin/test_csvparser

rm tests/test_data/csv/output/*.csv

popd

