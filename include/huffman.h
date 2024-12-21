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

/// @brief Generate an encoding map from a huffman tree
/// @param root root of the huffman tree
/// @param enc_map ptr to empty dict
void huffman_generate_enc_map(huffman_node_t* root, huffman_enc_map_t* enc_map);

/// @brief Encode data using its huffman tree
/// @param root root of huffman tree
/// @param data data to encode
/// @param size size of data
/// @return encoded data
bitstream_t *huffman_encode(huffman_enc_map_t *enc_map, uint8_t *data, size_t size);

/// @brief Get the depth of the tree
/// @param root root of huffman tree
/// @return depth
size_t huffman_height(huffman_node_t *root);

/// @brief Print huffman encoding map
/// @param enc_map ptr to the map
void huffman_print_enc_map(huffman_enc_map_t* enc_map);

/// @brief Default hash function for huffman_enc_map_t
/// @param data data to hash
/// @return hash
size_t sym_hash(const uint8_t *data);

/// @brief Default symbol compare for huffman_enc_map_t
/// @param lhs lhs
/// @param rhs rhs
/// @return 1 if lhs > rhs, -1 if lhs < rhs, 0 otherwise
int sym_compare(const uint8_t *lhs, const uint8_t *rhs);

#endif