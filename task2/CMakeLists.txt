cmake_minimum_required(VERSION 3.10)
project(MatrixLib VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WITH_TESTS "Enable building tests with Google Test" OFF)

add_library(MATRIX STATIC
    source/Matrix.hpp
    source/Matrix.cpp
    source/SparseMatrix.hpp
    source/SparseMatrix.cpp
)

target_include_directories(MATRIX PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

if(WITH_TESTS)
    enable_testing()

    find_package(GTest REQUIRED)
    include(GoogleTest)

    set(TEST_SOURCES
        tests/testMatrix.cpp
        tests/testSparseMatrix.cpp
    )

    add_executable(test_lib ${TEST_SOURCES})

    target_link_libraries(test_lib PRIVATE
        MATRIX
        GTest::gtest
        GTest::gtest_main
    )

    gtest_discover_tests(test_lib)
endif()
