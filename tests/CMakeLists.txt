cmake_minimum_required(VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)

project(test_dtables)
find_package(refacto)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(refacto_FOUND)
  add_executable(${PROJECT_NAME} test_dtables.cpp)
  target_link_libraries(test_dtables refacto)
  target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
else()
  message(FATAL_ERROR "refacto library not found")
endif()

