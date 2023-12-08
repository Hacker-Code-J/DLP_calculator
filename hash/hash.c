// hash.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// Hash function
unsigned int hash(char *key) {
    unsigned long int value = 0;
    unsigned int i = 0;
    unsigned int key_len = strlen(key);

    for (; i < key_len; ++i) {
        value = value * 37 + key[i];
    }

    value = value % TABLE_SIZE;

    return value;
}

// create_hash_table, insert, and search functions definitions...

hash_table *create_hash_table() {
    hash_table *new_table = malloc(sizeof(hash_table));
    for (int i = 0; i < TABLE_SIZE; i++) {
        new_table->buckets[i] = NULL;
    }
    return new_table;
}

void insert(hash_table *htable, char *key, int value) {
    unsigned int bucket_index = hash(key);
    node *new_node = malloc(sizeof(node));
    new_node->key = strdup(key);
    new_node->value = value;
    new_node->next = htable->buckets[bucket_index];
    htable->buckets[bucket_index] = new_node;
}

int search(hash_table *htable, char *key) {
    unsigned int bucket_index = hash(key);
    node *list = htable->buckets[bucket_index];
    while (list != NULL) {
        if (strcmp(list->key, key) == 0) {
            return list->value;
        }
        list = list->next;
    }
    return -1;
}
