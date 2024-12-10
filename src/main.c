/**
 * TODO:
 * - DEFLATE algorithm
 *      [ ] Huffman encoding/decoding
 *          [ ] Construct huffman tree
 *          [ ] Encode data
 *          [ ] Serialize/deserialize tree
 *          [ ] Decode data
 *      [ ] Duplicate string elimination (LZxx)
 * - .ZIP compliancy
 */

#include <stdio.h>

#include "prio_queue.h"

typedef struct {
    int a;
    size_t i;
} data_t;

static size_t* position(void *ptr)
{
    data_t* v = ptr;
    return &v->i;
}

static int compare_data(const void* _lhs, const void* _rhs)
{
    data_t* lhs = _lhs;
    data_t* rhs = _rhs;

    if (lhs->a == rhs->a)
        return 0;
    if (lhs->a > rhs->a)
        return -1;
    if (lhs->a > rhs->a)
        return 1;

    return 0;
}

int main(int argc, char** argv)
{
    generate_huffman_tree("AAABCC", 6);

    // prio_queue_t* pq = new_prio_queue(4, compare_data, position);

    // data_t a1 = { .a = 1};
    // data_t a2 = { .a = 2};
    // data_t a3 = { .a = 3};

    // pq_insert(pq, &a3);
    // pq_insert(pq, &a1);
    // pq_insert(pq, &a2);

    // for (int i = 0; i < 3; i++)
    // {
    //     int a = ((data_t*)pq_min(pq))->a;
    //     printf("%i", a);
    // }



    return 0;
}