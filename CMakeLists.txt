
cmake_minimum_required(VERSION 3.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")
add_compile_options("-std=gnu++17")
link_libraries(stdc++fs)

project(refacto VERSION 0.0.1 LANGUAGES CXX)
include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)
include(GNUInstallDirs)
include(CTest)



if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(FATAL_ERROR "First, Run \"conan install ..\" !!!")
endif()


file(GLOB SOURCES src/*.cpp)
include_directories(include)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

enable_testing()

target_include_directories(
    ${PROJECT_NAME} PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
configure_file(refacto.pc.in refacto.pc @ONLY)


install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})

file(GLOB HEADERS include/*.hpp)
install(FILES ${HEADERS} DESTINATION  ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(FILES ${CMAKE_BINARY_DIR}/refacto.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
install(EXPORT ${PROJECT_NAME} FILE refacto-config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})


target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
