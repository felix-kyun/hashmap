#include "types/string_int.h"
#include <stdio.h>

int main(void)
{
    hashmap_string_int_t* map = hashmap_create_string_int();

    hashmap_set_string_int(map, "one", 1);
    hashmap_set_string_int(map, "two", 2);

    printf("one: %d\n", hashmap_get_string_int(map, "one"));
    printf("two: %d\n", hashmap_get_string_int(map, "two"));
    printf("has three: %d\n", hashmap_get_string_int(map, "three"));

    hashmap_destroy_string_int(map);
    return 0;
}
