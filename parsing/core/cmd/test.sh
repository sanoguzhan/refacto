#!/usr/bin/env bash

SCRIPT_DIR=`dirname "$(realpath $0)"`
CODE_DIR="$(realpath $SCRIPT_DIR/..)"

pushd $CODE_DIR/build
make install
cd $CODE_DIR/tests/build
make
#ctest
 
cd $CODE_DIR
for script in tests/build/bin/*; do $script; done
popd

