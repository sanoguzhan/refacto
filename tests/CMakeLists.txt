cmake_minimum_required(VERSION 2.6)
set (CMAKE_CXX_STANDARD 17)

add_compile_options("-std=gnu++17")
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
project(test_dtables)
enable_testing()


find_package(refacto)
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIR})
link_libraries(stdc++fs)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(test_dtables 
      test_dtables.cpp)

add_executable(test_csvparser 
test_csvparser.cpp)

if(refacto_FOUND)

  #Fix this, Make it generic for eache executable
  target_link_libraries(test_dtables refacto)
  target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
  target_link_libraries(test_dtables GTest::GTest GTest::Main)

  target_link_libraries(test_csvparser refacto)
  target_link_libraries(test_csvparser ${CONAN_LIBS})
  target_link_libraries(test_csvparser GTest::GTest GTest::Main)

else()
  message(FATAL_ERROR "refacto library not found")
endif()

add_test(
    NAME test_dtables 
    COMMAND test_dtables -i "${CMAKE_SOURCE_DIR}/test.txt" "totoro.txt"
)