#include "test_header.hpp"

class DtableTests: public TestTimer{
};

TEST(TableTests, TableConstructor){
    std::string p("wrong_path");    
    try {      
        Tables table = Tables(p);
      } catch (std::exception& ex) {
          EXPECT_EQ("File not exist at " + p, ex.what());
      }
}




int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}