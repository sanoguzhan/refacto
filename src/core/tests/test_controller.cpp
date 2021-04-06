#include "test_header.hpp"

class ControllerTest : public TestTimer
{
};



TEST(Controller, TestConstructor)
{
    dict kwargs ={{"csvparser",{{"path", "tests/test_data/csv/input/min200611.csv"}, {"delim", ";"}, {"skip","0"}}}};
    CSVParserWrapper c(kwargs);
     Loc pac{
        .name = "Pac",
        .orient = "row",
        .row = 0};
    Loc pdc{
        .name = "Pdc",
        .orient = "row",
        .row = 0};
    c.from_csv0("row", pac, 1);
    c.from_csv0("row", pdc, 1);
    c.to_csv("test.csv");
    ASSERT_EQ(c.csvparser->file_name, "min200611");
}

TEST(XMLParserController, TestInterfaceController){
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

   IDMap mppt_etotal{
        .name="inverter_mppt",
        .node="Key",
        .key="E-total",
        .degree="Mean",
    };


    XMLParser parser(inv_pac,inv_id, mppt_amp, mppt_etotal, mppt_vol);
  
    parser("tests/test_data/xml/*.xml", "WebBox");

    parser.to_csv("tests/test_data/xml/");
    ASSERT_TRUE(fs::exists("tests/test_data/xml/inverter.csv"));
    ASSERT_TRUE(fs::exists("tests/test_data/xml/inverter_mppt.csv"));
  
}
int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
