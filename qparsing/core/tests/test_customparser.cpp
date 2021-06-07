#include "test_header.hpp"

class CSVParserTest : public TestTimer {};

/* Test for CustomParser
    - Test for openning file
    - Test for reading file and 2-d vector construction
    - Test for CSVParser values methods
    - Test for Entity Types
*/

// TEST(CustomParser, Constructor)
// {
//     /* Test for CustomParser Multi key table generation
//         - Load the file
//         - Create multi Tables ( one per key)
//         - Insert multi ids to each table
//         - Parse data from Series to tables
//         - Parse data from Entity vector to tables
//         - Parse data from Entity (single value) to tables
//     */

//    // !Example of multi ID mapping (insertion)
//    // we are iteration as row-wise and pick up each value where value begin
//     Entity inverter_ids{
//         .key = "inverter",
//         .name = "INV",
//         .orient = "row",
//         .type = "ids",
//         .row = 0,
//         .column =0,
//         .value_begin=1,
//         .keyword="multi"};

//     // For test purposes we pick up the same values for inverter_mmpt
//     // because tables can not created without ids
//     Entity mppt_ids{
//         .key = "inverter_mppt",
//         .name = "INV",
//         .orient = "row",
//         .type = "ids",
//         .row = 0,
//         .column =0,
//         .value_begin=1,
//         .keyword="multi"};

//     // !Example Series insertion
//     // Here we pick up Pac values and insert to inverter table
//     // name is a regex
//     Entity pac{
//         .key = "inverter",
//         .name = "Pac",
//         .orient = "row",
//         .type = "series",
//         .row=0,
//         .value_begin=1};

//     Entity pdc{
//         .key = "inverter",
//         .name = "Pdc1",
//         .orient = "row",
//         .type = "series",
//          .row=0,
//         .value_begin=1};

//     Entity pac1{
//         .key = "inverter_mppt",
//         .name = "Pac",
//         .orient = "row",
//         .type = "series",
//          .row=0,
//         .value_begin=1};

//     Entity pdc2{
//         .key = "inverter_mppt",
//         .name = "Pdc1",
//         .orient = "row",
//         .type = "series",
//          .row=0,
//         .value_begin=1};

//     // !Example vector value insertion
//     // here keyword is required for ambigous Constructor
//     // it picks up date column and insert to each id under key group
//     Entity date{
//         .key = "inverter_mppt",
//         .name = "#Date",
//         .orient = "column",
//         .type = "vector",
//         .keyword="vector",
//         .column=0,
//         .from=1,
//         .to=-1
//         };
//     Entity time{
//         .key = "inverter_mppt",
//         .name = "Time",
//         .orient = "column",
//         .type = "vector",
//         .keyword="vector",
//         .column=1,
//         .from=1,
//         .to=-1
//         };

//     // !Example single value insertion
//     // !if keyword is file_name then opened filename will be inserted as
//     single value
//     // if not keyword value will be inserted
//     Entity single{
//         .key = "inverter_mppt",
//         .name = "Test_single_Value",
//         .type = "entity",
//         .keyword="test",
//         };
//     CustomParser p(inverter_ids,pac, pdc, mppt_ids, pac1,date,time,single);
//     p(TEST_CSV_INPUT_DIR + "test_id_input/" , ";",  0);
//     for(const auto& table:p.tables){
//         if(table.first == "inverter" || table.first == "inverter_mppt"){
//             continue;
//         }else{
//             FAIL();
//         }
//     }
// }

// TEST(CustomParser, TestInputSize){

//     /* Test for CustomParser Conditional series insertion
//         - Load the file
//         - Create table
//         - Parse data from conditional series to table
//     */

//    // !Example of conditional series parsing
//    // ids as conditonal, and takes a regex
//     vector<map<string, string>> ent;
//     vector<map<string, string>> ent1;
//     ent.push_back({{"name", "^2001.*98$"},
//                     {"orient", "row"},
//                     {"row", "0"}});

//     Entity pac{
//         .key = "inverter",
//         .name = "Pac",
//         .orient = "row",
//         .type = "series",
//         .row = 1,
//         .value_start=3};

//     ent1.push_back({{"name", "^2001.*98$"},
//                     {"orient", "row"},
//                     {"row", "0"}});

//     Entity pac1{
//         .key = "inverter",
//         .name = "Riso",
//         .orient = "row",
//         .type = "series",
//         .row = 1,
//         .value_start=3};

//     Entity single{
//         .key = "inverter",
//         .name = "Test_single_Value",
//         .type = "entity",
//         .keyword="test",
//         };
//     pac.conditions = ent;
//     pac1.conditions = ent1;
//     CustomParser p(pac, single);
//     // insert single values, and other entities
//     p(TEST_CSV_INPUT_DIR + "test_one_input/" , ";",  3 );

//     for(auto t:p.tables){
//         if(t.first == "inverter"){
//             ASSERT_EQ(75,t.second->max_size());
//         }
//     }
//     // p.to_csv(".");
// }

// TEST(CustomParser, TestContinousSize){

//     /* Test for CustomParser Conditional series insertion
//         - Load the file
//         - Create table
//         - Parse data from conditional series to table
//         - Insert to exisint Series
//     */

//    // !Example of conditional series parsing
//    // ids as conditonal, and takes a regex
//     vector<map<string, string>> ent;
//     vector<map<string, string>> ent1;
//     ent.push_back({{"name", "^2001.*98$"},
//                     {"orient", "row"},
//                     {"row", "0"}});

//     Entity pac{
//         .key = "inverter",
//         .name = "Pac",
//         .orient = "row",
//         .type = "series",
//         .row = 1,
//         .value_start=3};

//     ent1.push_back({{"name", "^2001.*98$"},
//                     {"orient", "row"},
//                     {"row", "0"}});

//     Entity pac1{
//         .key = "inverter",
//         .name = "Riso",
//         .orient = "row",
//         .type = "series",
//         .row = 1,
//         .value_start=3};

//     Entity single{
//         .key = "inverter",
//         .name = "Test_single_Value",
//         .type = "entity",
//         .keyword="test",
//         };
//     pac.conditions = ent;
//     pac1.conditions = ent1;
//     CustomParser p(pac, single);
//     // insert single values, and other entities
//     p(TEST_CSV_INPUT_DIR + "test_more_input/" , ";",  3 );

//     for(auto t:p.tables){
//         if(t.first == "inverter"){
//             ASSERT_EQ(431,t.second->max_size());
//         }
//     }
//     // p.to_csv(".");
// }

// TEST(CustomParser, TestIdConstructor){
//     /* Test for CustomParser id insertion with variable
//         - Load the file
//         - Create table
//         - Create ids
//         - Parse data from conditional series to table
//     */

//     Entity ids{
//         .key = "inverter",
//         .name = "INV",
//         .orient = "row",
//         .type = "ids",
//         .row = 0,
//         .column =1,
//         .value_begin=1,
//         .keyword="multi"};

//     Entity pac{
//         .key = "inverter",
//         .name = "Pac",
//         .orient = "row",
//         .type = "series",
//         .row = 0,
//         .value_begin=1};

//     CustomParser p(ids, pac);
//     p(TEST_CSV_INPUT_DIR + "test_id_input/" , ";",  0);

//     for(auto t:p.tables){
//         if(t.first == "inverter"){
//             ASSERT_EQ(288, t.second->max_size());
//         }
//     }
// }

// TEST(CustomParser, TestTreatedInput){
//     /* Test for CustomParser id insertion with multiple variable
//         - Load the file
//         - Create table
//         - Create ids
//         - Parse data from conditional series to table
//     */
//     Entity ids{
//         .key = "inverter",
//         .name = "INV",
//         .type = "ids",
//         .keyword="file_name"};

//     Entity pac_watt{
//         .key = "inverter",
//         .name = "Pac_watt",
//         .orient= "row",
//         .type="series",
//         .row=0,
//         .value_begin=1
//     };
//      Entity time{
//         .key = "inverter",
//         .name = "time",
//         .orient= "row",
//         .type="series",
//         .row=0,
//         .value_begin=1
//     };

//      Entity voltage{
//         .key = "inverter",
//         .name = "Phase1_voltage_volt",
//         .orient= "row",
//         .type="series",
//         .row=0,
//         .value_begin=1
//     };
//     CustomParser p(ids,pac_watt,time,voltage);
//     p("tests/test_data/csv/", ";",  0);

//     for(auto t:p.tables){
//         if(t.first == "inverter"){
//             ASSERT_EQ(45756, t.second->max_size());
//         }
//     }

// }

// TEST(CustomParser, TestGroupId){
//     /* Test for CustomParser group id parsing
//         - Load the file
//         - Create table
//         - group ids with regex
//         - Create column name with regex
//         - Parse data from conditional series to table
//     */
//     vector<map<string, string>> ent;
//     ent.push_back({{"id", "(.*)_mppt_power_watt"},
//                     {"name", ".*(_mppt_power_watt)"}
//                     });

//     Entity power{
//         .key = "inverter",
//         .name = "(.*)(_mppt_power_watt)",
//         .orient ="row",
//         .type = "group",
//          .row=0,
//         .value_begin=1};
//     power.conditions = ent;

//     vector<map<string, string>> ent1;
//     ent1.push_back({{"id", "(.*)_string_current_amper"},
//                     {"name", ".*(_string_current_amper)"}
//                     });

//     Entity current{
//         .key = "string",
//         .name = "",
//         .orient ="row",
//         .type = "group",
//          .row=0,
//         .value_begin=1};
//     current.conditions = ent1;

//     CustomParser p(power, current);
//     p("tests/test_data/csv/input/test_id_group/", ";",  0);

//     for(auto t:p.tables){
//         if(t.first == "inverter"){
//             ASSERT_EQ(91166, t.second->max_size());
//         }else if(t.first == "string"){
//             ASSERT_EQ(91166, t.second->max_size());
//         }
//     }
//     //    p.to_csv(".");
// }

// TEST(CustomParser, TestHelperMethod){

//     /* Test for CustomParser Helper Methods
//     */
//     ASSERT_EQ(false, is_path_exist("foo/bar"));

// }

TEST(CustomParser, test) {
    /* Test for CustomParser group id parsing
        - Load the file
        - Create table
        - group ids with regex
        - Create column name with regex
        - Parse data from conditional series to table
    */
    vector<map<string, string>> ent;
    ent.push_back({{"id", ".*Channel_(\\d+)_I_AvgMn.*"},
                   {"name", ".*Channel_\\d+_(I_AvgMn).*"}});

    Entity first{.key = "string",
                 .name = "I_AvgMn",
                 .orient = "row",
                 .type = "group",
                 .row = 0,
                 .value_begin = 1};
    first.conditions = ent;

    Entity second{.key = "string",
                  .name = "dcbox",
                  .type = "entity",
                  .keyword = "file_name"};

    Entity date_time{.key = "string",
                     .name = "date_time",
                     .orient = "column",
                     .type = "vector",
                     .keyword = "vector",
                     .column = 0,
                     .from = 1,
                     .to = -1};

    Entity second2{.key = "inverter",
                   .name = "Inverter",
                   .type = "ids",
                   .keyword = "file_name"};

    Entity first2{.key = "inverter",
                  .name = ".*I_DC_AvgMn.*",
                  .orient = "row",
                  .type = "series",
                  .row = 0,
                  .value_begin = 1};

    Entity first3{.key = "inverter",
                  .name = ".*U_DC_AvgMn.*",
                  .orient = "row",
                  .type = "series",
                  .row = 0,
                  .value_begin = 1};

    Entity date_time2{.key = "inverter",
                      .name = "date_time",
                      .orient = "column",
                      .type = "vector",
                      .keyword = "vector",
                      .column = 0,
                      .from = 1,
                      .to = -1};

    Entity second3{.key = "inverter",
                   .name = "string",
                   .type = "entity",
                   .keyword = "file_name"};
    // CustomParser p(first, second, date_time, second3);
    CustomParser p(second2, first2, first3, date_time2, second3);
    // p("tests/test_data/test01/dcbox/", ";",  0);
    p("tests/test_data/test02/inverter/", ";", 0);
    // for(auto t:p.tables){
    //     if(t.first == "inverter"){
    //         ASSERT_EQ(91166, t.second->get_size());
    //     }else if(t.first == "string"){
    //         ASSERT_EQ(91166, t.second->get_size());
    //     }
    // }
    p.to_csv(".");
}

int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}