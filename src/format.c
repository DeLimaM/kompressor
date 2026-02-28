#include <stdlib.h>
#include <string.h>

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
    if (!buffer)
        return -1;

    generate_bpe(&file, result, original_size);
    memcpy(buffer, file.header.bytes, BPE_HEADER_SIZE);
    memcpy(buffer + BPE_HEADER_SIZE, file.rules, result->rule_count * sizeof(BPERule));
    memcpy(buffer + BPE_HEADER_SIZE + result->rule_count * sizeof(BPERule), file.compressed, result->size);

    int res = write_file(path, buffer, total_size);
    free(buffer);
    return res;
}