#include "test_header.hpp"


class CSVParser: public TestTimer{
};

TEST(CSVParser, ReadFile){

    read_file("tests/test_data/2018-07-05.csv");

}



int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();    

}