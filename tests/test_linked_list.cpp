#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>
#include <string>

extern "C" {
    typedef struct Node {
        int32_t data;
        struct Node* next;
    } Node;

    typedef enum {
        LIST_SUCCESS = 0,
        LIST_ERR_NULL,
        LIST_ERR_ALLOC
    } list_status_t;

    // From 9.2
    Node* create_list_from_array(const int32_t arr[], size_t size);
    void free_list(Node* head);

    // From 9.3
    void print_list(const Node* head);
    void print_list_reverse(const Node* head);
    const Node* find_node(const Node* head, int32_t target);

    // From 9.4
    list_status_t insert_beg(Node** ppHead, int32_t data);
    list_status_t insert_end(Node** ppHead, int32_t data);
}

TEST(LinkedListCreateTests, HandlesEmptyArray) {
    Node* head = create_list_from_array(NULL, 0);
    EXPECT_EQ(head, nullptr);
}

TEST(LinkedListCreateTests, HandlesSingleElement) {
    const int32_t arr[] = {42};
    Node* head = create_list_from_array(arr, 1);
    
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->data, 42);
    EXPECT_EQ(head->next, nullptr);
    
    free_list(head);
}

TEST(LinkedListCreateTests, HandlesMultipleElements) {
    const int32_t arr[] = {10, 20, 30, 40};
    Node* head = create_list_from_array(arr, 4);
    
    ASSERT_NE(head, nullptr);
    
    Node* temp = head;
    EXPECT_EQ(temp->data, 10);
    
    ASSERT_NE(temp->next, nullptr);
    temp = temp->next;
    EXPECT_EQ(temp->data, 20);
    
    ASSERT_NE(temp->next, nullptr);
    temp = temp->next;
    EXPECT_EQ(temp->data, 30);
    
    ASSERT_NE(temp->next, nullptr);
    temp = temp->next;
    EXPECT_EQ(temp->data, 40);
    EXPECT_EQ(temp->next, nullptr);
    
    free_list(head);
}

TEST(LinkedListCreateTests, FreeListHandlesNull) {
    // Should not crash
    free_list(nullptr);
}

// Traversal & Search Tests (9.3)
TEST(LinkedListTraversalTests, PrintsForwardCorrectly) {
    const int32_t arr[] = {10, 20, 30};
    Node* head = create_list_from_array(arr, 3);
    
    testing::internal::CaptureStdout();
    print_list(head);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "10 20 30 ");
    
    free_list(head);
}

TEST(LinkedListTraversalTests, PrintsForwardEmptyList) {
    testing::internal::CaptureStdout();
    print_list(nullptr);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");
}

TEST(LinkedListTraversalTests, PrintsReverseCorrectly) {
    const int32_t arr[] = {10, 20, 30};
    Node* head = create_list_from_array(arr, 3);
    
    testing::internal::CaptureStdout();
    print_list_reverse(head);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "30 20 10 ");
    
    free_list(head);
}

TEST(LinkedListTraversalTests, PrintsReverseEmptyList) {
    testing::internal::CaptureStdout();
    print_list_reverse(nullptr);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");
}

TEST(LinkedListTraversalTests, FindNodeReturnsCorrectNode) {
    const int32_t arr[] = {5, 15, 25};
    Node* head = create_list_from_array(arr, 3);
    
    const Node* found = find_node(head, 15);
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->data, 15);
    
    // Check it's actually the correct pointer in the list
    EXPECT_EQ(found, head->next);
    
    free_list(head);
}

TEST(LinkedListTraversalTests, FindNodeReturnsNullIfNotFound) {
    const int32_t arr[] = {5, 15, 25};
    Node* head = create_list_from_array(arr, 3);
    
    const Node* found = find_node(head, 99);
    EXPECT_EQ(found, nullptr);
    
    const Node* found_in_empty = find_node(nullptr, 5);
    EXPECT_EQ(found_in_empty, nullptr);
    
    free_list(head);
}

// Insertion Tests (9.4)
TEST(LinkedListInsertionTests, InsertBegHandlesNullHeadPointer) {
    EXPECT_EQ(insert_beg(nullptr, 10), LIST_ERR_NULL);
}

TEST(LinkedListInsertionTests, InsertBegInsertsIntoEmptyList) {
    Node* head = nullptr;
    EXPECT_EQ(insert_beg(&head, 42), LIST_SUCCESS);
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->data, 42);
    EXPECT_EQ(head->next, nullptr);
    free_list(head);
}

TEST(LinkedListInsertionTests, InsertBegInsertsMultipleElements) {
    Node* head = nullptr;
    EXPECT_EQ(insert_beg(&head, 10), LIST_SUCCESS);
    EXPECT_EQ(insert_beg(&head, 20), LIST_SUCCESS);
    EXPECT_EQ(insert_beg(&head, 30), LIST_SUCCESS);
    
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->data, 30);
    ASSERT_NE(head->next, nullptr);
    EXPECT_EQ(head->next->data, 20);
    ASSERT_NE(head->next->next, nullptr);
    EXPECT_EQ(head->next->next->data, 10);
    EXPECT_EQ(head->next->next->next, nullptr);
    
    free_list(head);
}

TEST(LinkedListInsertionTests, InsertEndHandlesNullHeadPointer) {
    EXPECT_EQ(insert_end(nullptr, 10), LIST_ERR_NULL);
}

TEST(LinkedListInsertionTests, InsertEndInsertsIntoEmptyList) {
    Node* head = nullptr;
    EXPECT_EQ(insert_end(&head, 42), LIST_SUCCESS);
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->data, 42);
    EXPECT_EQ(head->next, nullptr);
    free_list(head);
}

TEST(LinkedListInsertionTests, InsertEndInsertsMultipleElements) {
    Node* head = nullptr;
    EXPECT_EQ(insert_end(&head, 10), LIST_SUCCESS);
    EXPECT_EQ(insert_end(&head, 20), LIST_SUCCESS);
    EXPECT_EQ(insert_end(&head, 30), LIST_SUCCESS);
    
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->data, 10);
    ASSERT_NE(head->next, nullptr);
    EXPECT_EQ(head->next->data, 20);
    ASSERT_NE(head->next->next, nullptr);
    EXPECT_EQ(head->next->next->data, 30);
    EXPECT_EQ(head->next->next->next, nullptr);
    
    free_list(head);
}

