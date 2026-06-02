cmake_minimum_required(VERSION 3.10)
project(C_Practice_Tests)

set(CMAKE_CXX_STANDARD 11)

# Enable testing
include(CTest)

# Manual GTest Setup
set(GTEST_ROOT "C:/gtest_install")
find_package(GTest REQUIRED)

# Define the test executable
add_executable(run_tests 
    tests/test_fibonacci.cpp
    1_Basic_C_Programming/1.1_Fibonacci_series.c
)

# Link to gtest
target_link_libraries(run_tests GTest::gtest GTest::gtest_main)

# Add definitions for testing
target_compile_definitions(run_tests PRIVATE TESTING)

# Include the source directories so headers can be found
target_include_directories(run_tests PRIVATE 
    1_Basic_C_Programming
    ${GTEST_INCLUDE_DIRS}
)

# Discover tests
include(GoogleTest)
gtest_discover_tests(run_tests)
