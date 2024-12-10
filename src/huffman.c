#include "huffman.h"

#include <malloc.h>
#include <memory.h>
#include <stdio.h>

#include "prio_queue.h"

#define NUM_SYMBOLS 256

// Prioritize node with greatest weight
static int compare_huffman(const void *_lhs, const void *_rhs)
{
    const huffman_node_t *lhs = (const huffman_node_t *)_lhs;
    const huffman_node_t *rhs = (const huffman_node_t *)_rhs;

    // printf("(lhs %c, rhs %c)\n", lhs->symbol, rhs->symbol);

    if (lhs->weight == rhs->weight)
        return 0;
    if (lhs->weight > rhs->weight)
        return -1;
    if (lhs->weight < rhs->weight)
        return 1;

    return 0;
}

// Position function for huffman priority queue
static size_t *position_huffman(void *_ptr)
{
    huffman_node_t *ptr = (huffman_node_t *)_ptr;
    return &ptr->pq_i;
}

// TODO: complete this
void print_huffman(huffman_node_t **root)
{
    printf("{Symbol:%c, weight:%f}", (*root)->symbol, (*root)->weight);
}

huffman_node_t *generate_huffman_tree(uint8_t *buf, int size)
{
    size_t freq[NUM_SYMBOLS];
    double prob[NUM_SYMBOLS];
    memset(freq, 0, NUM_SYMBOLS * sizeof(size_t));
    memset(prob, 0, NUM_SYMBOLS * sizeof(double));

    prio_queue_t *pq_huffman = new_prio_queue(NUM_SYMBOLS, compare_huffman, position_huffman);

    // Count each symbol
    for (int i = 0; i < size; i++)
        freq[buf[i]]++;

    // Calculate weights and create initial huffman nodes
    for (size_t i = 0; i < NUM_SYMBOLS; i++)
    {
        double weight = (double)freq[i] / (double)size;
        prob[i] = weight;
        huffman_node_t *hn = malloc(sizeof(huffman_node_t));

        // Add node only if symbol occurs
        if (freq[i]) {
            //printf("%c\n", (uint8_t)i);
            hn->symbol = (uint8_t)i;
            hn->weight = weight;
            pq_insert(pq_huffman, hn);
        }
    }

    // Next step: create huffman tree :)

    //printf("size: %li\n", pq_size(pq_huffman));

    // int s = pq_size(pq_huffman);
    // for (int i = 0; i < s; i++)
    // {
    //     char a = ((huffman_node_t *)pq_min(pq_huffman))->symbol;
    //     printf("%c\n", a);
    // }

    // if (1)
    // {
    //     int sum = 0;
    //     double probsum = 0;
    //     for (size_t i = 0; i < NUM_SYMBOLS; i++)
    //     {
    //         prob[i] = (double)freq[i] / (double)size;
    //         probsum += prob[i];
    //         sum += freq[i];
    //         if (freq[i])
    //             printf("%c : %li, %f\n", (char)i, freq[i], prob[i]);
    //     }
    //     printf("sum: %i\n", sum);
    //     printf("probsum: %f\n", probsum);
    // }

    return NULL;
}
