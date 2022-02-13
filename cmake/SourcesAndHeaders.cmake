set(sources
  src/controller.cpp
  src/csvparser.cpp
  src/customparser.cpp
  src/pugixml.cpp
  src/table.cpp
  src/xmlparser.cpp
   
)

set(headers
    include/refacto/cod.hpp
    include/refacto/controller.hpp
    include/refacto/csvparser.hpp
    include/refacto/customparser.hpp
    include/refacto/parser.hpp
    include/refacto/pugixml.hpp
    include/refacto/table.hpp
    include/refacto/xmlparser.hpp

)

set(test_sources
  src/test_cod.cpp
  src/test_controller.cpp
  src/test_csv_cases.cpp
  src/test_customparser.cpp
  src/test_table.cpp
  src/test_xmlparser.cpp
)
