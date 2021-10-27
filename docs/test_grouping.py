from parsing.engine import XMLParser
import time
kwargs = [{"name": "inverter",
            "node":"Key",
            "key":"Pac",
            "degree":"TimeStamp",
            "type":"single",
            "output":"date_time"},

            {"name": "inverter",
            "node":"Key",
            "key":"Pac",
            "degree":"Mean",
            "type":"single"},
           
            {"name": "inverter",
            "node":"Key",
            "key":"Seri",
            "degree":"Mean",
            "type":"single"},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":"Amp",
            "degree":"TimeStamp",
            "type":"single",
            "output": "date_time"},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":"Ipv",
            "degree":"Mean",
            "type":"single"},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":"Upv-Ist",
            "degree":"Mean",
            "type":"single"},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":"Seri",
            "degree":"Mean",
            "type":"single"},


    ]

# for i in range(10):
#     a = time.time()
#     xml_parser = XMLParser(kwargs)

#     # # Uncomment to run
#     xml_parser("parsing/core/tests/test_data/chanzeaux/", "WebBox")
#     xml_parser.to_csv(".")
#     print("Time without group: %f" % (time.time() -a))


kwargs = [{"name": "inverter",
            "node":"Key",
            "key":"Seri",
            "degree":"TimeStamp",
            "output" : "date_time",
            "type": "single"},


            {"name": "inverter",
            "node":"Key",
            "key":"Pac",
            "degree":"Mean",
            "type": "single"},


            {"name": "inverter",
            "node":"Key",
            "key":"Seri",
            "degree":"Mean",
            "type": "single"},

            # {"name": "inverter",
            # "node":"Key",
            # "key":".*\\w+.Ms.(Amp)",
            # "degree":"TimeStamp",
            # "type": "group",
            # "condition":{
            #     "id": "(.*\\w+).Ms.Amp",
            #     "name" :  ".*\\w+.Ms.(Amp)"
            # }},


            {"name": "inverter_mppt",
            "node":"Key",
            "key":".*\\w+.Ms.(Amp)",
            "degree":"TimeStamp",
            "type": "group",
            "condition":{
                "id": "(.*\\w+).Ms.Amp",
                "name" :  ".*\\w+.Ms.(Amp)"
            }},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":".*\\w+.Ms.(Vol)",
            "degree":"Mean",
            "type": "group",
            "condition":{
                "id": "(.*\\w+).Ms.Vol",
                "name" :  ".*\\w+.Ms.(Vol)"
            }},

            {"name": "inverter_mppt",
            "node":"Key",
            "key":".*\\w+.Ms.(Amp)",
            "degree":"Mean",
            "type": "group",
            "condition":{
                "id": "(.*\\w+).Ms.Amp",
                "name" :  ".*\\w+.Ms.(Amp)"
            }}
    ]

b = time.time()
xml_parser = XMLParser(kwargs)

# # Uncomment to run
xml_parser("parsing/core/tests/test_data/chanzeaux/", "WebBox")

xml_parser.to_csv(".")
print("Time with grouping: %f" % (time.time() -b))

