#!/usr/bin/env bash

SCRIPT_DIR=`dirname "$(realpath $0)"`
CODE_DIR="$(realpath $SCRIPT_DIR/..)"
pushd $CODE_DIR/build
make install
cd $CODE_DIR/tests/build
make

cd $CODE_DIR
echo CODE_DIR

# ./tests/build/bin/test_xmlparser
for script in tests/build/bin/*; do $script; done
popd

