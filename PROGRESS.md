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
| | 2.6 Matrix operations | ⚪ Not Started | |
| **3. Strings** | ... | ⚪ Not Started | |
| **4. Pointers** | ... | ⚪ Not Started | |
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






---
*Status Legend: ⚪ Not Started | 🟡 In Progress | ✅ Completed | 🚀 Optimized (Interview Ready)*


---
*Status Legend: ⚪ Not Started | 🟡 In Progress | ✅ Completed | 🚀 Optimized (Interview Ready)*
