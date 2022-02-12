
cmake_minimum_required(VERSION 3.2)
project(refacto VERSION 0.0.1 LANGUAGES CXX)

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
	set(CMAKE_BUILD_TYPE Release CACHE STRING
		"Type of build, options are: Debug, Release" FORCE)
endif()

add_compile_options(-Wall -Wextra -fPIC -funroll-loops -fmax-errors=10)
set(CMAKE_CXX_STANDARD 17)


set(CMAKE_CXX_FLAGS_DEBUG "-g2 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

#Code dynamic analysis Memory - Debug
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

link_libraries(stdc++fs)


set(ENABLE_CODE_ANALYSIS "Run code analysis" OFF)
message(STATUS "ENABLE_CODE_ANALYSIS ${ENABLE_CODE_ANALYSIS}")

if(ENABLE_CODE_ANALYSIS)
    find_program(cppcheck cppcheck)
    message(STATUS "cppcheck ${cppcheck}")
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
endif()



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
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${CMAKE_BINARY_DIR}/refacto.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
install(EXPORT ${PROJECT_NAME} FILE refacto-config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UCASE)
message(STATUS ">> CXX Compile Flags (For ${CMAKE_BUILD_TYPE}):\
 ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE_UCASE}}")
