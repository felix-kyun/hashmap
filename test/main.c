#include "helpers/int_int.h"
#include <stdio.h>

int main(void)
{
    hashmap_int_int_t* map = hashmap_create_int_int();

    hashmap_set_int_int(map, 1, 100);
    hashmap_set_int_int(map, 2, 200);

    printf("Key: 1, Value: %d\n", hashmap_get_int_int(map, 1));
    printf("Key: 2, Value: %d\n", hashmap_get_int_int(map, 2));

    hashmap_destroy_int_int(map);
}
