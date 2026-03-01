#ifndef BPE_H
#define BPE_H

#define MAX_RULES 256

#include <stddef.h>
#include <stdint.h>

typedef struct
{
  uint8_t symbol;
  uint8_t pair[2];
} BPERule;

typedef struct
{
  BPERule *rules;
  int rule_count;
  uint8_t *data;
  size_t size;
} BPEResult;

BPEResult *bpe_compress(const uint8_t *data, size_t size);
uint8_t *bpe_decompress(const BPEResult *compressed, size_t *out_size);
void bpe_free(BPEResult *result);

#endif