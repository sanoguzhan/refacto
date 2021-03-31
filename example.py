from parsing.engine import CSVParser


p = CSVParser({"csvparser" :{"path":"src/core/tests/test_data/csv/input/min200611.csv",
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
            },
        "pdc":
            {"name":"Pdc",
            "orient": "row",
            "row":0,
            "index": 1,
            "cond":
                []
            }
        },
   "vector_value":
       {"date":
           {"orient":"column",
           "index":0,
           "from":1,
          "to":-1},
       "time":
           {"orient":"column",
           "index":1,
           "from":1,
           "to":-1}
        },
    "single_value":
       {"name":
           {"name":"test",
               "orient":"orient",
           "row":1,
            "column":0}
        }
}

p(data)
p.to_csv("foo.csv")
