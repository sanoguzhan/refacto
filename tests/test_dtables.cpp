#include "test_header.hpp"
class DtableTests: public TestTimer{
};

TEST(DtableTests, WrongPathConstructor){
    std::string p("wrong_path");    
    try {      
        Table table = Table(p);
      } catch (std::exception& ex) {
          EXPECT_EQ("File not exist at " + p, ex.what());
      }
}

TEST(DtableTests, InvalidConfigFileError){
       try {      
          Table table = Table("tests/test_data/test_wrong_columns.yaml");
   
      } catch (std::exception& ex) {
          EXPECT_EQ("Configuration missing COLUMNS ", (std::string)ex.what());
      } 
    
}


TEST(DtableTests, DataAttributes){
   Table table = Table("tests/test_data/inverter_metrics.yaml");
  //   // EXPECT_EQ(table.data[0]->Dtype, "datetime");
  //   // EXPECT_EQ(table.data[1]->Dtype, "int");
  //   // EXPECT_EQ(table.data[2]->Dtype, "double");
  //  table["power_alternate_current"]; 
    try {
        table["date_col"];
        // table["power_alternate_current"];
        FAIL() << "Expected std::runtime_error";
    }
    catch(std::runtime_error const & err) {
        EXPECT_EQ(err.what(),std::string("Column not exist"));
    }
}

TEST(DtableTests, GetterMethods){
    Table table = Table("tests/test_data/inverter_metrics.yaml");
    
    std::vector<std::string> key{table.columns()};
    EXPECT_EQ(key[0], "date");
    EXPECT_EQ(key[1], "inverter_id");
    EXPECT_EQ(key[2], "power_alternate_current");
    EXPECT_EQ(key[3], "power_direct_current");
    table.info();
}
int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}