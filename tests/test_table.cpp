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

    Loc targets3{
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
    table.insert("date", "2010");
    table.save("test.csv");

}



int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}