#include <iostream>
#include<vector>
#include<string>

#include <filesystem>
#include "gtest/gtest.h"
#include "refacto/dtables.hpp"

using namespace std;
using namespace dtable;
namespace fs = std::filesystem;

TEST(TableClass, WrongPathConstructor){
    std::string p("wrong_path");    
    try {      
        Table table = Table(p);
      } catch (std::exception& ex) {
          EXPECT_EQ("File not exist at " + p, ex.what());
      }
}

// TEST(TableClass, ){
//     Table table = Table("tests/test_data/test_wrong_columns.yaml");
    
//     std::vector<std::string> key{table.columns()};
//     for(auto& k: key){
//       std::cout<< k << "\n";
//     }
// }
TEST(TableClass, GetterMethods){
    Table table = Table("tests/test_data/inverter_metrics.yaml");
    
    std::vector<std::string> key{table.columns()};
    for(auto& k: key){
      std::cout<< k << "\n";
    }
}
int main(int argc, char* argv[]) {
    namespace fs = std::filesystem;
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

    // return 0;
}