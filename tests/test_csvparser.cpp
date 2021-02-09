#include "test_header.hpp"


class CSVParserTest: public TestTimer{
};

TEST(CSVParser, WrongPath){
    string p("tests/test_data/test.csv");
    try{
        CSVParser csv = CSVParser(p);
    } catch (std::exception& ex) {
          EXPECT_EQ("CSV not exist at "+p, (std::string)ex.what());
      }    

}


TEST(CSVParser, ReadEachRow){
    CSVParser p("tests/test_data/csv/2018-07-05.csv");
    ASSERT_EQ(p.data.size(), 81);   
}


TEST(CSVParser, ReadAll){
    CSVParser p("tests/test_data/csv/2018-07-05.csv", 3);
    ASSERT_EQ(p.data.size(), 79);
}

TEST(CSVParser, GetValTest){
    CSVParser p("tests/test_data/csv/2018-07-05.csv", 3);
    Loc loc{
        .name="WR09",
        .orient="row",
        .row=0
    };    
    p.get_val(loc);


}



int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}