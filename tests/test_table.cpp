#include "test_header.hpp"

class DtableTests: public TestTimer{
};

TEST(TableTests, TableConstructor){

/* Same element should not be inserted twice
   Same column of an id should not be inserted twice
*/  


    Table table = Table();
    

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
    table.insert(cols);
    table.insert(cols2);
    for(auto& p:table.data){
        for(auto r:p.second){
            ASSERT_EQ(75, r.second.size());
        }
    }

}

TEST(TableTests, TableSave){

/* Same element should not be inserted twice
   Same column of an id should not be inserted twice
*/  


    Table table = Table();
    

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
    table.insert(cols);
    table.insert(cols2);
    table.insert("date",p.file_name);
    table.save("test.csv");
    for(const auto& p:table.data){
        ASSERT_EQ(p.second.size(),3);
    }

}

TEST(TableTests, TableSaveRowWise){

/* Same element should not be inserted twice
   Same column of an id should not be inserted twice
*/  


    Table table = Table();
    

    CSVParser p("tests/test_data/csv/min200611.csv", 0);
    
    
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
    table.insert("date",p.file_name);
    table.save("test2.csv");
    for(const auto& p:table.data){
        ASSERT_EQ(p.second.size(),3);
    }

}


int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}