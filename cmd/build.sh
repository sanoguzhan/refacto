#!/usr/bin/env bash

pushd build
rm -rf *
conan install .. 
cmake ..
make install
popd

pushd tests/build
rm -rf *
conan install ../..
cmake ..
popd
