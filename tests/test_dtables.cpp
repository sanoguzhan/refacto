#include <iostream>
#include<vector>
#include<string>
#include <filesystem>
#include "gtest/gtest.h"
#include "refacto/dtables.hpp"

using namespace std;

TEST(TESTtry, SubTest){
    namespace fs = std::filesystem;
  std::cout << "Current path is " << fs::current_path() << '\n';
    // keys("./tests/test_data/inverter_metrics.yaml");
    EXPECT_EQ(0,0);
}


int main(int argc, char* argv[]) {

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

    return 0;
}