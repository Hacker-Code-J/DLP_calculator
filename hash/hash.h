// hash.h

#ifndef _HASH_H
#define _HASH_H

#define TABLE_SIZE 100

typedef struct node {
    char *key;
    int value;
    struct node *next;
} node;

typedef struct hash_table {
    node *buckets[TABLE_SIZE];
} hash_table;

// Function declarations
hash_table *create_hash_table();
void insert(hash_table *htable, char *key, int value);
int search(hash_table *htable, char *key);

#endif // _HASH_H