#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef struct Node {
        int32_t data;
        struct Node* next;
    } Node;

    Node* create_list_from_array(const int32_t arr[], size_t size);
    void free_list(Node* head);
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
