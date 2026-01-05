#include <stdio.h>
#include "hashmap.h"
#include "hashmap_string.h"

int
main(void)
{
    hashmap_t* map = hashmap_create_string(sizeof(int));
    hashmap_set(map, "one", &(int){1});

    int* retrieved_value = hashmap_get(map, "one");
    if (retrieved_value != NULL) {
        printf("Retrieved value: %d\n", *retrieved_value);
    } else {
        printf("Key not found\n");
    }

    hashmap_destroy(map);
}
