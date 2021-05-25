#include "test_header.hpp"

class XMLParserTest: public TestTimer{

};


/* Test for XMLParser
    - Test for openning file
    - Test for reading file and 2-d vector construction
    _ Test for XML values methods
*/

// TEST(XMLParser, IncludeDir){
//     /* Test for XML value extraction 
//     - Arbitary number of IDMap input
//     - Classified granularity based files
//     */
//     IDMap inv_pac{
//         .name="inverter",
//         .node="Key",
//         .key="Pac",
//         .degree="Mean",
//     };

//     IDMap inv_id{
//         .name="inverter",
//         .node="Key",
//         .key="Seri",
//         .degree="Mean",
//     };

//     IDMap mppt_amp{
//         .name="inverter_mppt",
//         .node="Key",
//         .key="Amp",
//         .degree="Mean",
//     };

//     IDMap mppt_vol{
//         .name="inverter_mppt",
//         .node="Key",
//         .key="Vol",
//         .degree="Mean",
//     };

//    IDMap mppt_etotal{
//         .name="inverter_mppt",
//         .node="Key",
//         .key="E-total",
//         .degree="Mean",
//     };


//     XMLParser parser(inv_pac,inv_id, mppt_amp, mppt_etotal, mppt_vol);
  
//     parser("tests/test_data/xml/", "WebBox");

//     parser.to_csv("tests/test_data/xml/");
//     ASSERT_TRUE(fs::exists("tests/test_data/xml/inverter.csv"));
//     ASSERT_TRUE(fs::exists("tests/test_data/xml/inverter_mppt.csv"));

// }




// TEST(XMLParser, TestGlobVector){
//     /* Test for file search with pattern
//     */
//     auto files{listdir("tests/test_data/xml/")};
//     EXPECT_EQ(3, files.size());
  
// }

TEST(XMLParser, RegexSearch){
    IDMap inv_pac{
        .name="string",
        .node="Key",
        .key="^(.*?).Ms.Amp",
        .degree="Mean",
    };

    IDMap inv_pac2{
        .name="string",
        .node="Key",
        .key="^(.*?).Ms.Amp",
        .degree="Mean",
    };
    XMLParser parser(inv_pac,inv_pac2);
  
    parser("tests/test_data/xml/multi-variables/sciheco/", "WebBox");

    parser.to_csv("tests/test_data/xml/");
  
}


int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}