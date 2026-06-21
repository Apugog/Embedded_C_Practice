#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

typedef struct Node{
    int32_t data;
    struct Node* next;
} Node;

void check_malloc()
{
    Node* New = malloc(sizeof(Node));
    if(New == NULL)
    {
        fprintf(stderr,"Allocation Failed !!");
        return;
    }
    New->data = 42;
    New->next = NULL;
    printf("%d \n",New->data);
    free(New);
}

#ifndef TESTING
int main()
{
    check_malloc();
    return 0;
}
#endif