#include<stdio.h>
#include<stdlib.h>
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

Node* new_node(int32_t data, Node* next) {
    Node* n = malloc(sizeof(Node));
    if (n) {
        n->data = data;
        n->next = next;
    }
    return n;
}

list_status_t delete_node(Node** ppHead, int32_t data) {
    if (ppHead == NULL) {
        return LIST_ERR_NULL;
    }

    Node** ppCurr = ppHead;
    while (*ppCurr != NULL) {
        if ((*ppCurr)->data == data) {
            Node* to_delete = *ppCurr;
            *ppCurr = (*ppCurr)->next;
            free(to_delete);
            // Do not advance ppCurr because *ppCurr now points to the next node
        } else {
            ppCurr = &((*ppCurr)->next);
        }
    }
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

    print_list(head);
    delete_node(&head, 10);
    print_list(head);

    // Clean up remaining nodes
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}
#endif