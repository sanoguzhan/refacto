#ifndef __TEST_HEADER_HPP
#define __TEST_HEADER_HPP



#include <iostream>
#include<vector>
#include<string>
#include <limits.h>
#include <time.h>
#include <filesystem>

#include "gtest/gtest.h"
#include "refacto/dtables.hpp"

using namespace std;
using namespace dtable;
namespace fs = std::filesystem;

class TestTimer: public testing::Test{
    protected:
        void SetUp() override { start_time_ = time(nullptr); }

        void TearDown() override {
             const time_t end_time = time(nullptr);


            EXPECT_TRUE(end_time - start_time_ <= 5) << "The test took too long.";
        }
         time_t start_time_;
};

#endif