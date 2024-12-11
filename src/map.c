#include "map.h"

#include "list.h"

struct hash_map_t {
    size_t (*hash_func)(void*);
    size_t cap;

    list_t* chain[];
};

//hash_map_t* hash_map_new(size_t cap, size_t (*hash_func)(void*))


//void hash_map_insert(hash_map_t* h, )