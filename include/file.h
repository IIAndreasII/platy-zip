#ifndef __FILE_H__
#define __FILE_H__

#include <stddef.h>
#include <inttypes.h>

#define FILE_APPEND "a"
#define FILE_WRITE "w"

int write_to_file(const char *filename, uint8_t *data, size_t size, const char *mode);

#endif