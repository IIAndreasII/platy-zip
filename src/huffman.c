#include "huffman.h"

#include <malloc.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>

#include "hashmap.h"
#include "prio_queue.h"

#define NUM_SYMBOLS 256

struct huffman_node_t
{
    uint8_t symbol;
    double weight; // Probability of symbol occurrance
    huffman_node_t *left;  // 0
    huffman_node_t *right; // 1
    bool is_branch;
    size_t pq_i; // used for priority queue position function
};

// Prioritize node with least weight
static int compare_huffman(const void *_lhs, const void *_rhs)
{
    const huffman_node_t *lhs = (const huffman_node_t *)_lhs;
    const huffman_node_t *rhs = (const huffman_node_t *)_rhs;

    if (lhs->weight == rhs->weight)
        return 0;
    if (lhs->weight > rhs->weight)
        return 1;
    if (lhs->weight < rhs->weight)
        return -1;

    return 0;
}

// Position function for huffman priority queue
static size_t *position_huffman(void *_ptr)
{
    huffman_node_t *ptr = (huffman_node_t *)_ptr;
    return &ptr->pq_i;
}

void __print_huffman(huffman_node_t *root, size_t depth)
{
    if (root == NULL)
        return;

    if (root->is_branch)
    {
        //printf("branch\n");
        // for (size_t i = 0; i < depth; i++)
        //     printf(".");
        // printf("\n");
        __print_huffman(root->left, depth + 1);

        // for (size_t i = 0; i < depth; i++)
        //     printf(".");
        // printf("\n");
        __print_huffman(root->right, depth + 1);
    }
    else
    {
        for (size_t i = 0; i < depth; i++)
            printf(".");

        printf("'%c' - %f\n", root->symbol, root->weight);
    }
}



huffman_node_t *huffman_generate(uint8_t *buf, size_t size)
{
    size_t freq[NUM_SYMBOLS];
    double prob[NUM_SYMBOLS];
    memset(freq, 0, NUM_SYMBOLS * sizeof(size_t));
    memset(prob, 0, NUM_SYMBOLS * sizeof(double));

    prio_queue_t *pq_huffman = pq_new(NUM_SYMBOLS, compare_huffman, position_huffman);

    // Count each symbol
    for (size_t i = 0; i < size; i++)
        freq[buf[i]]++;

    // Calculate weights and create initial huffman nodes
    for (size_t i = 0; i < NUM_SYMBOLS; i++)
    {
        double weight = (double)freq[i] / (double)size;
        prob[i] = weight;
        huffman_node_t *hn = calloc(1, sizeof(huffman_node_t));

        // Add node only if symbol occurs
        if (freq[i])
        {
            // printf("%c, %f\n", (uint8_t)i, weight);
            hn->symbol = (uint8_t)i;
            hn->weight = freq[i];
            pq_insert(pq_huffman, hn);
        }
    }

    // Next step: create huffman tree :)
    while (pq_size(pq_huffman) > 1)
    {
        huffman_node_t *l = (huffman_node_t *)pq_min(pq_huffman);
        huffman_node_t *r = (huffman_node_t *)pq_min(pq_huffman);

        huffman_node_t *internal = malloc(sizeof(huffman_node_t));
        internal->left = l;
        internal->right = r;
        internal->weight = l->weight + r->weight;
        internal->is_branch = true;
        pq_insert(pq_huffman, internal);
    }

    huffman_node_t *root = (huffman_node_t *)pq_min(pq_huffman);
    return root;
}

int sym_compare(void *lhs, void *rhs)
{
    uint8_t l = *(uint8_t*)lhs;
    uint8_t r = *(uint8_t*)rhs;

    if (l == r)
        return 0;
    else if (l > r)
        return 1;
    else if (l < r)
        return -1;

    return 0;
}

typedef struct {
    uint64_t code;
    size_t bit_len;
} sym_code_t;

void huffman_encode_table(HASHMAP(uint8_t, sym_code_t)* h, huffman_node_t* root, sym_code_t key)
{
    if (root == NULL)
        return;


    if (root->is_branch)
    {
        key.bit_len++;
        uint64_t prev_code = key.code;
        key.code = (prev_code << 1);
        huffman_encode_table(h, root->left, key);
        key.code = ((prev_code) << 1) + 1;
        huffman_encode_table(h, root->right, key);
    }
    else
    {
        sym_code_t* sc = malloc(sizeof(sym_code_t));
        sc->bit_len = key.bit_len;
        sc->code = key.code;
        hashmap_put(h, &root->symbol, sc);
    }
}

uint8_t* huffman_encode(huffman_node_t* root, uint8_t* data, size_t size)
{
    HASHMAP(uint8_t, sym_code_t) enc_map;
    hashmap_init(&enc_map, hashmap_hash_string, sym_compare);

    sym_code_t key = { .bit_len = 0, .code = 0};

    huffman_encode_table(&enc_map, root, key);

    sym_code_t* v;
    uint8_t* k;
    hashmap_foreach(k, v, &enc_map) {
        printf("sym: '%c', code: ", *k);
        for (int i = v->bit_len - 1; i >= 0 ; i--)
            if ((v->code >> i) & 1)
                printf("1");
            else
                printf("0");
        printf(", bit_len: %li\n", v->bit_len);
    }
}
