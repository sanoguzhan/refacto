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

TEST(CSVParserCases, SolarMax__Inverter)
{
    CSVParser p(TEST_CSV_INPUT_DIR + "SolarMax.csv", ",", 4);
    p.erase_data("column", 1, 4);
    p.erase_data("column", 6, 8);
    p.to_csv(TEST_CSV_OUTPUT_DIR + "SolarMax__Inverter.csv");
    ASSERT_EQ(readFile(TEST_CSV_EXPECTED_DIR + "SolarMax__Inverter.csv"), readFile(TEST_CSV_OUTPUT_DIR + "SolarMax__Inverter.csv"));
}

TEST(CSVParserCases, SolarMax__Mppt)
{
    CSVParser p(TEST_CSV_INPUT_DIR + "SolarMax.csv", ",", 4);
    p.erase_data("column", 1, 7);
    p.erase_data("column", 8, 9);
    p.to_csv(TEST_CSV_OUTPUT_DIR + "SolarMax__Mppt.csv");
    ASSERT_EQ(readFile(TEST_CSV_EXPECTED_DIR + "SolarMax__Mppt.csv"), readFile(TEST_CSV_OUTPUT_DIR + "SolarMax__Mppt.csv"));
}

TEST(CSVParserCases, RCD_Inverter)
{
    CSVParser p(TEST_CSV_INPUT_DIR + "RCD_int_kwr_180223.txt", ";", 4);
    p.erase_data("row", 1, 3);
    p.erase_pattern("row", "Info;Time");
    p.to_csv(TEST_CSV_OUTPUT_DIR + "RCD_int_kwr_180223_inverter.csv");
    ASSERT_EQ(readFile(TEST_CSV_EXPECTED_DIR + "RCD_int_kwr_180223_inverter.csv"), readFile(TEST_CSV_OUTPUT_DIR + "RCD_int_kwr_180223_inverter.csv"));
}

TEST(CSVParserCases, Meteocontrol_Inverter)
{

    CSVParser p(TEST_CSV_INPUT_DIR + "Meteocontrol.csv", ",", 1);
    Table table;
    auto datetime{p.values("column", 0, 2, -1)};

    Loc id{
        .name = "Id",
        .orient = "row",
        .row = 0};

    Loc ac{
        .name = "U_AC",
        .orient = "row",
        .row = 1};

    Loc udc{
        .name = "U_DC",
        .orient = "row",
        .row = 1};

    auto ac_cols{p.values("row", 2, id, ac)};
    auto udc_cols{p.values("row", 2, id, udc)};

    table.insert(ac_cols);
    table.insert(udc_cols);
    table.insert("datetime", datetime);
    table.save(TEST_CSV_OUTPUT_DIR + "Meteocontrol__inverter.csv");
    ASSERT_EQ(readFile(TEST_CSV_OUTPUT_DIR + "Meteocontrol__inverter.csv"), readFile(TEST_CSV_EXPECTED_DIR + "Meteocontrol__inverter.csv"));
}

TEST(CSVParserCases, Maulevrier_Inverter)
{

    CSVParser p(TEST_CSV_INPUT_DIR + "Maulevrier.csv", ",", 2);
    Table table;

    // p.erase_data("row", 1, 5);
    // p.erase_data("row", 2, 3);
    // p.erase_diverge_row();
    p.to_csv(TEST_CSV_OUTPUT_DIR + "test.csv");
    auto datetime{p.values("column", 0, 2, -1)};

    Loc id{
        .name = "-",
        .orient = "row",
        .row = 0};

    Loc Pdc{
        .name = "Gen.",
        .orient = "row",
        .row = 1};

    Loc Pac{
        .name = "Generated",
        .orient = "row",
        .row = 1};

    auto pac_cold{p.values("row", 2, id, Pac)};
    auto pdc_cols{p.values("row", 2, id, Pdc)};

    table.insert(pac_cold);
    table.insert(pdc_cols);
    table.insert("datetime", datetime);
    table.save(TEST_CSV_OUTPUT_DIR + "Maulevrier__inverter.csv");
    // ASSERT_EQ(readFile(TEST_CSV_OUTPUT_DIR + "Maulevrier__inverter.csv"), readFile(TEST_CSV_EXPECTED_DIR + "Maulevrier__inverter.csv"));
}

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}