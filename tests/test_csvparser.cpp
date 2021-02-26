#include "test_header.hpp"


class CSVParserTest: public TestTimer{
};

TEST(CSVParser, TestFreeFunctions){
    CSVParser p("tests/test_data/csv/2018-07-05.csv");
    ASSERT_EQ(p.file_name, "2018-07-05");   
}

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

TEST(CSVParser, RowSearch){
    CSVParser p("tests/test_data/csv/2018-07-05.csv", 3);
    Loc loc1{
        .name="WR",
        .orient="row",
        .row=0
    };

    Loc loc2{
    .name="Pac",
    .orient="row",
    .row=2
    };
    std::vector<u_int32_t> expected{17,34,51,68,85,102,119,
                                    136,153,170,187,204,221,
                                    238,255,272,289,306,323,
                                    340,357,374,391,408};

    std::vector<u_int32_t> actual{row_search(p.data, loc1, loc2)};

    ASSERT_EQ(actual, expected);
}



TEST(CSVParser, GetValsTwoCond){
    CSVParser p("tests/test_data/csv/2018-07-05.csv", 3);
    
    Loc targets{
        .name="2001",
        .orient="row",
        .row=1
    };
    
    Loc cond1{
        .name="WR",
        .orient="row",
        .row=0
    };

    Loc cond2{
        .name="Pac",
        .orient="row",
        .row=2
    };

    auto cols{p.values("row", 4, targets, cond1, cond2)};
    for(auto& p:cols.values){
        ASSERT_EQ(75, p.second.size());
    }
}

TEST(CSVParser, GetValsOneCond){
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

    auto cols{p.values("row", 4, targets, cond1)};
    for(auto& p:cols.values){
        ASSERT_EQ(75, p.second.size());
}
}
TEST(CSVParser, OneItemCapture){
    CSVParser p("tests/test_data/csv/min200611.csv", 0);
    Loc date{
        .name="date",
        .row=1,
        .column=0
    };
    auto date_value{p.value(date)};
    ASSERT_EQ(date_value, "11/06/20");
}

TEST(CSVParser, SliceSelection){
    CSVParser p("tests/test_data/csv/min200611.csv", 0);
    
    auto date_value{p.values("row", 0,0,20)};
    ASSERT_EQ(date_value.size(), 20);
}


int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}