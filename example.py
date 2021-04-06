from parsing.engine import CSVParser
from parsing.engine import XMLParser

# CSVParser
# Constructor input should be all string
p = CSVParser({"csvparser" :{"path":"src/core/tests/test_data/csv/input/2110130777_treated_data.csv",
        "delim":";","skip":"0"}})

data = {
    "series":
        {"pac" :
            {"name":"Pac",
            "orient": "row",
            "row":0,
            "index": 1,
            "cond":[
                ],
            }
        },
   "vector_value":
       {"date_time":
           {"orient":"column",
           "index":0,
           "from":1,
          "to":-1},
        }
}

p(data)

#XMLParser
# Expect number of dictionary as list
# Name in the dictionary will be used to catogorized to file
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

# Here, pattern search for xml files are given, and the root name of each document
xml_parser("src/core/tests/test_data/xml/*.xml", "WebBox");

# Export location, it will write multiple files if there are more than one unique name in the kwargs dict
xml_parser.to_csv(".");
