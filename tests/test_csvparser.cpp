#include "test_header.hpp"


class CSVParserTest: public TestTimer{
};

/* Test for CSVParser
    - Test for openning file
    - Test for reading file and 2-d vector construction
    _ Test for CSVParser values methods
*/  

TEST(CSVParser, TestFreeFunctions){
    CSVParser p(TEST_CSV_INPUT_DIR + "2018-07-05.csv");
    ASSERT_EQ(p.file_name, "2018-07-05");   
}

TEST(CSVParser, WrongPath){
    /* Test for CSV reader 
    - wrong path should rise error
    */  
    string p("tests/test_data/test.csv");
    try{
        CSVParser csv = CSVParser(p);
    } catch (std::exception& ex) {
          EXPECT_EQ("CSV not exist at "+p, (std::string)ex.what());
      }    

}


TEST(CSVParser, ReadEachRow){
    /* Test for CSV row reader
    - Read value must match with row size
    */  
    CSVParser p(TEST_CSV_INPUT_DIR + "2018-07-05.csv");
    CSVParser p2(TEST_CSV_INPUT_DIR + "2018-07-05.csv", 3);

    ASSERT_EQ(p.data.size(), 81);   
    ASSERT_EQ(p2.data.size(), 79);
}



TEST(CSVParser, RowSearch){
    /* Test for CSV row search
        - Found index of the search variables should match 
    */  
    CSVParser p(TEST_CSV_INPUT_DIR + "2018-07-05.csv", 3);
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
    CSVParser p(TEST_CSV_INPUT_DIR + "2018-07-05.csv", 3);
    
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
    /* Test for CSV value searc with one condition
        - Found index of the search variables should match 
    */  
    CSVParser p(TEST_CSV_INPUT_DIR + "2018-07-05.csv", 3);
    
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
    /* Test for CSV row search with only name
        - Found index of the search variables should match 
    */  
    CSVParser p(TEST_CSV_INPUT_DIR + "min200611.csv", 0);
    Loc date{
        .name="date",
        .row=1,
        .column=0
    };
    auto date_value{p.value(date)};
    ASSERT_EQ(date_value, "11/06/20");
}

TEST(CSVParser, SliceSelection){
    /* Test for CSV  row slice
        - Found index of the search variables should match 
    */  
    CSVParser p(TEST_CSV_INPUT_DIR + "min200611.csv", 0);
    
    auto columns{p.values("row", 0,0,20)};
    ASSERT_EQ(columns.size(), 20);

    auto date_value{p.values("column", 0,1,3)};

    for(auto i:date_value){
        ASSERT_EQ("11/06/20", i);
    }
}

TEST(CSVParser, ParseData){
    /* Test for CSV Parser pipeline
        - Full pipeline for parser
    */  
   CSVParser p(TEST_CSV_INPUT_DIR + "min200611.csv", 0);
    Table table;
    auto date{p.values("column", 0,1,-1)};
    auto time{p.values("column", 1,1,-1)};
    

    Loc  pac{
        .name="Pac",
        .orient="row",
        .row=0
    };
    
    Loc pdc{
        .name="Pdc",
        .orient="row",
        .row=0
    };

    auto pac_cols{p.values("row",pac,1)};
    auto pdc_cols{p.values("row",pdc,1)}; 
 
    table.insert(pac_cols);
    table.insert(pdc_cols); 
    table.insert("date",date);
    table.insert("time",time);
    table.save("inverter.csv");

}


int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}