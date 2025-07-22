/**
 * TODO:
 * - DEFLATE algorithm
 *      [ ] Huffman encoding/decoding
 *          [x] Construct huffman tree
 *          [x] Encode data
 *          [x] Decode data
 *          [ ] Serialize/deserialize tree
 *          [ ] Limit tree height to 18s
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

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "prio_queue.h"
#include "huffman.h"
#include "hashmap.h"

int main(int argc, char **argv)
{
    const char *str = "A_DEAD_DAD_CEDED_A_BAD_BABE_A_BEADED_ABACA_BED";
    // char* str = "the letters are sorted by increasing frequency, and the least frequent two at each step are combined and reinserted into the list, and a partial tree is constructed";

    printf("C std: %li\n", __STDC_VERSION__);
    printf("Input:\n%s\n", str);

    size_t size = strlen(str);
    printf("Data length: %lu\n", size);
    huffman_node_t *huff_tree = huffman_generate((uint8_t *)str, size);
    printf("Hufftree depth: %lu\n", huffman_height(huff_tree));

    huffman_enc_map_t *enc_map = calloc(1, sizeof(huffman_enc_map_t));
    hashmap_init(enc_map, sym_hash, sym_compare);
    huffman_generate_enc_map(huff_tree, enc_map);
    huffman_print_enc_map(enc_map);

    bitstream_t *stream = huffman_encode(enc_map, (uint8_t*)str, size);

    printf("Encoded stream:\n");
    print_bitstream(stream);
    print_bitstream_hex(stream);

    printf("Original size: %lu bits\nEncoded size: %lu bits\n", size * UINT8_BIT_COUNT, bitstream_size(stream));

    uint8_t *buf = calloc(size + 1, sizeof(uint8_t));
    buf[size] = '\0';
    huffman_decode(huff_tree, stream, buf, size);
    printf("Decoded string:\n%s\n", buf);


    huffman_free(huff_tree);
    bitstream_free(stream);
    huffman_enc_map_free(enc_map);
    free(buf);
    return 0;
}