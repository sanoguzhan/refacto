#!/usr/bin/env bash

pushd build
make install
popd
pushd tests/build
make 
./bin/test_dtables
popd