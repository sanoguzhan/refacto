#include "test_header.hpp"

class DtableTests: public TestTimer{
};

TEST(TableTests, TableConstructor){



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
    table.insert(cols);
    for(auto& p:table.data){
        std::cout << p.first << std::endl;
        for(auto r:p.second){
            std::cout<<r.first<<std::endl;
            std::cout << r.second.size() << std::endl;
        }
    }

}




int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}