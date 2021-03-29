#include "test_header.hpp"

class ControllerTest : public TestTimer
{
};



TEST(Controller, TestConstructor)
{
    dict kwargs ={{"csvparser",{{"path", "tests/test_data/csv/input/min200611.csv"}, {"delim", ";"}, {"skip","0"}}}};
    CSVParserWrapper c(kwargs);
     Loc pac{
        .name = "Pac",
        .orient = "row",
        .row = 0};
    Loc pdc{
        .name = "Pdc",
        .orient = "row",
        .row = 0};
    c.from_csv("row", pac, 1);
    c.from_csv("row", pdc, 1);
    c.to_csv("test.csv");
    ASSERT_EQ(c.csvparser->file_name, "min200611");
}


int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
