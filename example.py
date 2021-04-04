from parsing.engine import CSVParser


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
p.to_csv("foo.csv")
