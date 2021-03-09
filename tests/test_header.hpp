#ifndef __TEST_HEADER_HPP
#define __TEST_HEADER_HPP

#include <iostream>
#include <vector>
#include <string>
#include <limits.h>
#include <time.h>
#include <filesystem>
#include <fstream>
#include <string>

#include "gtest/gtest.h"
#include "../include/dtables.hpp"
#include "../include/csvparser.hpp"
using namespace std;
using namespace dtable;
namespace fs = std::filesystem;

class TestTimer : public testing::Test
{
protected:
    void SetUp() override { start_time_ = time(nullptr); }

    void TearDown() override
    {
        const time_t end_time = time(nullptr);

        EXPECT_TRUE(end_time - start_time_ <= 5) << "The test took too long.";
    }
    time_t start_time_;
};

std::string readFile(fs::path path)
{
    // Open the stream to 'lock' the file.
    std::ifstream f(path, std::ios::in | std::ios::binary);

    // Obtain the size of the file.
    const auto sz = fs::file_size(path);

    // Create a buffer.
    std::string result(sz, '\0');

    // Read the whole file into the buffer.
    f.read(result.data(), sz);

    return result;
}

const static string TEST_CSV_DIR = "tests/test_data/csv/";
const static string TEST_CSV_INPUT_DIR = "tests/test_data/csv/input/";
const static string TEST_CSV_OUTPUT_DIR = "tests/test_data/csv/output/";
const static string TEST_CSV_EXPECTED_DIR = "tests/test_data/csv/expected/";

#endif