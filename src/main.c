/**
 * TODO:
 * - DEFLATE algorithm
 *      [ ] Huffman encoding/decoding
 *          [x] Construct huffman tree
 *          [x] Encode data
 *          [ ] Serialize/deserialize tree
 *          [ ] Decode data
 *      [ ] Duplicate string elimination (LZxx)
 * - .ZIP compliancy
 *      [ ] Headers
 *          [ ] Local file header
 *          [ ] Data descriptor
 *          [ ] Central directory file header (CDHF)
 *          [ ] End of central directory record (EOCD)
 * - Unit testing
 *      [ ] Priority queue
 *      [ ] Bitstream
 *      [ ] Huffman
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "prio_queue.h"
#include "huffman.h"
#include "hashmap.h"

int main(int argc, char** argv)
{
    char* str = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";

    //char* str = "the letters are sorted by increasing frequency, and the least frequent two at each step are combined and reinserted into the list, and a partial tree is constructed";
    size_t size = strlen(str);
    huffman_node_t* huff_tree = huffman_generate((uint8_t*)str, size);

    huffman_enc_map_t *e;
    printf("Hufftree depth: %i\n", huffman_depth(huff_tree));

    printf("Encoding dict:\n");
    bitstream_t* stream = huffman_encode(huff_tree, (uint8_t*)str, size);

    printf("Input:\n%s\n", str);
    printf("Encoded stream:\n");
    print_bitstream(stream);
    print_bitstream_hex(stream);

    printf("Original size: %lu bits\nEncoded size: %lu bits\n", size * UINT8_BIT_COUNT, bitstream_size(stream));

    return 0;
}