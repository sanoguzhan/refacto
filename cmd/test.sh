#!/usr/bin/env bash

pushd build
make install
popd
pushd tests/build
make
#ctest
popd
 
./tests/build/bin/test_dtables