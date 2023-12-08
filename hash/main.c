// main.c

#include <stdio.h>
#include "hash.h"

int main() {
    hash_table *my_table = create_hash_table();

    insert(my_table, "name", 1);
    insert(my_table, "age", 30);

    printf("Search for 'name': %d\n", search(my_table, "name"));
    printf("Search for 'age': %d\n", search(my_table, "age"));

    return 0;
}
