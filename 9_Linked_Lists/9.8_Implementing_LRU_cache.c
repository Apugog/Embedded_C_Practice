#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* 
 * Structure definitions for LRU Cache.
 * (Define your Node, Hash Entry, and Cache structs here)
 */

typedef struct Node {
    int32_t key;
    int32_t value;
    struct Node* prev;
    struct Node* next;
    struct Node* hnext; // Hash collision bucket next node
} Node;

typedef struct LRUCache {
    int32_t capacity;
    int32_t size;
    // Add hash table representation and head/tail pointers here
    Node* head;
    Node* tail;
    Node** bucket;
} LRUCache;

static size_t hashing_func(int32_t key, uint32_t capacity){
    int64_t safe_key = (int64_t)key;

    if(safe_key < 0)
    {
        safe_key = -safe_key;
    }

    return (size_t)(safe_key%capacity);
}

/**
 * Creates an LRU Cache with the given capacity.
 */
LRUCache* lru_cache_create(int32_t capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    LRUCache* hash = malloc(sizeof(LRUCache));
    if(hash == NULL){
        return NULL;
    }

    hash->bucket = calloc(capacity,sizeof(Node*));
    if(hash->bucket == NULL){
        free(hash);
        return NULL;
    }

    hash->capacity = capacity;
    hash->size = 0;
    hash->head = malloc(sizeof(Node));
    if(hash->head == NULL){
        free(hash->bucket);
        free(hash);
        return NULL;
    }
    hash->tail = malloc(sizeof(Node));
    if(hash->tail == NULL){
        free(hash->head);
        free(hash->bucket);
        free(hash);
        return NULL;
    }
    hash->head->prev = hash->tail->next = NULL;
    hash->head->next = hash->tail;
    hash->tail->prev = hash->head;

    return hash;
}

static void detach_node(Node* node){
    node->prev->next = node->next;
    node->next->prev = node->prev;
}

static void insert_at_mru(LRUCache* cache, Node* node){
    cache->tail->prev->next = node;
    node->prev = cache->tail->prev;
    node->next = cache->tail;
    cache->tail->prev = node;
}

static void remove_from_hash(LRUCache* cache, Node* node) {
    size_t idx = hashing_func(node->key, cache->capacity);
    Node** pp = &cache->bucket[idx];
    while (*pp != NULL) {
        if (*pp == node) {
            *pp = node->hnext; // Unlink node from hash bucket chain
            return;
        }
        pp = &((*pp)->hnext);
    }
}

/**
 * Retrieves the value associated with the key from the cache.
 * Returns -1 if the key is not present.
 */
int32_t lru_cache_get(LRUCache* cache, int32_t key) {
    if(cache == NULL){
        return -1;
    }

    size_t idx = hashing_func(key, cache->capacity);

    if(cache->bucket[idx]!=NULL){
        Node* temp = cache->bucket[idx]; 
        while(temp!=NULL){
            if(temp->key==key)
            {
                detach_node(temp);
                insert_at_mru(cache, temp);
                return temp->value;
            }
            temp = temp->hnext;
        }
    }
    
    return -1;
}

/**
 * Inserts or updates the key-value pair in the cache.
 * If the capacity is reached, evicts the least recently used item.
 */
void lru_cache_put(LRUCache* cache, int32_t key, int32_t value) {
    if(cache==NULL){
        return;
    }

    size_t hash_idx = hashing_func(key, cache->capacity);
    // insert a new node

    Node* curr = cache->bucket[hash_idx];
    while(curr!=NULL)
    {
        if(curr->key==key){
            curr->value = value;
            // detach_node curr node from list
            detach_node(curr);
            // insert before tail
            insert_at_mru(cache, curr);
            return;
        }
        curr=curr->hnext;
    }

    Node* new_node = malloc(sizeof(Node));
    if(new_node == NULL){
        return;
    }

    if(cache->size >= cache->capacity)
    {
        // evict last recently used
        // get the last node from after head
        Node* node_to_remove = cache->head->next;
        // detach_node node
        detach_node(node_to_remove);
        remove_from_hash(cache,node_to_remove);
        free(node_to_remove);
        // decrease size
        cache->size--;
    }

    new_node->key = key;
    new_node->value = value;
    new_node->hnext = cache->bucket[hash_idx];

    cache->bucket[hash_idx] = new_node;
    //insert new node at the tail
    insert_at_mru(cache, new_node);
    // increase size
    cache->size++;
}

/**
 * Frees all memory allocated for the cache.
 */
void lru_cache_free(LRUCache* cache) {
    if(cache==NULL){
        return;
    }

    // Traverse the doubly linked list and free every node (including head & tail sentinels)
    Node* curr = cache->head;
    while (curr != NULL) {
        Node* next_node = curr->next;
        free(curr);
        curr = next_node;
    }

    // Free the hash bucket array and the cache struct itself
    free(cache->bucket);
    free(cache);
}

#ifndef TESTING
int main() {
    printf("LRU Cache Implementation Sandbox\n");
    
    // Example test usage:
    LRUCache* cache = lru_cache_create(2);
    if (cache == NULL) {
        printf("Failed to create cache.\n");
        return 0;
    }
    
    lru_cache_put(cache, 1, 10);
    lru_cache_put(cache, 2, 20);
    printf("Get 1: %d (Expected: 10)\n", lru_cache_get(cache, 1));
    
    lru_cache_put(cache, 3, 30); // Evicts key 2
    printf("Get 2: %d (Expected: -1)\n", lru_cache_get(cache, 2));
    
    lru_cache_put(cache, 4, 40); // Evicts key 1
    printf("Get 1: %d (Expected: -1)\n", lru_cache_get(cache, 1));
    printf("Get 3: %d (Expected: 30)\n", lru_cache_get(cache, 3));
    printf("Get 4: %d (Expected: 40)\n", lru_cache_get(cache, 4));
    
    lru_cache_free(cache);
    return 0;
}
#endif
