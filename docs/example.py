from parsing.engine import CSVParser
from parsing.engine import XMLParser
from parsing.engine import Decompressdir, Cleandir, Compressdir

""" """

test_csv1 = CSVParser([
                {"key": "inverter",
                    "name":"Pac",
                    "orient": "row",
                    "type" : "series",
                    "row" : "1",
                    "value_begin" : "3",
                    "condition": {
                        "name" : "^2001.*98$",
                        "orient": "row",
                        "row": "0"
                        }
                 },
                {"key":"inverter",
                    "name" : "Riso",
                    "orient" : "row",
                    "type" : "series",
                    "row" :"1",
                    "value_begin" :"3"}
            ])
test_csv1("qparsing/core/tests/test_data/csv/input/test_one_input/" , ";",  3)
# test_csv1.to_csv(".")

test_csv2 = CSVParser([
                {"key": "inverter",
                    "name":"INV",
                    "orient": "row",
                    "type" : "ids",
                    "row" : "0",
                    "column" : "0",
                    "value_begin":"1",
                    "keyword": "multi"
                 },
               {"key": "inverter",
                    "name":"Pac",
                    "orient": "row",
                    "type" : "series",
                    "row" : "0",
                    "value_begin":"1",
                 },
                {"key": "inverter",
                    "name":"Pdc1",
                    "orient": "row",
                    "type" : "series",
                    "row" : "0",
                    "value_begin":"1",
                 },
                {"key": "inverter_mppt",
                    "name":"INV",
                    "orient": "row",
                    "type" : "ids",
                    "row" : "0",
                    "column" : "0",
                    "value_begin":"1",
                    "keyword": "multi"
                 },
                    {"key": "inverter_mppt",
                    "name":"Pac",
                    "orient": "row",
                    "type" : "series",
                    "row" : "0",
                    "value_begin":"1",
                 },
                {"key": "inverter_mppt",
                    "name":"Pdc1",
                    "orient": "row",
                    "type" : "series",
                    "row" : "0",
                    "value_begin":"1",
                 },
                {"key": "inverter_mppt",
                    "name":"Time",
                    "orient": "column",
                    "type" : "vector",
                    "column" : "1",
                    "from" : "1",
                    "to":"-1",
                 }, 
                 {"key": "inverter_mppt",
                    "name":"Test_single_Value",
                    "type": "entity",
                    "keyword" : "test",
                 }
            ])
test_csv2("qparsing/core/tests/test_data/csv/input/test_id_input/" , ";",  0)
# test.to_csv(".")


test_csv3 = CSVParser([
                {"key": "inverter",
                    "name":"INV",
                    "orient": "row",
                    "type" : "ids",
                    "keyword": "file_name"
                 },
               {"key": "inverter",
                    "name":"Pac_watt",
                    "orient": "row",
                    "type" : "series",
                    "row" : "0",
                    "value_begin":"1",
                 },
                {"key": "inverter",
                    "name":"time",
                    "orient": "row",
                    "type" : "series",
                    "row" : "0",
                    "value_begin":"1",
                 },
                {"key": "inverter",
                    "name":"Phase1_voltage_volt",
                    "orient": "row",
                    "type" : "series",
                    "row" : "0",
                    "value_begin":"1",
                 }
            ])
test_csv3("qparsing/core/tests/test_data/csv/" , ";",  0)
test_csv3.to_csv(".")


# #XMLParser
# # Expect number of dictionary as list
# # Name in the dictionary will be used to catogorized to file
kwargs = [{"name": "inverter",
            "node":"Key",
            "key":"Pac",
            "degree":"Mean"},

            {"name": "inverter",
            "node":"Key",
            "key":"Seri",
            "degree":"Mean"},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":"Amp",
            "degree":"Mean"},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":"Vol",
            "degree":"Mean"},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":"E-total",
            "degree":"Mean"}
    ]

xml_parser = XMLParser(kwargs)

# # Here, pattern search for xml files are given, and the root name of each document
# # Uncomment to run
xml_parser("qparsing/core/tests/test_data/xml/", "WebBox");

# # Export location, it will write multiple files if there are more than one unique name in the kwargs dict

# # Uncomment to run
# xml_parser.to_csv(".");


# # Decompressdir, Compressdir, Cleandir examples

# Decompressdir()("qparsing/core/tests/test_data/gz/")
# Cleandir()("qparsing/core/tests/test_data/gz/", "*.csv")
# Compressdir()("qparsing/core/tests/test_data/gz/", "test")