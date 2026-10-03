cmake_minimum_required(VERSION 3.20)
project(PixieLib LANGUAGES CXX)

# Only headers: the primitives are templates and small structs, compiled inside whoever includes them.
add_library(pixie INTERFACE)
add_library(pixie::pixie ALIAS pixie)
target_include_directories(pixie INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(pixie INTERFACE cxx_std_20)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    option(PIXIE_BUILD_TESTS "Build the tests of PixieLib" ON)
else()
    option(PIXIE_BUILD_TESTS "Build the tests of PixieLib" OFF)
endif()

if(PIXIE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
