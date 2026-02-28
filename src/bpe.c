#include <stdlib.h>
#include <string.h>

#include "bpe.h"

static void find_used_bytes(const uint8_t *data, size_t size, int used[MAX_RULES])
{
    memset(used, 0, MAX_RULES * sizeof(int));
    for (size_t i = 0; i < size; i++)
        used[data[i]] = 1;
}

static void count_pairs(const uint8_t *data, size_t size,
                        int pair_counts[MAX_RULES][MAX_RULES])
{
    memset(pair_counts, 0, MAX_RULES * MAX_RULES * sizeof(int));
    for (size_t i = 0; i + 1 < size; i++)
        pair_counts[data[i]][data[i + 1]]++;
}

static int find_best_pair(int pair_counts[MAX_RULES][MAX_RULES],
                          uint8_t *best_a, uint8_t *best_b)
{
    int max_count = 0;
    for (int a = 0; a < MAX_RULES; a++)
    {
        for (int b = 0; b < MAX_RULES; b++)
        {
            if (pair_counts[a][b] > max_count)
            {
                max_count = pair_counts[a][b];
                *best_a = (uint8_t)a;
                *best_b = (uint8_t)b;
            }
        }
    }
    return max_count;
}

static size_t replace_pair(uint8_t *data, size_t size,
                           uint8_t a, uint8_t b, uint8_t new_sym)
{
    size_t j = 0;
    for (size_t i = 0; i < size;)
    {
        if (i + 1 < size && data[i] == a && data[i + 1] == b)
        {
            data[j++] = new_sym;
            i += 2;
        }
        else
        {
            data[j++] = data[i++];
        }
    }
    return j;
}

BPEResult *bpe_compress(const uint8_t *data, size_t size)
{
    BPEResult *result = malloc(sizeof(BPEResult));
    if (!result)
        return NULL;

    uint8_t *temp_data = malloc(size);
    if (!temp_data)
    {
        free(result);
        return NULL;
    }
    memcpy(temp_data, data, size);

    int used[MAX_RULES];
    int reserved[MAX_RULES];
    int pair_counts[MAX_RULES][MAX_RULES];
    size_t current_size = size;
    int rule_count = 0;

    memset(reserved, 0, sizeof(reserved));

    while (rule_count < MAX_RULES)
    {
        find_used_bytes(temp_data, current_size, used);
        count_pairs(temp_data, current_size, pair_counts);

        uint8_t a, b;
        int count = find_best_pair(pair_counts, &a, &b);
        if (count < 2)
            break;

        /* Find a free symbol (not used in data AND not reserved by a rule) */
        int sym = -1;
        for (int s = 0; s < MAX_RULES; s++)
        {
            if (!used[s] && !reserved[s])
            {
                sym = s;
                break;
            }
        }
        if (sym == -1)
            break;

        reserved[sym] = 1;
        result->rules[rule_count] = (BPERule){.symbol = (uint8_t)sym, .pair = {a, b}};
        rule_count++;
        current_size = replace_pair(temp_data, current_size, a, b, (uint8_t)sym);
    }

    result->rule_count = rule_count;
    result->data = malloc(current_size);
    if (!result->data)
    {
        free(temp_data);
        free(result);
        return NULL;
    }
    memcpy(result->data, temp_data, current_size);
    result->size = current_size;

    free(temp_data);
    return result;
}

uint8_t *bpe_decompress(const BPEResult *compressed, size_t *out_size)
{
    (void)compressed;
    (void)out_size;
    return NULL;
}

void bpe_free(BPEResult *result)
{
    free(result->data);
}