#ifndef FORMAT_H
#define FORMAT_H

#include <stddef.h>
#include <stdint.h>

#include "bpe.h"

#define BPE_MAGIC "BPE"
#define BPE_MAGIC_SIZE 3
#define BPE_RULE_COUNT_BYTES 2
#define BPE_ORIGINAL_SIZE_BYTES 8
#define BPE_COMPRESSED_SIZE_BYTES 8
#define BPE_HEADER_SIZE                                                        \
  (BPE_MAGIC_SIZE + BPE_RULE_COUNT_BYTES + BPE_ORIGINAL_SIZE_BYTES +           \
   BPE_COMPRESSED_SIZE_BYTES)

typedef union {
  struct __attribute__((packed)) {
    char magic[BPE_MAGIC_SIZE];
    uint16_t rule_count;
    uint64_t original_size;
    uint64_t compressed_size;
  } fields;

  uint8_t bytes[BPE_HEADER_SIZE];
} BPEHeaderUnion;

typedef struct {
  BPEHeaderUnion header;
  const BPERule *rules;
  const uint8_t *compressed;
} BPEFile;

int write_bpe(const char *path, const BPEResult *result, size_t original_size);

int read_bpe(const char *path, BPEResult *result, size_t *original_size);

#endif
