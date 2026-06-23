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
    tests/test_prime.cpp
    1_Basic_C_Programming/1.2_Prime_number_checks.c
    tests/test_reverse.cpp
    1_Basic_C_Programming/1.3_Reversing_numbers.c
    tests/test_count_digits.cpp
    1_Basic_C_Programming/1.4_Counting_digits.c
    tests/test_gcd.cpp
    1_Basic_C_Programming/1.5_Finding_GCD.c
    tests/test_palindrome.cpp
    1_Basic_C_Programming/1.6_Palindrome_checks.c
    tests/test_swap.cpp
    1_Basic_C_Programming/1.7_Swapping_numbers.c
    tests/test_array_ops.cpp
    2_Arrays/2.1_Finding_the_largest_smallest_and_second-largest_elements.c
    2_Arrays/2.2_In-place_reversal.c
    2_Arrays/2.3_Rotating_by_N_positions.c
    2_Arrays/2.4_Removing_duplicates_from_sorted_arrays.c
    tests/test_linked_list.cpp
    9_Linked_Lists/9.2_Creating_singly_linked_lists.c
    9_Linked_Lists/9.3_Traversal.c
    9_Linked_Lists/9.4_Inserting_nodes_at_the_beginning_or_end.c
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
