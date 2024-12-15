#include "bitstream.h"

#include <malloc.h>
#include <assert.h>

struct bitstream_t
{
    uint8_t *stream;
    size_t size; // in bits
    uint8_t bit_offset;
    size_t byte_offset;
};

bitstream_t *bitstream_new(size_t init_size)
{
    bitstream_t *bs = calloc(1, sizeof(bitstream_t));
    bs->stream = malloc(init_size);
    return bs;
}

void free_bitstream(bitstream_t *bs)
{
    free(bs->stream);
    free(bs);
}

void __bitstream_write_8(bitstream_t *bs, uint8_t data, size_t num_bits)
{
    // Can't write more bits than exist in a byte
    assert(num_bits <= UINT8_BIT_COUNT);

    // If we overlap into next byte
    if (bs->bit_offset + num_bits > UINT8_BIT_COUNT)
    {
        // space left in current byte
        uint8_t space_left = UINT8_BIT_COUNT - bs->bit_offset;
        uint8_t bit_overlap = num_bits - space_left;

        uint8_t left_byte_bits = data >> bit_overlap;
        uint8_t right_byte_bits = data << (UINT8_BIT_COUNT - bit_overlap);

        // Shift and OR to prepend bits
        bs->stream[bs->byte_offset] |= left_byte_bits;
        bs->byte_offset++;

        bs->stream[bs->byte_offset] |= right_byte_bits;
        bs->bit_offset = bit_overlap;
    }
    else
    {
        // shift to prepend bits
        uint8_t bits = data << (UINT8_BIT_COUNT - num_bits - bs->bit_offset);
        bs->stream[bs->byte_offset] |= bits;
        bs->bit_offset += num_bits;
    }
    bs->size += num_bits;
}

void __bitstream_write_16(bitstream_t *bs, uint16_t bits, size_t num_bits)
{
    assert(num_bits <= UINT16_BIT_COUNT);

    if (num_bits <= UINT8_BIT_COUNT)
    {
        __bitstream_write_8(bs, (uint8_t)bits, num_bits);
        return;
    }

    uint8_t b1 = (uint8_t)(bits >> (num_bits - UINT8_BIT_COUNT));
    __bitstream_write_8(bs, b1, UINT8_BIT_COUNT);
    __bitstream_write_8(bs, (uint8_t)bits, num_bits - UINT8_BIT_COUNT);
}

void __bitstream_write_32(bitstream_t *bs, uint32_t bits, size_t num_bits)
{
    assert(num_bits <= UINT32_BIT_COUNT);

    if (num_bits <= UINT16_BIT_COUNT)
    {
        __bitstream_write_16(bs, (uint16_t)bits, num_bits);
        return;
    }

    uint16_t a = (uint16_t)(bits >> (num_bits - UINT16_BIT_COUNT));
    __bitstream_write_16(bs, a, UINT16_BIT_COUNT);
    __bitstream_write_16(bs, (uint16_t)bits, num_bits - UINT16_BIT_COUNT);
}

void __bitstream_write_64(bitstream_t *bs, uint64_t bits, size_t num_bits)
{
    assert(num_bits <= UINT64_BIT_COUNT);

    if (num_bits <= UINT32_BIT_COUNT)
    {
        __bitstream_write_32(bs, (uint32_t)bits, num_bits);
        return;
    }

    uint32_t a = (uint32_t)(bits >> (num_bits - UINT32_BIT_COUNT));
    __bitstream_write_32(bs, a, UINT32_BIT_COUNT);
    __bitstream_write_32(bs, (uint16_t)bits, num_bits - UINT32_BIT_COUNT);
}

void print_bitstream(bitstream_t *bs)
{
    for (size_t i = 0; i < bs->byte_offset; i++)
    {
        for (int j = UINT8_BIT_COUNT - 1; j >= 0; j--)
        {
            if ((bs->stream[i] >> j) & 1)
                printf("1");
            else
                printf("0");
        }
        printf(" ");
    }

    for (int i = UINT8_BIT_COUNT - 1; i >= UINT8_BIT_COUNT - bs->bit_offset; i--)
        if ((bs->stream[bs->byte_offset] >> i) & 1)
            printf("1");
        else
            printf("0");
    printf("\n");
}

size_t bitstream_size(bitstream_t *bs)
{
    return bs->size;
}