#ifndef __TEST_HEADER_HPP
#define __TEST_HEADER_HPP

#include <limits.h>
#include <time.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"
// #include "refacto/cod.hpp"
#include "refacto/controller.hpp"
#include "refacto/csvparser.hpp"
#include "refacto/customparser.hpp"
#include "refacto/xmlparser.hpp"
using namespace std;
namespace fs = std::filesystem;

class TestTimer : public testing::Test {
   protected:
    void SetUp() override { start_time_ = time(nullptr); }

    void TearDown() override {
        const time_t end_time = time(nullptr);

        EXPECT_TRUE(end_time - start_time_ <= 5) << "The test took too long.";
    }
    time_t start_time_;
};

/**
 * @brief Read a file and return it as string
 *
 * @param path path of the file to read
 * @return std::string
 */
std::string readFile(fs::path path) {
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

/**
 * @brief Constant with the csv test directory
 *
 */
const static string TEST_CSV_DIR = "data/csv/";
/**
 * @brief Constant directory for test with input csv data
 *
 */
const static string TEST_CSV_INPUT_DIR = "data/csv/input/";
/**
 * @brief Constant directory for outputs of csv tests
 *
 */
const static string TEST_CSV_OUTPUT_DIR = "data/csv/expected/";
/**
 * @brief Constant directory for expected results of csv tests
 *
 */
const static string TEST_CSV_EXPECTED_DIR = "data/csv/expected/";
/**
 * @brief Constant directory for expected results of xml tests
 *
 */
const static string TEST_XML_OUTPUT_DIR = "data/";
#endif