#!/usr/bin/env bash

# Set reference directory to make path relative to this
SCRIPT_DIR=`dirname "$(realpath $0)"`
CODE_DIR="$(realpath $SCRIPT_DIR/..)"
pushd $CODE_DIR

# Compile project
export UBSAN_OPTIONS=suppressions=ubsan.supp
mkdir -p build
cppcheck -i/pugixml.cpp --language=c++ --enable=all --xml --xml-version=2 src 2> cppcheck-result.xml

cd build
rm -rf *

#  Enable Code Analysis -DENABLE_CODE_ANALYSIS=ON 
conan install $CODE_DIR 
cmake -G "Ninja" $CODE_DIR 
make install

# Compile tests
mkdir -p $CODE_DIR/tests/build
cd $CODE_DIR/tests/build
rm -rf *
conan install $CODE_DIR
cmake -G "Ninja" $CODE_DIR/tests -Wno-deprecated 

popd
