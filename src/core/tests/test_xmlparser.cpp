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
    parser("tests/test_data/xml/Mean.20190102_153037.xml", "WebBox");
    parser("tests/test_data/xml/Mean.20190112_123035.xml", "WebBox");
    for(auto item:parser.data){
        std::cout << item.name << std::endl;
        std::cout << item.key << std::endl;
        for(auto val:item.values){
            std::cout << val << "";
        }
    }

}
// TEST(XMLParser, IncludeDir){
//     IDMap inverter{
//         .name=“inverter”,
//         .node=“Key”,
//         .key=“Pac”,
//         .value=“Mean”,
//     };
//     XMLParser parser;
//     parser.read(“tests/test_data/xml/Mean.20190515_073036.xml”);
// }
int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}