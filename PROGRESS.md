# Embedded Systems Interview Preparation - Progress Tracker

## Session Context
- **Start Date:** 2026-05-31
- **Focus:** Embedded C Programming (Efficiency, Robustness, Hardware Awareness)
- **Environment:** Windows (win32)
- **Goal:** Behave as a Subject Matter Expert and help user complete all exercises in the `2026_C_practice` folder with "Senior Embedded Engineer" quality.

## Master Progress Table

| Topic | Sub-Topic | Status | Notes |
| :--- | :--- | :--- | :--- |
| **1. Basic C** | 1.1 Fibonacci | 🚀 Optimized | Expert refactor: overflow safety, single loop. |
| | 1.2 Prime Checks | 🚀 Optimized | O(sqrt(n)) via 6k+/-1 logic. Overflow safe (i <= n/i). |
| | 1.3 Reversing Numbers| 🚀 Optimized | Signed int32_t safety, explicit overflow checks for MAX/MIN. |
| | 1.4 Counting Digits | 🚀 Optimized | Fixed-width types (uint8_t return). Handled INT32_MIN safely without division overflow check. |
| | 1.5 Finding GCD | 🚀 Optimized | Modulo-based Euclidean GCD. Safe INT32_MIN absolute value logic. |
| | 1.6 Palindrome | 🚀 Optimized | Reverses only second half to avoid overflow. Safe for negative and 0. |
| | 1.7 Swapping | 🚀 Optimized | XOR-based swap with null check & self-aliasing safety. |
| **2. Arrays** | 2.1 Min/Max/2nd-Max | 🚀 Optimized | Single-pass logic for 2nd max, duplicate/edge boundaries handled. |
| | 2.2 In-place reversal | 🚀 Optimized | Fixed-width types, O(1) space, boundary checks, zero-underflow safe. |
| | 2.3 Rotating by N positions | 🚀 Optimized | Handled size_t underflow for k=0, added static scoping to helpers. |
| | 2.4 Removing duplicates | 🚀 Optimized | In-place two-pointer approach, O(N) time, O(1) space, size_t safety checks. |
| | 2.5 Merging sorted arrays | 🚀 Optimized | Fixed flipped capacity check condition, missing destination NULL check, and incorrect leftover loop logic. Enabled graceful merge of single empty arrays and added size_t overflow check. |
| | 2.6 Matrix operations | 🚀 Optimized | Refactored with dynamic sizing, const correctness, safe signed overflow detection, and overlap/in-place transpose protection. |
| **3. Strings** | 3.1 Finding length | 🚀 Optimized | Pointer-based loop, const-correct, size_t safety checks, handles NULL. |
| | 3.2 In-place reversal | 🚀 Optimized | In-place two-pointer swap, O(1) space, enum-based error statuses. Safe for NULL and empty string pointer underflow. |
| | 3.3 Palindrome checks | 🚀 Optimized | In-place two-pointer match, O(1) space, enum-based error statuses, const-correct, safe for empty string pointer underflow. |
| | 3.4 Counting vowels/consonants/digits | 🚀 Optimized | Enum status, null checks, count initialization safety, case-insensitivity using bitwise OR optimization. |
| | 3.5 String comparison | 🚀 Optimized | Double NULL check, result pointer safety, and unsigned char cast for standard-conforming comparison. |
| | 3.6 Custom string copy | 🚀 Optimized | In-place forward/backward copy based on pointer overlap. Shifted size_t indexing to prevent underflow/infinite loops. |
| | 3.7 Frequency counting | 🚀 Optimized | Fixed-width types, NULL checks, capacity bounds verification, internal array initialization, and static linkage helpers. |
| | 3.8 Removing spaces | 🚀 Optimized | In-place removal using two-pointer approach, avoiding redundant self-assignments. |
| **4. Pointers** | 4.1 Swapping pointer values | 🚀 Optimized | Byte-by-byte generic swap using `void*` and `unsigned char*` to avoid strict aliasing violation. |
| | 4.2 Array traversal and reversal | 🚀 Optimized | Const-correct traversal, two-pointer array reversal, bounds underflow protection, tested with GTest. |
| | 4.3 Pointer-to-pointer logic | 🚀 Optimized | Heap allocation via double pointer, size_t overflow guard, dangling pointer elimination on free. |
| | 4.4 Printing elements | ⚪ Not Started | |
| | 4.5 Modifying variables within functions | ⚪ Not Started | |
| | 4.6 Implementing callback functions | ⚪ Not Started | |
| **5. Structures**| ... | ⚪ Not Started | |
| **6. Bit Manipulation**| ... | ⚪ Not Started | |
| **7. Recursion** | ... | ⚪ Not Started | |
| **8. Search/Sort**| ... | ⚪ Not Started | |
| **9. Linked Lists** | 9.1 Dynamic memory allocation | 🚀 Optimized | Allocated node with malloc, NULL check, and free cleanup. |
| | 9.2 Creating singly list | 🚀 Optimized | Double-pointer construction, correct order, clean failure cleanup. |
| | 9.3 Traversal | 🚀 Optimized | Const-correct traversal & search, safe recursive reverse print. |
| | 9.4 Inserting nodes | 🚀 Optimized | Double-pointer traversal and insertion without special-casing the empty list. |
| | 9.5 Deleting nodes | 🚀 Optimized | Double-pointer traversal to bypass and delete all matches in one pass without special-casing the head node. |
| | 9.6 Counting nodes | 🚀 Optimized | Iterative count, const-correct, size_t return. |
| | 9.7 Reversing list | 🚀 Optimized | In-place O(N) iterative reversal with three-pointer swap. |
| | 9.8 Implementing LRU cache | 🚀 Optimized | Complete $O(1)$ LRU Cache with hash table buckets, sentinel doubly-linked list, $O(1)$ get/put, and leak-free destruction. |
| | 9.9 Implementing Hash Table | 🚀 Optimized | Chaining-based hash table using linked lists. Clean pointer traversal, collision resolution, duplicate/size management, and leak-free destruction. |
| **10. Stacks/Queues**| ... | ⚪ Not Started | |

## Technical Key Takeaways
- *[2026-05-31]*: Initiated practice. Discussed the importance of `stdint.h` and iterative approaches over recursion for stack safety in embedded systems.
- *[2026-06-02]*: Optimized Prime Number check to O(sqrt(n)). Implemented overflow-safe loop condition `i <= n / i` to prevent infinite loops on large 32-bit inputs. Integrated GoogleTest for performance benchmarking.
- *[2026-06-05]*: Completed 1.3 Reversing Numbers. Implemented robust `int32_t` reversal with explicit overflow checks for both `INT32_MAX` and `INT32_MIN`. Handled C99 signed modulo behavior by using `int8_t` for digit extraction. Integrated 7 GoogleTest cases in `tests/test_reverse.cpp` to verify boundary conditions. Ready for 1.4 Counting Digits.
- *[2026-06-07]*: Completed 1.4 Counting Digits. Optimized return type to `uint8_t` to save stack space. Verified that division-based iteration naturally avoids overflows, making bounds checks unnecessary. Handled `INT32_MIN` safely. Integrated 7 GTest cases in `tests/test_count_digits.cpp`. Ready for 1.5 Finding GCD.
- *[2026-06-14]*: Completed 1.5 Finding GCD. Optimized from O(N) subtraction to O(log N) modulo-based iterative Euclidean algorithm. Handled the INT32_MIN overflow case safely using uint32_t absolute casting. All 7 test cases passed in 0 ms. Ready for 1.6 Palindrome.
- *[2026-06-14]*: Completed 1.6 Palindrome. Implemented a branch-reduced half-reversal algorithm that completely avoids signed overflow without using 64-bit types. Handled negative numbers, numbers ending in 0, and single digits (including 0) safely. Integrated 7 GoogleTest cases. Ready for 1.7 Swapping.
- *[2026-06-15]*: Completed 1.7 Swapping. Implemented an XOR-based swap with safety checks for null pointer dereferences and pointer aliasing. Integrated 6 GoogleTest cases. Ready for 2.1 Finding the largest, smallest, and second-largest elements in Arrays.
- *[2026-06-17]*: Completed 2.1 Arrays (largest, smallest, second_largest). Replaced hardcoded limits initialization with safe array bounds iteration, corrected loop variable typing to prevent sign-mismatch, and implemented single-pass duplicate-max tracking. All 53 tests passing. Ready for 2.2 In-place reversal.
- *[2026-06-20]*: Completed 2.2 In-place reversal. Implemented robust two-pointer swap using size_t indices. Prevented unsigned index underflow using strict initial validation (len == 0). Integrated 4 GoogleTest cases. Ready for 2.3 Rotating by N positions.
- *[2026-06-20]*: Completed 2.3 Rotating by N positions. Solved critical unsigned index underflow `k-1` when rotating by 0 (or multiples of length). Limited linkage scope of helper functions to static to prevent multiple definitions during test compilation. Integrated 6 GoogleTest cases. Ready for 2.4 Removing duplicates.
- *[2026-06-20]*: Completed 2.4 Removing duplicates from sorted arrays. Optimized with an in-place two-pointer algorithm requiring O(N) time and O(1) space. Ensured type safety (using size_t for lengths and pointers to avoid stack corruption on 64-bit systems) and null pointer protection. Integrated 5 GoogleTest cases. Ready for 2.5 Merging sorted arrays.
- *[2026-06-21]*: Completed 9.1 Dynamic Memory Allocation. Implemented memory allocation using malloc, robust NULL checks, and free cleanup with fixed-width types (int32_t) and pointer management. Ready for 9.2 Creating singly linked lists.
- *[2026-06-21]*: Completed 9.2 Creating singly list. Implemented a double pointer (Node** ppNext) pattern for clean node insertion without special-casing the head, maintaining array order. Resolved dangling pointer issue on allocation failure. Ready for 9.3 Traversal.
- *[2026-06-21]*: Completed 9.3 Traversal. Implemented const-correct traversal, recursive reverse printing with a safe NULL base case, and target node searching. Prevented linkage conflicts by scoping duplicate helper functions with static. Integrated 6 GoogleTest cases. Ready for 9.4 Inserting nodes at the beginning or end.
- *[2026-06-22]*: Completed 9.4 Inserting nodes. Optimized `insert_end` to use pointer-to-pointer (`Node** ppCurr`) traversal, eliminating special cases for empty lists. Added 6 GoogleTest test cases. Ready for 9.5 Deleting nodes by value.
- *[2026-06-25]*: Completed 9.5 Deleting nodes. Implemented the double-pointer (`Node**`) traversal to delete all occurrences of a target value, eliminating special-casing for the head node. Added 8 GoogleTest test cases testing all scenarios (empty, single-node, middle, tail, multiples, null pointer input). Ready for 9.6 Counting nodes.
- *[2026-06-28]*: Completed 9.6 Counting nodes. Implemented an iterative O(N) traversal. Ensured stack safety, const-correctness, and size_t type safety. Restrained linking conflicts of helpers via static scoping. Integrated 3 unit tests. Ready for 9.7 Reversing the list.
- *[2026-06-28]*: Completed 9.7 Reversing list. Implemented an in-place $O(N)$ three-pointer loop to reverse the list. Handled outer `ppHead` and empty `*ppHead` list bounds safely. Added 4 unit tests covering null pointer handling, single-node lists, and multi-node sequences. Ready for 10. Stacks & Queues.
- *[2026-06-29]*: Completed 2.5 Merging sorted arrays. Fixed logical bugs in the leftover array copy loop (`idx_1 < idx_2` condition) and the flipped destination capacity check. Added safety check for `dest == NULL` and size_t integer overflow verification. Allowed graceful merging of single empty arrays while preserving error-handling for dual empty arrays. Added 7 robust GoogleTest cases. Ready for 2.6 Matrix operations.
- *[2026-07-02]*: Completed 2.6 Matrix operations. Refactored `matrix_add`, `matrix_transpose`, and `matrix_print` with dynamic sizing, proper const-correctness, and explicit safety checks: buffer overlap (in-place transpose for square matrices only; partial/non-square overlaps rejected) and overflow-safe signed integer addition check logic. Added 10 robust GoogleTest test cases. Ready for 3. Strings.
- *[2026-07-04]*: Completed 3.1 Finding length. Refactored `cus_strlen` to use `const char*` for safety, `size_t` for portability/overhead optimization, and pointer arithmetic instead of array offset indexing. Added 4 GoogleTest test cases. Ready for 3.2 In-place reversal.
- *[2026-07-04]*: Completed 3.2 In-place reversal. Refactored `reverse_string` to return a `rev_status_t` enum. Resolved a critical undefined behavior (pointer underflow before the start of the array) when input is an empty string `""` by checking `*str == '\0'` early. Added 6 GoogleTest test cases. Ready for 3.3 Palindrome checks.
- *[2026-07-04]*: Completed 3.3 Palindrome checks. Refactored `check_palindrome` to be `const`-correct (using `const char*` for safety against read-only Flash string literals). Handled NULL pointers and avoided pointer underflow for empty strings. Added 7 GoogleTest test cases. Ready for 3.4 Counting vowels/consonants/digits.
- *[2026-07-05]*: Completed 3.4 Counting vowels/consonants/digits. Added enum statuses, strict null validations, and explicit struct counts zero-initialization to protect against garbage values in uninitialized stack structs. Optimized alphabetical check hierarchy with `else if` and implemented a bitwise lowercase conversion `*c | 0x20` to reduce comparison overhead for case-insensitive vowel matching. Enabled build and verified with 8 GoogleTest cases. Ready for 3.5 String comparison.
- *[2026-07-07]*: Completed 3.5 String comparison. Implemented strict safety checks for pointer inputs (s1, s2, and result). cast pointers to unsigned char to prevent platform-specific signed character overflow comparison failures. Added 11 unit tests. Ready for 3.6 Custom string copy (handling address overlapping).
- *[2026-07-09]*: Completed 3.7 Frequency counting. Implemented robust frequency analysis for lowercase 'a'-'z'. Handled NULL safety checks, output buffer capacity verification (`len < 26`), and array zero-initialization. Declared print helper as `static` to resolve linker multiple-definition errors during build. Integrated 6 GoogleTest cases. Ready for 3.8 Removing spaces.
- *[2026-07-16]*: Completed 3.8 Removing spaces and 4.1 Swapping pointer values. Discussed void** strict aliasing and pointer representation limitations in C. Designed type-safe, standard-compliant byte-level swap for pointer targets. Verified with 4 unit tests.

---
*Status Legend: ⚪ Not Started | 🟡 In Progress | ✅ Completed | 🚀 Optimized (Interview Ready)*

