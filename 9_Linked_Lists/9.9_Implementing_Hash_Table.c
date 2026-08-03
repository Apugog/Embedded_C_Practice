#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Structure definitions for Hash Table with chaining.
 */
typedef struct HashNode {
    int32_t key;
    int32_t value;
    struct HashNode* next;
} HashNode;

typedef struct HashTable {
    HashNode** buckets;
    size_t capacity;
    size_t size;
} HashTable;

/**
 * Hash function to map a key to an index.
 */
static size_t hash_function(int32_t key, size_t capacity) {
    // Simple modulo hash (handles negative keys safely)
    int64_t safe_key = (int64_t)key;
    if (safe_key < 0) {
        safe_key = -safe_key;
    }
    return (size_t)(safe_key % capacity);
}

/**
 * Creates a Hash Table with the given capacity.
 */
HashTable* hash_table_create(size_t capacity) {
    if(capacity == 0){
        return NULL;
    }

    HashTable* hash= malloc(sizeof(HashTable));
    if(hash == NULL){
        return NULL;
    } 

    hash->buckets = calloc(capacity,sizeof(HashNode*));
    if(hash->buckets==NULL){
        free(hash);
        return NULL;
    }

    hash->capacity = capacity; 
    hash->size = 0;
        
    return hash;
}

/**
 * Inserts or updates a key-value pair in the Hash Table.
 * Returns true if successful, false otherwise.
 */
bool hash_table_insert(HashTable* table, int32_t key, int32_t value) {
    if(table==NULL){
        return false;
    }

    size_t hash_idx = hash_function(key,table->capacity);
    if(table->buckets[hash_idx]!=NULL)
    {
        HashNode* curr = table->buckets[hash_idx];
        while(curr != NULL)
        {
            if(curr->key==key){
                curr->value = value;
                return true;
            }
            curr = curr->next;
        }
    }
    
    HashNode* newNode = malloc(sizeof(HashNode));
    if(newNode==NULL){
        return false;
    }

    newNode->key = key;
    newNode->value = value;
    newNode->next = table->buckets[hash_idx];

    table->buckets[hash_idx] = newNode;
    table->size++;
    
    return true;
}

/**
 * Retrieves the value associated with the key.
 * Stores result in out_value. Returns true if key is found, false otherwise.
 */
bool hash_table_get(const HashTable* table, int32_t key, int32_t* out_value) {
    if(table == NULL || out_value == NULL){
        return false;
    }

    size_t hash_idx = hash_function(key, table->capacity);
    const HashNode* curr = table->buckets[hash_idx];

    while(curr!=NULL){
        if(curr->key==key){
            *out_value = curr->value;
            return true;
        }
        curr = curr->next;
    }        

    return false;
}

/**
 * Removes the key-value pair from the Hash Table.
 * Returns true if the key was found and removed, false otherwise.
 */
bool hash_table_remove(HashTable* table, int32_t key) {
    if(table==NULL){
        return false;
    }

    size_t hash_idx = hash_function(key, table->capacity);
    HashNode* curr = table->buckets[hash_idx];
    HashNode* prev = NULL;
    while(curr!=NULL)
    {
        if(curr->key==key){
            if(prev==NULL){
                table->buckets[hash_idx] = curr->next;
            } else {
                prev->next = curr->next;
            }

            free(curr);
            table->size--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
        
    return false;
}

/**
 * Frees all memory allocated for the Hash Table.
 */
void hash_table_free(HashTable* table) {
    if(table==NULL)
        return;
    for(size_t i = 0; i<table->capacity;i++)
    {
        HashNode* curr = table->buckets[i];
        while(curr!=NULL){
            HashNode* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

#ifndef TESTING
int main() {
    printf("Hash Table Implementation Sandbox\n");

    HashTable* table = hash_table_create(5);
    if (table == NULL) {
        printf("Failed to create table.\n");
        return 0;
    }

    hash_table_insert(table, 1, 10);
    hash_table_insert(table, 6, 60); // Collision if capacity is 5 (1 % 5 == 6 % 5)
    
    int32_t value;
    if (hash_table_get(table, 1, &value)) {
        printf("Found key 1: %d (Expected: 10)\n", value);
    }
    if (hash_table_get(table, 6, &value)) {
        printf("Found key 6: %d (Expected: 60)\n", value);
    }

    hash_table_remove(table, 1);
    if (!hash_table_get(table, 1, &value)) {
        printf("Key 1 removed successfully.\n");
    }

    hash_table_free(table);
    return 0;
}
#endif
