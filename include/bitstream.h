#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

#include <stddef.h>
#include <inttypes.h>

#define UINT8_BIT_COUNT sizeof(uint8_t) * __CHAR_BIT__
#define UINT16_BIT_COUNT sizeof(uint16_t) * __CHAR_BIT__
#define UINT32_BIT_COUNT sizeof(uint32_t) * __CHAR_BIT__
#define UINT64_BIT_COUNT sizeof(uint64_t) * __CHAR_BIT__

typedef struct
{
    uint8_t *stream;
    size_t size; // in bits
    uint8_t bit_offset;
    size_t byte_offset;
} bitstream_t;

//struct bitstream_t bitstream_t;

/// @brief Allocate new bitstream with room for init_size bytes
/// @param init_size size at initialization (bytes)
/// @return ptr to new bitstream
bitstream_t* bitstream_new(size_t init_size);

/// @brief Free bitstream
/// @param bs ptr to the bitstream
void free_bitstream(bitstream_t* bs);

/// @brief Print the bitstream's bits, byte separated
/// @param bs ptr to the stream
void print_bitstream(const bitstream_t *bs);

/// @brief Print the bitstream in hexademicals, byte separated.
///        If the last byte is incomplete, print the tail in bits
/// @param bs ptr to the stream
void print_bitstream_hex(const bitstream_t* bs);

/// @brief Write num_bits from data to the bitstream
/// @param bs ptr to the stream
/// @param data data to be written
/// @param num_bits number of bits to write
void bitstream_write_8(bitstream_t *bs, const uint8_t data, const size_t num_bits);

/// @brief Write num_bits from data to the bitstream
/// @param bs ptr to the stream
/// @param data data to be written
/// @param num_bits number of bits to write
void bitstream_write_16(bitstream_t *bs, const uint16_t data, const size_t num_bits);

/// @brief Write num_bits from data to the bitstream
/// @param bs ptr to the stream
/// @param data data to be written
/// @param num_bits number of bits to write
void bitstream_write_32(bitstream_t *bs, const uint32_t data, const size_t num_bits);

/// @brief Write num_bits from data to the bitstream
/// @param bs ptr to the stream
/// @param data data to be written
/// @param num_bits number of bits to write
void bitstream_write_64(bitstream_t *bs, const uint64_t data, const size_t num_bits);

/// @brief Get the size of the stream in bits
/// @param bs ptr to the stream
/// @return number of bits in stream
size_t bitstream_size(const bitstream_t *bs);

/// @brief Get the byte offset of the stream
/// @param bs ptr to the stream
/// @return byte offset
size_t bitstream_byte_offset(const bitstream_t* bs);

/// @brief Get the bit offset for the current byte
/// @param bs ptr to the stream
/// @return bit offset
size_t bitstream_bit_offset(const bitstream_t *bs);

/// @brief Read the bit at current offset
/// @param bs ptr to the bitstream
/// @return value of bit
uint8_t bitstream_read_bit(const bitstream_t *bs);

/// @brief Free the bitstream and set pointer to nullptr
/// @param bs ptr to the stream
void bitstream_free(bitstream_t *bs);

#endif