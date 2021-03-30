#include "test_header.hpp"

class XMLParserTest : public TestTimer
{
};


TEST(XMLParser, TestConstructor)
{
    XMLParser p(TEST_CSV_INPUT_DIR + "2018-07-05.csv");
    ASSERT_EQ(p.file_name, "2018-07-05");
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}