#ifndef __BITSTREAM_H__
#define __BITSTREAM_H__

#include <stddef.h>
#include <inttypes.h>

#define UINT8_BIT_COUNT sizeof(uint8_t) * 8UL
#define UINT16_BIT_COUNT sizeof(uint16_t) * 8UL
#define UINT32_BIT_COUNT sizeof(uint32_t) * 8UL
#define UINT64_BIT_COUNT sizeof(uint64_t) * 8UL

typedef struct bitstream_t bitstream_t;

/// @brief Allocate new bitstream with room for init_size bytes
/// @param init_size size at initialization (bytes)
/// @return ptr to new bitstream
bitstream_t* bitstream_new(size_t init_size);

/// @brief Free bitstream
/// @param bs ptr to the bitstream
void free_bitstream(bitstream_t* bs);

/// @brief Write num_bits from a byte to the stream
/// @param bs ptr to the stream
/// @param data byte to write from
/// @param num_bits number of bits from data to write
void bitstream_write_8(bitstream_t *bs, uint8_t bits, uint8_t num_bits);

/// @brief Print the bitstream's bits, byte separated
/// @param bs ptr to the stream
void print_bitstream(bitstream_t *bs);

// TODO: print in hex format

#define bitstream_write_8(bs, data, num_bits) __bitstream_write_8(bs, data, num_bits)
#define bitstream_write_16(bs, data, num_bits) __bitstream_write_16(bs, data, num_bits)
#define bitstream_write_32(bs, data, num_bits) __bitstream_write_32(bs, data, num_bits)
#define bitstream_write_64(bs, data, num_bits) __bitstream_write_64(bs, data, num_bits)

/// @brief Write num_bits from data to the bitstream
/// @param bs ptr to the stream
/// @param data data to be written
/// @param num_bits number of bits to write
void __bitstream_write_8(bitstream_t *bs, uint8_t data, size_t num_bits);

/// @brief Write num_bits from data to the bitstream
/// @param bs ptr to the stream
/// @param data data to be written
/// @param num_bits number of bits to write
void __bitstream_write_16(bitstream_t *bs, uint16_t data, size_t num_bits);

/// @brief Write num_bits from data to the bitstream
/// @param bs ptr to the stream
/// @param data data to be written
/// @param num_bits number of bits to write
void __bitstream_write_32(bitstream_t *bs, uint32_t data, size_t num_bits);

/// @brief Write num_bits from data to the bitstream
/// @param bs ptr to the stream
/// @param data data to be written
/// @param num_bits number of bits to write
void __bitstream_write_64(bitstream_t *bs, uint64_t data, size_t num_bits);

/// @brief Get the size of the stream in bits
/// @param bs ptr to the stream
/// @return number of bits in stream
size_t bitstream_size(bitstream_t *bs);
#endif