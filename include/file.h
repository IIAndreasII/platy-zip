#ifndef __FILE_H__
#define __FILE_H__

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

#define FILE_APPEND "a"
#define FILE_WRITE "w"

int open_and_write_to_file(const char *filename, uint8_t *data, size_t size, const char *mode);
int write_to_file(FILE *file, uint8_t *data, size_t size);

#endif