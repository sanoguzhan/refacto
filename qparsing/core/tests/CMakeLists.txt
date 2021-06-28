cmake_minimum_required(VERSION 2.6)
set (CMAKE_CXX_STANDARD 17)

add_compile_options("-std=gnu++17")
add_compile_options("-fmax-errors=10")
set(CMAKE_CXX_FLAGS "-O2")
add_compile_definitions(LOG)
add_compile_options("-Wno-deprecated-declarations")
project(test_dtables)
enable_testing()


find_package(refacto)
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})
link_libraries(stdc++fs)

option(ENABLE_CODE_ANALYSIS "Run code analysis" OFF)
message(STATUS "ENABLE_CODE_ANALYSIS       ${ENABLE_CODE_ANALYSIS}")


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

function(build_tests target_name target_source)
  add_executable(${target_name} 
        ${target_source})
  if(refacto_FOUND)
    target_link_libraries(${target_name} refacto)
    target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
    target_link_libraries(${target_name} GTest::GTest GTest::Main)
  else()
    message(FATAL_ERROR "refacto library not found")
endif()
endfunction()

build_tests(test_dtables test_dtables.cpp)
build_tests(test_csv_cases test_csv_cases.cpp)

build_tests(test_csvparser test_csvparser.cpp)
build_tests(test_customparser test_customparser.cpp)

build_tests(test_table test_table.cpp)
build_tests(test_controller test_controller.cpp)
build_tests(test_xmlparser test_xmlparser.cpp)
build_tests(test_cod test_cod.cpp)

