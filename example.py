from parsing.engine import CSVParser


p = CSVParser({"csvparser" :{"path":"code/src/core/tests/test_data/csv/input/min200611.csv",
        "delim":";","skip":"0"}})


data = {
    "series":
        {"pac" :
            {"name":"Pac",
            "orient": "row",
            "row":0,
            "index": "1",
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
  #  "vector_value":
  #      {"date":
   #         {"orient":"column",
    #        "index":0,
     #       "from":2,
      #     "to":-1},
       # "time":
        #    {"orient":"column",
         #   "index":1,
          #  "from":2,
           # "to":-1}
        #},
    #"single_value":
     #   {"date":
      #      {"orient":"column",
       #     "orient":"row",
            # "row":0}
        # }
}

p(data)
p.to_csv("here.csv")
