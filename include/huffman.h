#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <inttypes.h>
#include <stddef.h>

#include "bitstream.h"
#include "hashmap.h"


typedef struct huffman_node_t huffman_node_t;
typedef struct sym_code_t sym_code_t;

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

typedef HASHMAP(uint8_t, sym_code_t) huffman_enc_map_t;

/// @brief Print huffman tree
/// @param root root of tree
/// @param depth used to offset printing
void __print_huffman(const huffman_node_t *root, const size_t depth);

#define print_huffman(root)       \
    {                             \
        printf("root\n");         \
        __print_huffman(root, 0); \
    }

/// @brief Creates huffman tree from provided data
/// @param data the data
/// @param size size of data
/// @return root of huffman tree
huffman_node_t *huffman_generate(const uint8_t *data, const size_t size);

/// @brief Generate an encoding map from a huffman tree
/// @param root root of the huffman tree
/// @param enc_map ptr to empty dict
void huffman_generate_enc_map(const huffman_node_t* root, huffman_enc_map_t* enc_map);

/// @brief Encode data using its huffman tree
/// @param bs ptr to bitstream
/// @param enc_map ptr to encoding map
/// @param data data to encode
/// @param size size
/// @return encoded data
void huffman_encode(bitstream_t *bs, const huffman_enc_map_t *enc_map, const uint8_t *data, const size_t size);

/// @brief Decode data using its huffman encoding map
/// @param enc_map root of huffman tree
/// @param bs bitstream of encoded data
/// @param buf ptr to buffer for decoded data
/// @return 0 if successful, -1 if not
uint8_t huffman_decode(huffman_node_t *root, bitstream_t *bs, uint8_t *buf, const size_t size);

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

/// @brief Free the huffman tree
/// @param root ptr to the root
void huffman_free(huffman_node_t* root);

/// @brief Free encoding map for huffman tree
/// @param enc_map ptr
void huffman_enc_map_free(huffman_enc_map_t *enc_map);

#endif