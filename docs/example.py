from qparsing.engine import CSVParser
from qparsing.engine import XMLParser
from qparsing.engine import Decompressdir, Cleandir, Compressdir

""" CSV PARSER Examples"""
# Conditional id search
test_csv0 = CSVParser([
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
# test_csv0("qparsing/core/tests/test_data/csv/input/test_one_input/" , ";",  3)
# test_csv0.to_csv(".")


# Grouping elements with column name
test_csv1 = CSVParser([
                {"key": "inverter",
                    "name":"Mppt_power",
                    "orient": "row",
                    "type" : "group",
                    "row" : "0",
                    "value_begin" : "1",
                    "condition": {
                   "id": "(.*)_string_current_amper",
                    "name": ".*(_string_current_amper)"
                        }
                 },
            ])
# test_csv1("qparsing/core/tests/test_data/csv/input/test_id_group/" , ";",  0)
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
# test_csv2("qparsing/core/tests/test_data/csv/input/test_id_input/" , ";",  0)
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
# test_csv3("qparsing/core/tests/test_data/csv/" , ";",  0)
# test_csv3.to_csv(".")


# --------------------- | ----------------------#

# #XMLParser
# # Expect number of dictionary as list
# # Name in the dictionary will be used to catogorized to file
# Example 0:
# Values can be a single entity from XML Tree, in this case
# All files searched and values converted to a table
# Single search expect:
#        name of the grouping
#        node: XML node
#        key: search variable
#        degree: leaf of Key
#        type: single
#        output: New Column name
#
#! Note: if output value is given, type must be specified.

kwargs = [{"name": "inverter",
            "node":"Key",
            "key":"Pac",
            "degree":"Mean",
            "type":"single",
            "output":"foobar"},

            {"name": "inverter",
            "node":"Key",
            "key":"Seri",
            "degree":"Mean",
            "type":"single"},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":"Amp",
            "degree":"Mean",
            "type":"single"},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":"Vol",
            "degree":"Mean",
            "type":"single"},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":"E-Total",
            "degree":"Mean",
            "type":"single"}
    ]

xml_parser = XMLParser(kwargs)

# # Uncomment to run
xml_parser("qparsing/core/tests/test_data/xml/single_variables/", "WebBox");
# xml_parser.to_csv(".")

# Example 1:
# Values can be a group entity from XML Tree, in this case
# Here, values are grouped based on given condition
# Condition must include id and name keys
# for each condition, value column created.
#        name of the grouping
#        node: XML node
#        key: search variable
#        degree: leaf of Key
#        type: group
#        condition: {}
#
#! Note: Key regex is used for search and condition is used for transforming data


kwargs = [{"name": "inverter",
            "node":"Key",
            "key":"Pac",
            "degree":"Mean",
            "type": "single"},

            {"name": "inverter",
            "node":"Key",
            "key":"^(.*?)Uac",
            "degree":"Mean",
            "type": "group",
            "condition":{
                "id": "(.*)Uac",
                "name" :  ".*(Uac)"
            }}
    ]
xml_parser = XMLParser(kwargs)

# # Uncomment to run
xml_parser("qparsing/core/tests/test_data/vihiers/", "WebBox");
# xml_parser.to_csv(".")


# Example 2:
# Values can be multi columns from XML Tree, in this case
# Here, values are searched with given regex and 
# a column is created per found vairable
#        name of the grouping
#        node: XML node
#        key: search variable
#        degree: leaf of Key
#        type: multi
#
#! Note: Output column name cannot be given

kwargs = [{"name": "inverter",
            "node":"Key",
            "key":"Pac",
            "degree":"Mean",
            "type":"single"},

            {"name": "inverter",
            "node":"Key",
            "key":"^(.*?).Ms.Amp",
            "degree":"Mean",
            "type": "multi",
            },
    ]
xml_parser = XMLParser(kwargs)
# # Uncomment to run 
xml_parser("qparsing/core/tests/test_data/xml/multi-variables/sciheco/", "WebBox");
# xml_parser.to_csv(".");

# --------------------- | ----------------------#
# # Decompressdir, Compressdir, Cleandir examples

# Decompressdir()("qparsing/core/tests/test_data/gz/")
# --------------------- | ----------------------#
# Cleandir()("qparsing/core/tests/test_data/gz/", "*.csv")
# --------------------- | ----------------------#
# Compressdir()("qparsing/core/tests/test_data/gz/", "test")