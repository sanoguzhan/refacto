
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2 -fPIC -funroll-loops")
add_compile_options("-fmax-errors=10")
# add_compile_definitions(LOG)
add_compile_options("-Wno-deprecated-declarations")
link_libraries(stdc++fs)

project(refacto VERSION 0.0.1 LANGUAGES CXX)
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)
include(GNUInstallDirs)
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Code Static Analysis

# set(CMAKE_CXX_CLANG_TIDY 
#   clang-tidy;
#   -header-filter=.;
#   -checks=*;
#   --export-fixes=fixes.yml;)

option(ENABLE_CODE_ANALYSIS "Run code analysis" OFF)
message(STATUS "ENABLE_CODE_ANALYSIS       ${ENABLE_CODE_ANALYSIS}")

if(ENABLE_CODE_ANALYSIS)
    find_program(cppcheck cppcheck)
    message(STATUS "cppcheck                   ${cppcheck}")
    if(NOT (cppcheck MATCHES "NOTFOUND"))
        # Notes:
        # - unmatchedSuppression needed since not all source files are
        #   analyzed simultaneously
        # - unusedFunction needed since not all source files are
        #   analyzed simultaneously
        # - template needed to force "warning" into output string, and
        #   to make the 'id' available for suppression
        set(CMAKE_CXX_CPPCHECK "${cppcheck}"
            "--enable=all"
            "-i/pugixml.cpp"
            "--language=c++"
            "--inconclusive"
            "--inline-suppr"
            "--quiet"
            "--suppress=unmatchedSuppression"
            "--suppress=unusedFunction"
            "--template='{file}:{line}: warning: {id} ({severity}): {message}'")
    endif()
endif(ENABLE_CODE_ANALYSIS)

#Code dynamic analysis Memory - Debug
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

#Code dynamic analysis: Enable Undefined Behaviour Sanitizer (UBSan) for GCC/Clang
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
# set (CMAKE_EXELINKER_FLAGS "${CMAKE_EXELINKER_FLAGS} -fsanitize=undefined")

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(FATAL_ERROR "First, Run \"conan install ..\" !!!")
endif()


file(GLOB SOURCES src/*.cpp)
include_directories(include)
add_library(${PROJECT_NAME} STATIC ${SOURCES})

enable_testing()

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
configure_file(refacto.pc.in refacto.pc @ONLY)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})

file(GLOB HEADERS include/*.hpp)
install(FILES ${HEADERS} DESTINATION  ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${CMAKE_BINARY_DIR}/refacto.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
install(EXPORT ${PROJECT_NAME} FILE refacto-config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
