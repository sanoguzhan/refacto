#include "test_header.hpp"

class CSVParserTest : public TestTimer
{
};

/* Test for CSVParser
    - Test for openning file
    - Test for reading file and 2-d vector construction
    _ Test for CSVParser values methods
*/



// TEST(CustomParser, Constructor)
// {
//     /* Test for CSV Parser erase diverge function
//         - Load the file
//         - check size of loaded data
//         - clear diverged data 
//         - check if the cleaned has been performed
//     */
//     // CSVParser p(TEST_CSV_INPUT_DIR + "Maulevrier.csv", ",");


//     Entity pac{
//         .key = "inverter",
//         .name = "Pac",
//         .orient = "row",
//         .type = "series"};

//     Entity pdc{
//         .key = "inverter",
//         .name = "Pdc",
//         .orient = "row",
//         .type = "series"};
//     Entity pac1{
//         .key = "inverter_mppt",
//         .name = "Pac",
//         .orient = "row",
//         .type = "series"};

//     Entity pdc2{
//         .key = "inverter_mppt",
//         .name = "Pdc",
//         .orient = "row",
//         .type = "series"};
//     CustomParser p(pac, pdc,pac1, pdc2);
//     // p(TEST_CSV_INPUT_DIR , ",",  0);

//     // ASSERT_EQ(20558, p.data.size());
//     // p.erase_diverge_row();
//     // ASSERT_EQ(20557, p.data.size());



// }


// TEST(CustomParser, TestInputSize)
// {

//     vector<map<string, string>> ent;
//     ent.push_back({{"name", "^2001.*8$"},
//                     {"orient", "row"},
//                     {"row", "0"}});

//     Entity pac{
//         .key = "inverter",
//         .name = "Pac",
//         .orient = "row",
//         .type = "series",
//         .row = 1,
//         .value_start=3};

    
//     pac.conditions = ent;
//     CustomParser p(pac, pac,pac, pac);

//     // CustomParser p(pac);
 
//     p(TEST_CSV_INPUT_DIR + "test_one_input/" , ";",  3 );


//     for(auto t:p.tables){
//         if(t.first == "inverter"){
//             ASSERT_EQ(75,t.second->get_size());
            
//         }
//     }
// }


// TEST(CustomParser, TestIdConstructor){

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
//             ASSERT_EQ(288, t.second->get_size());
//         }
//     }
// }


TEST(CustomParser, TestTreatedInput)
{

    // Entity ids{
    //     .key = "inverter",
    //     .name = "INV",
    //     .orient = "row",
    //     .type = "ids",
    //     .row = 0,
    //     .column =1,
    //     .value_begin=1,
    //     .keyword="multi"};

    Entity ids{
        .key = "inverter",
        .name = "INV",
        .type = "ids",
        .keyword="file_name"};

    Entity pac_watt{
        .key = "inverter",
        .name = "Pac_watt",
        .orient= "row",
        .type="series",
        .row=0,
        .value_begin=1
    };
     Entity time{
        .key = "inverter",
        .name = "time",
        .orient= "row",
        .type="series",
        .row=0,
        .value_begin=1
    }; 
     Entity voltage{
        .key = "inverter",
        .name = "Phase1_voltage_volt",
        .orient= "row",
        .type="series",
        .row=0,
        .value_begin=1
    }; 
    CustomParser p(ids,pac_watt,time,voltage);
    p("tests/test_data/csv/", ";",  0);
    p.to_csv("."); 

    for(auto t:p.tables){
        if(t.first == "inverter"){
            ASSERT_EQ(45756, t.second->get_size());
        }
    }
}



int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}