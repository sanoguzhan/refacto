#include "test_header.hpp"
class XMLParserTest: public TestTimer{
};


TEST(XMLParser, IncludeDir){
    IDMap inv_pac{
        .name="inverter",
        .node="Key",
        .key="Pac",
        .degree="Mean",
    };

    IDMap inv_id{
        .name="inverter",
        .node="Key",
        .key="Seri",
        .degree="Mean",
    };
    IDMap mppt_amp{
        .name="inverter_mppt",
        .node="Key",
        .key="Amp",
        .degree="Mean",
    };

    IDMap mppt_vol{
        .name="inverter_mppt",
        .node="Key",
        .key="Vol",
        .degree="Mean",
    };

//    IDMap mppt_etotal{
//         .name="inverter_mppt",
//         .node="Key",
//         .key="Amp",
//         .degree="E-total",
//     };


    XMLParser parser(inv_pac, inv_id, mppt_amp, mppt_vol);

    parser("tests/test_data/xml/Mean.20190102_153037.xml", "WebBox");
    parser("tests/test_data/xml/Mean.20190512_150040.xml", "WebBox");
    parser("tests/test_data/xml/Mean.20190112_123035.xml", "WebBox");
    // parser("tests/test_data/xml/Mean.20190515_091539.xml", "WebBox");

    parser.to_csv(".");
    // Table table;

    // table.insert(parser.data);



}
// TEST(XMLParser, IncludeDir){
//     IDMap inverter{
//         .name="inverter",
//         .node="Key",
//         .key="Pac",
//         .value="Mean",
//     };
//     XMLParser parser;

//     parser.read("tests/test_data/xml/Mean.20190515_073036.xml");

// }
int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}