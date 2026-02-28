#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "format.h"
#include "io.h"

int generate_bpe(BPEFile *file, const BPEResult *result, size_t original_size)
{
    for (int i = 0; i < BPE_MAGIC_SIZE; i++)
    {
        file->header.fields.magic[i] = BPE_MAGIC[i];
    }

    file->header.fields.rule_count = result->rule_count;
    file->header.fields.original_size = original_size;
    file->header.fields.compressed_size = result->size;
    file->rules = result->rules;
    file->compressed = result->data;

    return 0;
}

int write_bpe(const char *path, const BPEResult *result, size_t original_size)
{
    BPEFile file;
    size_t total_size = BPE_HEADER_SIZE + result->rule_count * sizeof(BPERule) + result->size;
    uint8_t *buffer = malloc(total_size);

    generate_bpe(&file, result, original_size);
    memcpy(buffer, file.header.bytes, BPE_HEADER_SIZE);
    memcpy(buffer + BPE_HEADER_SIZE, file.rules, result->rule_count * sizeof(BPERule));
    memcpy(buffer + BPE_HEADER_SIZE + result->rule_count * sizeof(BPERule), file.compressed, result->size);

    int res = write_file(path, buffer, total_size);
    free(buffer);
    return res;
}

int read_bpe(const char *path, BPEResult *result, size_t *original_size)
{
    size_t file_size;
    uint8_t *data = read_file(path, &file_size);

    BPEHeaderUnion header;
    memcpy(header.bytes, data, BPE_HEADER_SIZE);

    if (memcmp(header.fields.magic, BPE_MAGIC, BPE_MAGIC_SIZE) != 0)
    {
        printf("Invalid magic\n");
        free(data);
        return -1;
    }

    uint16_t rule_count = header.fields.rule_count;
    uint64_t original_sz = header.fields.original_size;
    uint64_t compressed_sz = header.fields.compressed_size;

    result->rule_count = rule_count;
    memcpy(result->rules, data + BPE_HEADER_SIZE, rule_count * sizeof(BPERule));
    result->data = malloc(compressed_sz);
    memcpy(result->data, data + BPE_HEADER_SIZE + rule_count * sizeof(BPERule), compressed_sz);
    result->size = compressed_sz;

    *original_size = original_sz;

    free(data);
    return 0;
}