/**
 * TODO:
 * - DEFLATE algorithm
 *      [ ] Huffman encoding/decoding
 *          [x] Construct huffman tree
 *          [x] Encode data
 *          [ ] Decode data
 *          [ ] Serialize/deserialize tree
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
    uint8_t *str = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";
    //char* str = "the letters are sorted by increasing frequency, and the least frequent two at each step are combined and reinserted into the list, and a partial tree is constructed";

    printf("Input:\n%s\n", str);

    size_t size = strlen(str);
    huffman_node_t* huff_tree = huffman_generate((uint8_t*)str, size);
    printf("Hufftree depth: %lu\n", huffman_height(huff_tree));

    huffman_enc_map_t *enc_map;
    hashmap_init(enc_map, sym_hash, sym_compare);
    huffman_generate_enc_map(huff_tree, enc_map);
    huffman_print_enc_map(enc_map);

    bitstream_t *stream = huffman_encode(enc_map, str, size);

    printf("Encoded stream:\n");
    print_bitstream(stream);
    print_bitstream_hex(stream);

    printf("Original size: %lu bits\nEncoded size: %lu bits\n", size * UINT8_BIT_COUNT, bitstream_size(stream));

    return 0;
}