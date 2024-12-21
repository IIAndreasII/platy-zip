#include "huffman.h"

#include <malloc.h>
#include <memory.h>
#include <stdio.h>

#include "hashmap.h"
#include "prio_queue.h"
#include "bitstream.h"

#define NUM_SYMBOLS 256

struct sym_code_t
{
    uint64_t code;
    size_t bit_len;
};

struct huffman_node_t
{
    uint8_t symbol;
    double weight;         // Probability of symbol occurrance
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

static bool huffman_is_branch(huffman_node_t* node)
{
    return node->left != nullptr && node->right != nullptr;
}

void __print_huffman(huffman_node_t *root, size_t depth)
{
    if (root == nullptr)
        return;

    if (huffman_is_branch(root))
    {
        printf("branch\n");
        for (size_t i = 0; i < depth; i++)
            printf(".");
        printf("\n");
        __print_huffman(root->left, depth + 1);

        for (size_t i = 0; i < depth; i++)
            printf(".");
        printf("\n");
        __print_huffman(root->right, depth + 1);
    }
    else
    {
        for (size_t i = 0; i < depth; i++)
            printf(".");
        printf("'%c' - %f\n", root->symbol, root->weight);
    }
}

static huffman_node_t *huffman_node_new()
{
    huffman_node_t *hn = calloc(1, sizeof(huffman_node_t));
    hn->left = nullptr;
    hn->right = nullptr;
    return hn;
}

huffman_node_t *huffman_generate(uint8_t *buf, size_t size)
{
    size_t freq[NUM_SYMBOLS] = {0};

    prio_queue_t *pq_huffman = pq_new(NUM_SYMBOLS, compare_huffman, position_huffman);

    // Count each symbol
    for (size_t i = 0; i < size; i++)
        freq[buf[i]]++;

    // Calculate weights and create initial huffman nodes
    for (size_t i = 0; i < NUM_SYMBOLS; i++)
    {
        double weight = (double)freq[i] / (double)size;
        huffman_node_t *hn = huffman_node_new();

        // Add node only if symbol occurs
        if (freq[i])
        {
            hn->symbol = (uint8_t)i;
            hn->weight = weight;
            pq_insert(pq_huffman, hn);
        }
    }

    // Next step: create huffman tree :)
    while (pq_size(pq_huffman) > 1)
    {
        huffman_node_t *l = (huffman_node_t *)pq_min(pq_huffman);
        huffman_node_t *r = (huffman_node_t *)pq_min(pq_huffman);

        huffman_node_t *internal = huffman_node_new();
        internal->left = l;
        internal->right = r;
        internal->weight = l->weight + r->weight;
        internal->is_branch = true;
        pq_insert(pq_huffman, internal);
    }

    huffman_node_t *root = (huffman_node_t *)pq_min(pq_huffman);
    return root;
}

static void __huffman_generate_enc_map(huffman_enc_map_t *h, huffman_node_t *root, sym_code_t key)
{
    if (root == nullptr)
        return;

    if (huffman_is_branch(root))
    {
        key.bit_len++;
        uint64_t prev_code = key.code;
        key.code = (prev_code << 1);
        __huffman_generate_enc_map(h, root->left, key);
        key.code = ((prev_code) << 1) + 1;
        __huffman_generate_enc_map(h, root->right, key);
    }
    else
    {
        sym_code_t *sc = malloc(sizeof(sym_code_t));
        sc->bit_len = key.bit_len;
        sc->code = key.code;
        hashmap_put(h, &root->symbol, sc);
    }
}

void huffman_generate_enc_map(huffman_node_t *root, huffman_enc_map_t *enc_map)
{
    sym_code_t key = {.bit_len = 0, .code = 0};

    __huffman_generate_enc_map(enc_map, root, key);
}

bitstream_t *huffman_encode(huffman_enc_map_t *enc_map, uint8_t *data, size_t size)
{
    bitstream_t *bs = bitstream_new(size);
    sym_code_t *sc;
    for (size_t s = 0; s < size; s++)
    {
        sc = hashmap_get(enc_map, &data[s]);
        bitstream_write_64(bs, sc->code, sc->bit_len);
    }

    return bs;
}

size_t huffman_height(huffman_node_t *root)
{
    if (root == nullptr)
        return -1;

    size_t l = huffman_height(root->left);
    size_t r = huffman_height(root->right);

    return (l > r ? l : r) + 1;
}

void huffman_print_enc_map(huffman_enc_map_t *enc_map)
{
    const uint8_t *k;
    sym_code_t *v;
    hashmap_foreach(k, v, enc_map)
    {
        printf("key: %u, sym: '%c', code: ", *k, *k);
        for (int i = v->bit_len - 1; i >= 0; i--)
            if ((v->code >> i) & 1)
                printf("1");
            else
                printf("0");
        printf(", bit_len: %li\n", v->bit_len);
    }
}

size_t sym_hash(const uint8_t *data)
{
    return *data;
}

int sym_compare(const uint8_t *lhs, const uint8_t *rhs)
{
    if (*lhs == *rhs)
        return 0;
    else if (*lhs > *rhs)
        return 1;
    else
        return -1;
}