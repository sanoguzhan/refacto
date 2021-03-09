#include "test_header.hpp"

using namespace std;

class CSVParserTest : public TestTimer
{
};

/* Test for CSVParser
    - Test for csv file variations
    - Cross-Validation for multiple cases
*/

TEST(CSVParserCases, PlantName1)
{

    CSVParser p(TEST_CSV_INPUT_DIR + "min200611.csv", 0);
    Table table;
    auto date{p.values("column", 0, 1, -1)};
    auto time{p.values("column", 1, 1, -1)};

    Loc pac{
        .name = "Pac",
        .orient = "row",
        .row = 0};

    Loc pdc{
        .name = "Pdc",
        .orient = "row",
        .row = 0};

    auto pac_cols{p.values("row", pac, 1)};
    auto pdc_cols{p.values("row", pdc, 1)};

    table.insert(pac_cols);
    table.insert(pdc_cols);
    table.insert("date", date);
    table.insert("time", time);
    table.save(TEST_CSV_OUTPUT_DIR + "PlantName1_inverter.csv");
    ASSERT_EQ(readFile(TEST_CSV_OUTPUT_DIR + "PlantName1_inverter.csv"), readFile(TEST_CSV_EXPECTED_DIR + "PlantName1.csv"));
}

TEST(CSVParserCases, Solarlog_Kaco__Inverter)
{

    CSVParser p(TEST_CSV_INPUT_DIR + "Solarlog_Kaco.csv", 0);
    Table table;
    auto date{p.values("column", 0, 1, -1)};
    auto time{p.values("column", 1, 1, -1)};

    Loc pac{
        .name = "Pac",
        .orient = "row",
        .row = 0};

    Loc pdc{
        .name = "Pdc",
        .orient = "row",
        .row = 0};

    auto pac_cols{p.values("row", pac, 1)};
    auto pdc_cols{p.values("row", pdc, 1)};

    table.insert(pac_cols);
    table.insert(pdc_cols);
    table.insert("date", date);
    table.insert("time", time);
    table.save(TEST_CSV_OUTPUT_DIR + "Solarlog_Kaco__Inverter.csv");
    ASSERT_EQ(readFile(TEST_CSV_OUTPUT_DIR + "Solarlog_Kaco__Inverter.csv"), readFile(TEST_CSV_EXPECTED_DIR + "Solarlog_Kaco__Inverter.csv"));
}

TEST(CSVParserCases, Solarlog_Kaco__Mppt)
{

    CSVParser p(TEST_CSV_INPUT_DIR + "Solarlog_Kaco.csv", 0);
    Table table;
    auto date{p.values("column", 0, 1, -1)};
    auto time{p.values("column", 1, 1, -1)};

    Loc pac{
        .name = "Pac",
        .orient = "row",
        .row = 0};

    Loc pdc{
        .name = "Pdc",
        .orient = "row",
        .row = 0};

    Loc udc{
        .name = "Udc",
        .orient = "row",
        .row = 0};

    Loc idc{
        .name = "Idc",
        .orient = "row",
        .row = 0};

    auto pac_cols{p.values("row", pac, 1)};
    auto pdc_cols{p.values("row", pdc, 1)};
    auto udc_cols{p.values("row", udc, 1)};
    auto idc_cols{p.values("row", idc, 1)};

    table.insert(pac_cols);
    table.insert(pdc_cols);
    table.insert(udc_cols);
    table.insert(idc_cols);
    table.insert("date", date);
    table.insert("time", time);
    table.save(TEST_CSV_OUTPUT_DIR + "Solarlog_Kaco__Mppt.csv");
    ASSERT_EQ(readFile(TEST_CSV_OUTPUT_DIR + "Solarlog_Kaco__Mppt.csv"), readFile(TEST_CSV_EXPECTED_DIR + "Solarlog_Kaco__Mppt.csv"));
}

TEST(CSVParserCases, Solarlog_SMA__Inverter)
{

    CSVParser p(TEST_CSV_INPUT_DIR + "Solarlog_SMA.csv", 0);
    Table table;
    auto date{p.values("column", 0, 1, -1)};
    auto time{p.values("column", 1, 1, -1)};

    Loc pac{
        .name = "Pac",
        .orient = "row",
        .row = 0};

    Loc pdc{
        .name = "Pdc",
        .orient = "row",
        .row = 0};

    auto pac_cols{p.values("row", pac, 1)};
    auto pdc_cols{p.values("row", pdc, 1)};

    table.insert(pac_cols);
    table.insert(pdc_cols);
    table.insert("date", date);
    table.insert("time", time);
    table.save(TEST_CSV_OUTPUT_DIR + "Solarlog_SMA__Inverter.csv");
    ASSERT_EQ(readFile(TEST_CSV_OUTPUT_DIR + "Solarlog_SMA__Inverter.csv"), readFile(TEST_CSV_EXPECTED_DIR + "Solarlog_SMA__Inverter.csv"));
}

TEST(CSVParserCases, Solarlog_SMA__Mppt)
{

    CSVParser p(TEST_CSV_INPUT_DIR + "Solarlog_SMA.csv", 0);
    Table table;
    auto date{p.values("column", 0, 1, -1)};
    auto time{p.values("column", 1, 1, -1)};

    Loc pac{
        .name = "Pac",
        .orient = "row",
        .row = 0};

    Loc pdc{
        .name = "Pdc",
        .orient = "row",
        .row = 0};

    Loc udc{
        .name = "Udc",
        .orient = "row",
        .row = 0};

    Loc idc{
        .name = "Idc",
        .orient = "row",
        .row = 0};

    auto pac_cols{p.values("row", pac, 1)};
    auto pdc_cols{p.values("row", pdc, 1)};
    auto udc_cols{p.values("row", udc, 1)};
    // Empty IDC must be Nan Values
    auto idc_cols{p.values("row", idc, 1)};

    table.insert(pac_cols);
    table.insert(pdc_cols);
    table.insert(udc_cols);
    table.insert(idc_cols);
    table.insert("date", date);
    table.insert("time", time);
    table.save(TEST_CSV_OUTPUT_DIR + "Solarlog_SMA__Mppt.csv");
    ASSERT_EQ(readFile(TEST_CSV_OUTPUT_DIR + "Solarlog_SMA__Mppt.csv"), readFile(TEST_CSV_EXPECTED_DIR + "Solarlog_SMA__Mppt.csv"));
}

TEST(CSVParserCases, Solarlog_ABB__Inverter)
{

    CSVParser p(TEST_CSV_INPUT_DIR + "Solarlog_ABB.csv", 0);
    Table table;
    auto date{p.values("column", 0, 1, -1)};
    auto time{p.values("column", 1, 1, -1)};

    Loc pac{
        .name = "Pac",
        .orient = "row",
        .row = 0};

    auto pac_cols{p.values("row", pac, 1)};

    table.insert(pac_cols);
    table.insert("date", date);
    table.insert("time", time);
    table.save(TEST_CSV_OUTPUT_DIR + "Solarlog_ABB__Inverter.csv");
    ASSERT_EQ(readFile(TEST_CSV_OUTPUT_DIR + "Solarlog_ABB__Inverter.csv"), readFile(TEST_CSV_EXPECTED_DIR + "Solarlog_ABB__Inverter.csv"));
}

TEST(CSVParserCases, Solarlog_ABB__Mppt)
{

    CSVParser p(TEST_CSV_INPUT_DIR + "Solarlog_ABB.csv", 0);
    Table table;
    auto date{p.values("column", 0, 1, -1)};
    auto time{p.values("column", 1, 1, -1)};

    Loc pdc{
        .name = "Pdc1",
        .orient = "row",
        .row = 0};

    Loc pdc2{
        .name = "Pdc2",
        .orient = "row",
        .row = 0};

    Loc udc{
        .name = "Udc1",
        .orient = "row",
        .row = 0};

    Loc udc2{
        .name = "Udc2",
        .orient = "row",
        .row = 0};

    Loc idc{
        .name = "Idc",
        .orient = "row",
        .row = 0};

    auto pdc_cols{p.values("row", pdc, 1)};
    auto pdc2_cols{p.values("row", pdc2, 1)};
    auto udc_cols{p.values("row", udc, 1)};
    auto udc2_cols{p.values("row", udc2, 1)};
    // Empty IDC must be Nan Values
    auto idc_cols{p.values("row", idc, 1)};

    table.insert(pdc_cols);
    table.insert(pdc2_cols);
    table.insert(udc_cols);
    table.insert(udc2_cols);
    table.insert(idc_cols);
    table.insert("date", date);
    table.insert("time", time);
    table.save(TEST_CSV_OUTPUT_DIR + "Solarlog_ABB__Mppt.csv");
    ASSERT_EQ(readFile(TEST_CSV_OUTPUT_DIR + "Solarlog_ABB__Mppt.csv"), readFile(TEST_CSV_EXPECTED_DIR + "Solarlog_ABB__Mppt.csv"));
}

TEST(CSVParserCases, SolarMax)
{

    CSVParser p(TEST_CSV_INPUT_DIR + "SolarMax.csv", 0);
    Table table;
    auto datetime{p.values("column", 0, 1, -1)};

    // Loc  pac{
    //     .name="Pac",
    //     .orient="row",
    //     .row=0
    // };

    Loc ac{
        .name = "Puissance AC",
        .orient = "column",
        .row = 0};

    Loc dc_column{
        .name = "Puissance DC",
        .orient = "column",
        .row = 0};

    auto ac_cols{p.values("column", ac, 1)};
    // auto pdc_cols{p.values("row",pdc,1)};

    table.insert(ac_cols);
    // table.insert(pdc_cols);
    // table.insert("date",date);
    table.insert("datetime", datetime);
    table.save(TEST_CSV_OUTPUT_DIR + "SolarMax_Inverter.csv");
    // ASSERT_EQ(readFile(TEST_CSV_OUTPUT_DIR + "SolarMax_Inverter.csv"), readFile(TEST_CSV_EXPECTED_DIR + "SolarMax_Inverter.csv"));
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}