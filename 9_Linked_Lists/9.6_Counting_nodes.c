#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>

typedef struct Node{
int32_t data;
struct Node* next;
}Node;

static Node* new_node(int32_t data, Node* next){
    Node* n = malloc(sizeof(Node));
    if(n){
        n->data = data;
        n->next = next;
    }
    return n; 
}

static void print_list(const Node* head) {
    const Node* temp = head;
    while (temp) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

size_t count_node(const Node* head){
    const Node* curr = head;
    size_t num = 0;
    while(curr)
    {
        num++;
        curr = curr->next;
    }
    return num;
}

#ifndef TESTING
int main()
{
    Node* head =  NULL;
    head = new_node(1,new_node(2,new_node(3,new_node(4,NULL))));
    print_list(head);
    printf("No of nodes = %zu\n",count_node(head));
    return 0;
}
#endif