#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {
    LIST_SUCCESS = 0,
    LIST_ERR_NULL,      // If ppHead is NULL
    LIST_ERR_ALLOC      // If malloc fails
} list_status_t;

typedef struct Node {
    int32_t data;
    struct Node* next;
} Node;

static Node* new_node(int32_t data, Node* next) {
    Node* n = malloc(sizeof(Node));
    if (n) {
        n->data = data;
        n->next = next;
    }
    return n;
}

list_status_t reverse_list(Node** ppHead) {
    if (ppHead == NULL) {
        return LIST_ERR_NULL;
    }

    Node* prev = NULL;
    Node* curr = *ppHead;
    Node* front;

    while (curr) {
        front = curr->next;
        curr->next = prev;
        prev = curr;
        curr = front; 
    }

    *ppHead = prev;
    return LIST_SUCCESS;
}

static void print_list(const Node* head) {
    const Node* temp = head;
    while (temp) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

#ifndef TESTING
int main() {
    Node* head = new_node(10, new_node(20, new_node(30, new_node(40, NULL))));

    printf("Original: ");
    print_list(head);

    reverse_list(&head);

    printf("Reversed: ");
    print_list(head);

    // Clean up
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
#endif
