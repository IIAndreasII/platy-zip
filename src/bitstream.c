#include "bitstream.h"

#include <malloc.h>
#include <assert.h>


bitstream_t *bitstream_new(const size_t init_size)
{
    bitstream_t *bs = calloc(1, sizeof(bitstream_t));
    if (bs == nullptr)
        return nullptr;
    bs->stream = calloc(init_size, sizeof(uint8_t));
    if (bs->stream == nullptr)
        return nullptr;
    return bs;
}

void free_bitstream(bitstream_t *bs)
{
    free(bs->stream);
    free(bs);
}

void bitstream_write_8(bitstream_t *bs, const uint8_t data, const size_t num_bits)
{
    uint8_t space_left, bit_overlap, bits, left_byte_bits, right_byte_bits;
    // Can't write more bits than exist in a byte
    assert(num_bits <= UINT8_BIT_COUNT);

    // If we overlap into next byte
    if (bs->bit_offset + num_bits > UINT8_BIT_COUNT)
    {
        // space left in current byte
        space_left = UINT8_BIT_COUNT - bs->bit_offset;
        bit_overlap = num_bits - space_left;

        left_byte_bits = data >> bit_overlap;
        right_byte_bits = data << (UINT8_BIT_COUNT - bit_overlap);

        // Shift and OR to prepend bits
        bs->stream[bs->byte_offset] |= left_byte_bits;
        bs->byte_offset++;

        bs->stream[bs->byte_offset] |= right_byte_bits;
        bs->bit_offset = bit_overlap;
    }
    else
    {
        // shift to prepend bits
        bits = data << (UINT8_BIT_COUNT - num_bits - bs->bit_offset);
        bs->stream[bs->byte_offset] |= bits;
        bs->bit_offset += num_bits;
        if (bs->bit_offset >= UINT8_BIT_COUNT)
        {
            bs->bit_offset %= UINT8_BIT_COUNT;
            bs->byte_offset++;
        }
    }
    bs->size += num_bits;
}

void bitstream_write_16(bitstream_t *bs, const uint16_t bits, const size_t num_bits)
{
    uint8_t a;
    assert(num_bits <= UINT16_BIT_COUNT);

    if (num_bits <= UINT8_BIT_COUNT)
    {
        bitstream_write_8(bs, (const uint8_t)bits, num_bits);
        return;
    }

    a = (uint8_t)(bits >> (num_bits - UINT8_BIT_COUNT));
    bitstream_write_8(bs, a, UINT8_BIT_COUNT);
    bitstream_write_8(bs, (const uint8_t)bits, num_bits - UINT8_BIT_COUNT);
}

void bitstream_write_32(bitstream_t *bs, const uint32_t bits, const size_t num_bits)
{
    uint16_t a;
    assert(num_bits <= UINT32_BIT_COUNT);

    if (num_bits <= UINT16_BIT_COUNT)
    {
        bitstream_write_16(bs, (const uint16_t)bits, num_bits);
        return;
    }

    a = (uint16_t)(bits >> (num_bits - UINT16_BIT_COUNT));
    bitstream_write_16(bs, a, UINT16_BIT_COUNT);
    bitstream_write_16(bs, (const uint16_t)bits, num_bits - UINT16_BIT_COUNT);
}

void bitstream_write_64(bitstream_t *bs, const uint64_t bits, const size_t num_bits)
{
    uint32_t a;
    assert(num_bits <= UINT64_BIT_COUNT);

    if (num_bits <= UINT32_BIT_COUNT)
    {
        bitstream_write_32(bs, (const uint32_t)bits, num_bits);
        return;
    }

    a = (uint32_t)(bits >> (num_bits - UINT32_BIT_COUNT));
    bitstream_write_32(bs, a, UINT32_BIT_COUNT);
    bitstream_write_32(bs, (const uint16_t)bits, num_bits - UINT32_BIT_COUNT);
}

void print_bitstream(const bitstream_t *bs)
{
    size_t i;
    int j;

    for (i = 0; i < bs->byte_offset; i++)
    {
        for (j = UINT8_BIT_COUNT - 1; j >= 0; j--)
        {
            if ((bs->stream[i] >> j) & 1)
                printf("1");
            else
                printf("0");
        }
        printf(" ");
    }

    for (i = UINT8_BIT_COUNT - 1; i >= UINT8_BIT_COUNT - bs->bit_offset; i--)
        if ((bs->stream[bs->byte_offset] >> i) & 1)
            printf("1");
        else
            printf("0");
    printf("\n");
}

static uint8_t dec_to_hex(const uint8_t d)
{
    assert(d <= 15);

    switch (d)
    {
    case 10:
        return 'A';
    case 11:
        return 'B';
    case 12:
        return 'C';
    case 13:
        return 'D';
    case 14:
        return 'E';
    case 15:
        return 'F';
    default:
        return d + '0';
    }
}

void print_bitstream_hex(const bitstream_t *bs)
{
    size_t i;
    uint8_t l, r;
    for (i = 0; i < bs->byte_offset; i++)
    {
        l = bs->stream[i] >> (UINT8_BIT_COUNT / 2);
        r = bs->stream[i] & 0b00001111;
        printf("%c%c ", dec_to_hex(l), dec_to_hex(r));
    }

    if (bs->bit_offset > 0)
    {
        printf("[bit tail: ");
        for (i = UINT8_BIT_COUNT - 1; i >= UINT8_BIT_COUNT - bs->bit_offset; i--)
            if ((bs->stream[bs->byte_offset] >> i) & 1)
                printf("1");
            else
                printf("0");
        printf("]");
    }

    printf("\n");
}

size_t bitstream_size(const bitstream_t *bs)
{
    return bs->size;
}

size_t bitstream_byte_offset(const bitstream_t *bs)
{
    return bs->byte_offset;
}

size_t bitstream_bit_offset(const bitstream_t *bs)
{
    return bs->bit_offset;
}

uint8_t bitstream_read_bit(const bitstream_t *bs)
{
    uint8_t byte = bs->stream[bs->byte_offset];
    uint8_t bit = bs->bit_offset;

    return (byte >> (7 - bit)) & 1;
}

void bitstream_free(bitstream_t *bs)
{
    free(bs->stream);
    free(bs);
    bs = nullptr;
}
