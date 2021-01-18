#!/usr/bin/env bash
set -eou pipefail

# Gtest and Gmock
cd ~
git clone https://github.com/google/googletest.git
cd googletest
mkdir build && cd build
cmake .. -DBUILD_SHARED_LIBS=ON -DINSTALL_GTEST=ON -DCMAKE_INSTALL_PREFIX:PATH=/usr
make -j8
make install
ldconfig

cd ~
git clone https://github.com/jbeder/yaml-cpp.git
cd yaml-cpp 
mkdir build && cd build
cmake .. -DBUILD_SHARED_LIBS=ON -DINSTALL_GTEST=ON -DCMAKE_INSTALL_PREFIX:PATH=/usr