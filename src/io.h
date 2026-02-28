#ifndef IO_H
#define IO_H

#include <stddef.h>
#include <stdint.h>

uint8_t *read_file(const char *path, size_t *size);
int write_file(const char *path, const uint8_t *data, size_t size);

#endif