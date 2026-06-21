#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

typedef struct Node{
    int32_t data;
    struct Node* next;
}Node;

static Node* create_list_from_array(const int32_t arr[], size_t size)
{
    bool free_all = 0;
    Node* head = NULL;
    Node** ppNext = &head;

    for(size_t i=0; i<size; i++)
    {
        Node* New = malloc(sizeof(Node));
        if(New == NULL)
        {
            free_all = 1;
            break;
        }
        
        New->data = arr[i];
        New->next = NULL;

        *ppNext = New;
        ppNext = &New->next;
    }

    if(free_all)
    {
        Node* temp = head;
        while(temp)
        {
            Node* del = temp;
            temp = temp->next;
            free(del);
        }
        return NULL;
    }

    return head;
}

void print_list(const Node* head)
{
    const Node* temp = head;
    while(temp)
    {
        printf("%d ",temp->data);
        temp = temp->next;
    }
}

void print_list_reverse(const Node* head)
{
    if (head == NULL) {
        return;
    }
    print_list_reverse(head->next);
    printf("%d ",head->data);
}

const Node* find_node(const Node* head, int32_t target)
{
    const Node* temp = head;
    while(temp)
    {
        if(temp->data==target)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

static void free_list(Node* head)
{
    Node* temp = head;
    while(temp)
    {
        Node* del = temp;
        temp = temp->next;
        free(del); 
    }
}

#ifndef TESTING
int main()
{
    const int32_t arr[] = {10, 20, 30, 40};
    Node* head = create_list_from_array(arr,ARRAY_SIZE(arr));
    print_list(head);
    printf("\n");
    print_list_reverse(head);
    printf("\n");
    const Node* found = find_node(head,30);
    if (found != NULL) {
        printf("%d \n", found->data);
    } else {
        printf("Not found\n");
    }
    free_list(head);
}
#endif