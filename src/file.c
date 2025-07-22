#include "file.h"

#include <stdio.h>


int write_to_file(const char *filename, uint8_t *data, size_t size, const char *mode)
{
    size_t bytes_written;
    FILE *f = fopen(filename, mode);

    bytes_written = fwrite(data, sizeof(uint8_t), size, f);

    if (bytes_written != size)
        return EOF;

    return fclose(f);
}