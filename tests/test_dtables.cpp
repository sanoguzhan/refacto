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
   EXPECT_EQ(table.data[0]->Dtype, "datetime");
   EXPECT_EQ(table.data[1]->Dtype, "int");
   EXPECT_EQ(table.data[2]->Dtype, "float");
   
   ASSERT_EQ(table["power_alternate_current"].size(), 0) << "Vectors are unequal length"; 
    try {
        table["date_col"];
        table["power_alternate_current"];
        FAIL() << "Expected std::runtime_error";
    }
    catch(std::runtime_error const & err) {
        EXPECT_EQ(err.what(),std::string("Column not exist"));
    }
}


TEST(DtableTests, TableMethods){
    Table table = Table("tests/test_data/inverter_metrics.yaml");

    ASSERT_TRUE(table.insert("power_alternate_current", std::vector<string>{"1", "2","3"}));  
    ASSERT_EQ(table["power_alternate_current"].size(), 3) << "Vectors are unequal length"; 

}


TEST(DtableTests, GetterMethodsColumn){
    Table table = Table("tests/test_data/inverter_metrics.yaml");
    ASSERT_TRUE(table.get_column("inverter_id")->name == "inverter_id");

}



TEST(DtableTests, GetterMethods){
    Table table = Table("tests/test_data/inverter_metrics.yaml");
    
    std::vector<std::string> key{table.columns()};
    EXPECT_EQ(key[0], "date");
    EXPECT_EQ(key[1], "inverter_id");
    EXPECT_EQ(key[2], "power_alternate_current");
    EXPECT_EQ(key[3], "power_direct_current");
}


TEST(DtableTests, InsertSeries){
    Table table = Table("tests/test_data/inverter_metrics.yaml");
    

    CSVParser p("tests/test_data/csv/2018-07-05.csv", 3);
    
    Loc targets{
        .name="2001",
        .orient="row",
        .row=1
    };
    
    Loc cond1{
        .name="Pac",
        .orient="row",
        .row=2
    };

    Loc targets2{
        .name="2001",
        .orient="row",
        .row=1
    };
    
    Loc cond2{
        .name="Uac",
        .orient="row",
        .row=2
    };

    auto cols{p.values("row", 4, targets, cond1)};
    auto cols2{p.values("row", 4, targets2, cond2)};
     
    for(auto& p:cols.values){
        ASSERT_EQ(75, p.second.size());
    }
    table.insert("power_direct_current", "inverter_id", cols);
    table.insert("power_alternate_current", "inverter_id", cols2);

    std::vector<std::string> id_vec{table["inverter_id"]};
    std::vector<std::string> col_vec{table["power_direct_current"]};

    ASSERT_EQ(id_vec.size(),1800);
    ASSERT_EQ(col_vec.size(),1800);
    table.info();
    table.save();

}




int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}