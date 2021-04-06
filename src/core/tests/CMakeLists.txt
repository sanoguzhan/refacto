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
build_tests(test_table test_table.cpp)
build_tests(test_controller test_controller.cpp)
build_tests(test_xmlparser test_xmlparser.cpp)


