#include "test_header.hpp"

class XMLParserTest : public TestTimer {};

/* Test for XMLParser
    - Test for openning file
    - Test for reading file and 2-d vector construction
    _ Test for XML values methods
*/

TEST(XMLParser, IncludeDir) {
    /* Test for XML value extraction
    - Arbitary number of IDMap input
    - Classified granularity based files
    */
    IDMap inv_pac{
        .name = "inverter",
        .node = "Key",
        .key = "Pac",
        .degree = "Mean",
    };

    IDMap inv_id{.name = "inverter",
                 .node = "Key",
                 .key = "Seri",
                 .degree = "Mean",
                 .type = "single",
                 .output = "test_name"};

    IDMap mppt_amp{
        .name = "inverter_mppt",
        .node = "Key",
        .key = "Amp",
        .degree = "Mean",
    };

    IDMap mppt_vol{
        .name = "inverter_mppt",
        .node = "Key",
        .key = "Vol",
        .degree = "Mean",
    };

    IDMap mppt_etotal{
        .name = "inverter_mppt",
        .node = "Key",
        .key = "E-total",
        .degree = "Mean",
    };

    XMLParser parser(inv_pac, inv_id, mppt_amp, mppt_etotal, mppt_vol);

    parser("data/xml/single_variables/", "WebBox");

    parser.to_csv("data/xml/");
    ASSERT_TRUE(fs::exists("data/xml/inverter.csv"));
    ASSERT_TRUE(fs::exists("data/xml/inverter_mppt.csv"));
}

TEST(XMLParser, TestGlobVector) {
    /* Test for file search with pattern
     */
    auto files{listdir("data/xml/single_variables/")};
    EXPECT_EQ(3, files.size());
}

TEST(XMLParser, RegexSearch) {
    IDMap amp{.name = "string",
              .node = "Key",
              .key = "^(.*?).Ms.Amp",
              .degree = "Mean",
              .type = "multi"};

    //

    IDMap tmp{
        .name = "string",
        .node = "Key",
        .key = "WRTP4Q44:2110070939:Inv.TmpLimStt",
        .degree = "Mean",
    };

    // Single should be always the last one
    XMLParser parser(amp, tmp);

    parser("data/xml/multi-variables/sciheco/", "WebBox");

    ASSERT_EQ(2, parser.data.size());

    // parser.to_csv(".");
}

TEST(XMLParser, GroupElements) {
    IDMap amp{.name = "string",
              .node = "Key",
              .key = "^(.*?).Ms.Amp",
              .degree = "Mean",
              .type = "multi"};

    IDMap tmp{.name = "string",
              .node = "Key",
              .key = "^(.*?).TmpLimStt",
              .degree = "Mean",
              .type = "group"};

    IDMap stt{.name = "string",
              .node = "Key",
              .key = "^(.*?)InvCtl.Stt",
              .degree = "Mean",
              .type = "group"};
    IDMap tmp2{
        .name = "string",
        .node = "Key",
        .key = "WRTP4Q44:2110070939:Inv.TmpLimStt",
        .degree = "Mean",
    };
    map<string, string> ent1{{"id", "(.*)InvCtl.Stt"},
                             {"name", ".*(InvCtl.Stt)"}};

    map<string, string> ent{{"id", "(.*).TmpLimStt"},
                            {"name", ".*(.TmpLimStt)"}};
    tmp.conditions = ent;
    stt.conditions = ent1;

    XMLParser parser(tmp, stt, tmp2);

    parser("data/xml/multi-variables/sciheco/", "WebBox");
    // parser.to_csv(".");

    ASSERT_EQ(3, parser.data.size());

}

TEST(XMLParser, WrongType) {
    IDMap amp{.name = "string",
              .node = "Key",
              .key = "^(.*?).Ms.Amp",
              .degree = "Mean",
              .type = "test"};

    XMLParser parser(amp);
    try {
        parser("data/xml/multi-variables/sciheco/", "WebBox");

    } catch (std::exception& ex) {
        EXPECT_EQ("Wrong Type \n Defined Types: 'single', 'multi', 'group' ",
                  (std::string)ex.what());
    }
}

TEST(XMLParser, WrongPath) {
    IDMap amp{.name = "string",
              .node = "Key",
              .key = "^(.*?).Ms.Amp",
              .degree = "Mean",
              .type = "test"};

    XMLParser parser(amp);
    try {
        parser("test", "WebBox");

    } catch (std::exception& ex) {
        EXPECT_EQ("Files Not Found at test/",
                  (std::string)ex.what());
    }
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    // return RUN_ALL_TESTS();
}