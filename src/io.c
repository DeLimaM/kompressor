#include <stdio.h>
#include <stdlib.h>

#include "io.h"

uint8_t *read_file(const char *path, size_t *size)
{
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        perror("Failed to open file for reading");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size < 0)
    {
        perror("Failed to determine file size");
        fclose(file);
        return NULL;
    }

    uint8_t *buffer = malloc(file_size);
    if (!buffer)
    {
        perror("Failed to allocate memory for file");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, file_size, file);
    if (read_size != (size_t)file_size)
    {
        perror("Failed to read entire file");
        free(buffer);
        fclose(file);
        return NULL;
    }

    fclose(file);
    *size = read_size;
    return buffer;
}

int write_file(const char *path, const uint8_t *data, size_t size)
{
    FILE *file = fopen(path, "wb");
    if (!file)
    {
        perror("Failed to open file for writing");
        return -1;
    }

    size_t written_size = fwrite(data, 1, size, file);
    if (written_size != size)
    {
        perror("Failed to write entire file");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}