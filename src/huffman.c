#include "huffman.h"

#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <limits.h>

#include "hashmap.h"
#include "prio_queue.h"
#include "bitstream.h"

#define NUM_SYMBOLS (UCHAR_MAX + 1)

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

static bool huffman_is_branch(const huffman_node_t * const node)
{
    return node->left != nullptr && node->right != nullptr;
}

void __print_huffman(const huffman_node_t *root, const size_t depth)
{
    size_t i;

    if (root == nullptr)
        return;

    if (huffman_is_branch(root))
    {
        printf("branch\n");
        for (i = 0; i < depth; i++)
            printf(".");
        printf("\n");
        __print_huffman(root->left, depth + 1);

        for (i = 0; i < depth; i++)
            printf(".");
        printf("\n");
        __print_huffman(root->right, depth + 1);
    }
    else
    {
        for (i = 0; i < depth; i++)
            printf(".");
        printf("'%c' - %f\n", root->symbol, root->weight);
    }
}

static huffman_node_t *huffman_node_new()
{
    huffman_node_t *hn = calloc(1, sizeof(huffman_node_t));
    if (hn == nullptr)
        return nullptr;

    hn->left = nullptr;
    hn->right = nullptr;
    return hn;
}

huffman_node_t *huffman_generate(const uint8_t *buf, const size_t size)
{
    double weight;
    huffman_node_t *l, *r, *hn, *root, *internal;
    size_t i;
    size_t freq[NUM_SYMBOLS] = {0};

    prio_queue_t *pq_huffman = pq_new(NUM_SYMBOLS, compare_huffman, position_huffman);

    // Count each symbol
    for (i = 0; i < size; i++)
        freq[buf[i]]++;

    // Calculate weights and create initial huffman nodes
    for (i = 0; i < NUM_SYMBOLS; i++)
        // Add node only if symbol occurs
        if (freq[i])
        {
            weight = (double)freq[i] / (double)size;
            hn = huffman_node_new();
            hn->symbol = (uint8_t)i;
            hn->weight = weight;
            pq_insert(pq_huffman, hn);
        }

    // Create huffman tree
    while (pq_size(pq_huffman) > 1)
    {
        l = (huffman_node_t *)pq_min(pq_huffman);
        r = (huffman_node_t *)pq_min(pq_huffman);

        internal = huffman_node_new();
        internal->left = l;
        internal->right = r;
        internal->weight = l->weight + r->weight;
        internal->is_branch = true;
        pq_insert(pq_huffman, internal);
    }

    root = (huffman_node_t *)pq_min(pq_huffman);
    free(pq_huffman);
    return root;
}

static void __huffman_generate_enc_map(huffman_enc_map_t *h, const huffman_node_t *root, sym_code_t key)
{
    uint64_t prev_code;
    sym_code_t *sc;

    if (root == nullptr)
        return;

    if (huffman_is_branch(root))
    {
        key.bit_len++;
        prev_code = key.code;
        key.code = prev_code << 1;
        __huffman_generate_enc_map(h, root->left, key);
        key.code = (prev_code << 1) + 1;
        __huffman_generate_enc_map(h, root->right, key);
    }
    else
    {
        sc = calloc(1, sizeof(sym_code_t));
        sc->bit_len = key.bit_len;
        sc->code = key.code;
        hashmap_put(h, &root->symbol, sc);
    }
}

void huffman_generate_enc_map(const huffman_node_t *root, huffman_enc_map_t *enc_map)
{
    sym_code_t key = {.bit_len = 0, .code = 0};
    __huffman_generate_enc_map(enc_map, root, key);
}

void huffman_encode(bitstream_t *bs, const huffman_enc_map_t *enc_map, const uint8_t *data, const size_t size)
{
    size_t s;
    sym_code_t *sc;

    for (s = 0; s < size; s++)
    {
        sc = hashmap_get(enc_map, &data[s]);
        bitstream_write_64(bs, sc->code, sc->bit_len);
    }
}

static uint8_t decode_symbol(huffman_node_t *root, bitstream_t *bs)
{
    uint8_t bit;

    if (!root->is_branch)
        return root->symbol;

    bit = bitstream_read_bit(bs);

    bs->bit_offset++;
    if (bs->bit_offset >= UINT8_BIT_COUNT)
    {
        bs->bit_offset %= UINT8_BIT_COUNT;
        bs->byte_offset++;
    }

    return bit ? decode_symbol(root->right, bs) : decode_symbol(root->left, bs);
}

uint8_t huffman_decode(huffman_node_t *root, bitstream_t *bs, uint8_t *buf, const size_t size)
{
    size_t i;
    bs->bit_offset = 0;
    bs->byte_offset = 0;
    for (i = 0; i < size; i++)
        buf[i] = decode_symbol(root, bs);

    return 0;
}

size_t huffman_height(huffman_node_t *root)
{
    size_t l;
    size_t r;

    if (root == nullptr)
        return -1;

    l = huffman_height(root->left);
    r = huffman_height(root->right);

    return (l > r ? l : r) + 1;
}

void huffman_print_enc_map(huffman_enc_map_t *enc_map)
{
    int i;
    const uint8_t *k;
    sym_code_t *v;
    hashmap_foreach(k, v, enc_map)
    {
        printf("key: %u, sym: '%c', code: ", *k, *k);
        for (i = v->bit_len - 1; i >= 0; i--)
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
    return *lhs - *rhs;
}

void huffman_free(huffman_node_t *root)
{
    if (root == nullptr)
        return;

    huffman_free(root->left);
    huffman_free(root->right);
    free(root);
}

void huffman_enc_map_free(huffman_enc_map_t *enc_map)
{
    const uint8_t *k;
    sym_code_t *v;
    hashmap_foreach(k, v, enc_map)
        free(v);

    hashmap_cleanup(enc_map);
    free(enc_map);
}
