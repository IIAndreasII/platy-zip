#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <inttypes.h>
#include <stddef.h>

typedef struct huffman_node_t huffman_node_t;

struct huffman_node_t {
    uint8_t symbol;
    double weight; // Probability of symbol occurrance
    // If these are null, we're a leaf
    huffman_node_t* left; // 0
    huffman_node_t* right; // 1
    size_t pq_i; // used for priority queue position function
};

/// @brief Print huffman tree
/// @param root root of tree
void print_huffman(huffman_node_t** root);

/// @brief Creates huffman tree from provided data
/// @param data the data
/// @param size size of data (bytes)
/// @return root of huffman tree
huffman_node_t* generate_huffman_tree(uint8_t* data, int size);

#endif