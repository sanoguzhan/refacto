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
    std::vector<u_int32_t> expected{7,8,9,10,11,12,13,14,15,16,17,
                                    18,19,20,21,22,23,24,25,26,27,
                                    28,29,30,31,32,33,34,35,36,37,
                                    38,39,40,41,42,43,44,45,46,47,
                                    48,49,50,51,52,53,54,55,56,57,
                                    58,59,60,61,62,63,64,65,66,67,
                                    68,69,70,71,72,73,74,75,76,77, 
                                    78};
    std::vector<u_int32_t> actual{row_search(p.data, loc)};
    ASSERT_EQ(actual, expected);

}



int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}