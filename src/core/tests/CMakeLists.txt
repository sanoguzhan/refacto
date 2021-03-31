cmake_minimum_required(VERSION 2.6)
set (CMAKE_CXX_STANDARD 17)

add_compile_options("-std=gnu++17")
set(CMAKE_CXX_FLAGS "-O3")
add_compile_options("-Wno-error=deprecated-declarations")
project(test_dtables)
enable_testing()


find_package(refacto)
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})
link_libraries(stdc++fs)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# add_executable(test_dtables 
#       test_dtables.cpp)

# add_executable(test_csvparser 
#   test_csvparser.cpp)

# add_executable(test_table 
# test_table.cpp)

# add_executable(test_csv_cases 
# test_csv_cases.cpp)

# add_executable(test_controller 
# test_controller.cpp)

add_executable(test_xmlparser
test_xmlparser.cpp)

if(refacto_FOUND)

  #Fix this, Make it generic for eache executable
  # target_link_libraries(test_dtables refacto)
  # target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
  # target_link_libraries(test_dtables GTest::GTest GTest::Main)

  # target_link_libraries(test_csvparser refacto)
  # target_link_libraries(test_csvparser ${CONAN_LIBS})
  # target_link_libraries(test_csvparser GTest::GTest GTest::Main)

  # target_link_libraries(test_table refacto)
  # target_link_libraries(test_table ${CONAN_LIBS})
  # target_link_libraries(test_table GTest::GTest GTest::Main)

  # target_link_libraries(test_csv_cases refacto)
  # target_link_libraries(test_csv_cases ${CONAN_LIBS})
  # target_link_libraries(test_csv_cases GTest::GTest GTest::Main)

  # target_link_libraries(test_controller refacto)
  # target_link_libraries(test_controller ${CONAN_LIBS})
  # target_link_libraries(test_controller GTest::GTest GTest::Main)

  target_link_libraries(test_xmlparser refacto)
  target_link_libraries(test_xmlparser ${CONAN_LIBS})
  target_link_libraries(test_xmlparser GTest::GTest GTest::Main)

else()
  message(FATAL_ERROR "refacto library not found")
endif()

add_test(
    NAME test_dtables 
    COMMAND test_dtables -i "${CMAKE_SOURCE_DIR}/test.txt" "totoro.txt"
)