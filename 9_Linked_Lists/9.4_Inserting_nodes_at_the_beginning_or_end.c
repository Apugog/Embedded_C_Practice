#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {
    LIST_SUCCESS = 0,
    LIST_ERR_NULL,      // If ppHead is NULL
    LIST_ERR_ALLOC      // If malloc fails
} list_status_t;

typedef struct Node{
    int32_t data;
    struct Node* next;
}Node;

list_status_t insert_beg(Node** ppHead,int32_t data)
{
    if(ppHead==NULL)
        return LIST_ERR_NULL;

    Node* New = malloc(sizeof(Node));
    if(New == NULL)
        return LIST_ERR_ALLOC;

    New->data = data;
    New->next = *ppHead;
    *ppHead = New;

    return LIST_SUCCESS;
}

list_status_t insert_end(Node** ppHead,int32_t data)
{
    if(ppHead==NULL)
        return LIST_ERR_NULL;

    Node* New = malloc(sizeof(Node));
    if(New == NULL)
        return LIST_ERR_ALLOC;

    New->data = data;
    New->next = NULL;

    Node** ppCurr = ppHead;
    /*using double pointers instead of simply NULL check head
    note address to which NULL is stored is valid we exploit that property*/
    while (*ppCurr != NULL)
    {
        ppCurr = &((*ppCurr)->next);
    }

    *ppCurr = New;

    return LIST_SUCCESS;
}

static void print_list(const Node* head)
{
    const Node* temp = head;
    while(temp)
    {
        printf("%d ",temp->data);
        temp = temp->next;
    }
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
    Node* Head = NULL;

    for(int32_t i=1; i<9; i++)
    {
        insert_beg(&Head, i);
    }

    Node* Head1 = NULL;

    for(int32_t i=1; i<9; i++)
    {
        insert_end(&Head1, i);
    }

    print_list(Head);
    printf("\n");
    print_list(Head1);

    free_list(Head);
    free_list(Head1);

    return 0;
}
#endif