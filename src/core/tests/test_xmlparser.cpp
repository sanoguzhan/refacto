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
    XMLParser parser(inv_pac, inv_id, mppt_amp);

    parser("tests/test_data/xml/Mean.20190515_073036.xml", "WebBox");
    // parser("tests/test_data/xml/Mean.20190515_091539.xml", "WebBox");

    parser.to_csv();
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