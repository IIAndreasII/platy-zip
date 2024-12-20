#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <inttypes.h>
#include <stddef.h>

#include "bitstream.h"
#include "hashmap.h"


typedef struct huffman_node_t huffman_node_t;
typedef struct sym_code_t sym_code_t;

typedef HASHMAP(uint8_t, sym_code_t) huffman_enc_map_t;

/// @brief Print huffman tree
/// @param root root of tree
/// @param depth used to offset printing
void __print_huffman(huffman_node_t *root, size_t depth);

#define print_huffman(root)       \
    {                             \
        printf("root\n");         \
        __print_huffman(root, 0); \
    }

/// @brief Creates huffman tree from provided data
/// @param data the data
/// @param size size of data
/// @return root of huffman tree
huffman_node_t *huffman_generate(uint8_t *data, size_t size);

/// @brief Encode data using its huffman tree
/// @param root root of huffman tree
/// @param data data to encode
/// @param size size of data
/// @return encoded data
bitstream_t *huffman_encode(huffman_node_t *root, uint8_t *data, size_t size);

int huffman_depth(huffman_node_t *root);

#endif