#!/usr/bin/env bash

# Set reference directory to make path relative to this
SCRIPT_DIR=`dirname "$(realpath $0)"`
CODE_DIR="$(realpath $SCRIPT_DIR/..)"
pushd $CODE_DIR

# Compile project
mkdir -p build
cd build
rm -rf *
conan install $CODE_DIR
cmake $CODE_DIR  -Wno-deprecated
make install

# Compile tests
mkdir -p $CODE_DIR/tests/build
cd $CODE_DIR/tests/build
rm -rf *
conan install $CODE_DIR
cmake $CODE_DIR/tests -Wno-deprecated

popd
