#include <stdio.h>
#include "hashmap.h"

int
main(void)
{
    hashmap_t* map = hashmap_create_default(sizeof(int), sizeof(int));
    int key = 1;
    int value = 2;
    hashmap_set(map, &key, &value);

    int* retrieved_value = hashmap_get(map, &(int){2});
    if (retrieved_value != NULL) {
        printf("Retrieved value: %d\n", *retrieved_value);
    } else {
        printf("Key not found\n");
    }

    hashmap_destroy(map);
}
