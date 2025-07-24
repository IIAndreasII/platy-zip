#include "file.h"


int open_and_write_to_file(const char *filename, uint8_t *data, size_t size, const char *mode)
{
    FILE *f = fopen(filename, mode);
    if (f == nullptr)
        return EOF;
    return write_to_file(f, data, size);
}

int write_to_file(FILE* file, uint8_t *data, size_t size)
{
    size_t bytes_written = fwrite(data, sizeof(uint8_t), size, file);

    if (bytes_written != size)
        return EOF;

    return fclose(file);
}