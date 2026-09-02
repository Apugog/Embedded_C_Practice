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
    2_Arrays/2.5_Merging_sorted_arrays.c
    tests/test_matrix_ops.cpp
    2_Arrays/2.6_Matrix_operations_addition_and_transpose.c
    tests/test_strlen.cpp
    3_Strings_Without_Library_Functions/3.1_Finding_length.c
    tests/test_reverse_string.cpp
    3_Strings_Without_Library_Functions/3.2_In-place_reversal.c
    tests/test_palindrome_string.cpp
    3_Strings_Without_Library_Functions/3.3_Palindrome_checks.c
    tests/test_count_vowels_consonants_digits.cpp
    3_Strings_Without_Library_Functions/3.4_Counting_vowels_consonants_and_digits.c
    tests/test_strcmp.cpp
    3_Strings_Without_Library_Functions/3.5_String_comparison.c
    tests/test_strcpy.cpp
    3_Strings_Without_Library_Functions/3.6_Custom_string_copy_handling_address_overlapping.c
    tests/test_frequency.cpp
    3_Strings_Without_Library_Functions/3.7_Frequency_counting.c
    tests/test_remove_spaces.cpp
    3_Strings_Without_Library_Functions/3.8_Removing_spaces.c
    tests/test_swap_ptrs.cpp
    4_Pointers/4.1_Swapping_numbers.c
    tests/test_ptr_array_ops.cpp
    4_Pointers/4.2_Array_traversal_and_reversal.c
    tests/test_ptr_to_ptr.cpp
    4_Pointers/4.3_Pointer-to-pointer_logic.c
    tests/test_print_elements.cpp
    4_Pointers/4.4_Printing_elements.c
    tests/test_modify_variables.cpp
    4_Pointers/4.5_Modifying_variables_within_functions.c
    tests/test_generic_array.cpp
    4_Pointers/4.7_Generic_array_printing.c
    tests/test_linked_list.cpp
    9_Linked_Lists/9.2_Creating_singly_linked_lists.c
    9_Linked_Lists/9.3_Traversal.c
    9_Linked_Lists/9.4_Inserting_nodes_at_the_beginning_or_end.c
    9_Linked_Lists/9.5_Deleting_nodes_by_value.c
    9_Linked_Lists/9.6_Counting_nodes.c
    9_Linked_Lists/9.7_Reversing_the_list.c
    tests/test_lru_cache.cpp
    9_Linked_Lists/9.8_Implementing_LRU_cache.c
    tests/test_hash_table.cpp
    9_Linked_Lists/9.9_Implementing_Hash_Table.c
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
