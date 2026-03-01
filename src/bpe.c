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
    int (*pair_counts)[MAX_RULES] = malloc(MAX_RULES * MAX_RULES * sizeof(int));
    if (!pair_counts)
    {
        free(temp_data);
        free(result);
        return NULL;
    }

    BPERule temp_rules[MAX_RULES];
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
        temp_rules[rule_count] = (BPERule){.symbol = (uint8_t)sym, .pair = {a, b}};
        rule_count++;
        current_size = replace_pair(temp_data, current_size, a, b, (uint8_t)sym);
    }

    free(pair_counts);

    result->rule_count = rule_count;
    result->rules = malloc(rule_count * sizeof(BPERule));
    if (!result->rules)
    {
        free(temp_data);
        free(result);
        return NULL;
    }
    memcpy(result->rules, temp_rules, rule_count * sizeof(BPERule));

    result->data = malloc(current_size);
    if (!result->data)
    {
        free(result->rules);
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
    int is_rule[MAX_RULES];
    uint8_t rule_pair[MAX_RULES][2];
    memset(is_rule, 0, sizeof(is_rule));

    for (int i = 0; i < compressed->rule_count; i++)
    {
        uint8_t sym = compressed->rules[i].symbol;
        is_rule[sym] = 1;
        rule_pair[sym][0] = compressed->rules[i].pair[0];
        rule_pair[sym][1] = compressed->rules[i].pair[1];
    }

    size_t current_size = compressed->size;
    uint8_t *current_data = malloc(current_size);
    memcpy(current_data, compressed->data, current_size);
    for (int i = compressed->rule_count - 1; i >= 0; i--)
    {
        uint8_t sym = compressed->rules[i].symbol;
        if (is_rule[sym])
        {

            size_t sym_count = 0;
            for (size_t c = 0; c < current_size; c++)
                if (current_data[c] == sym)
                    sym_count++;
            size_t new_size = current_size + sym_count;
            uint8_t *new_data = malloc(new_size);
            size_t j = 0;
            for (size_t k = 0; k < current_size; k++)
            {
                if (current_data[k] == sym)
                {
                    new_data[j++] = rule_pair[sym][0];
                    new_data[j++] = rule_pair[sym][1];
                }
                else
                {
                    new_data[j++] = current_data[k];
                }
            }
            free(current_data);
            current_data = new_data;
            current_size = j;
        }
    }

    *out_size = current_size;
    return current_data;
}

void bpe_free(BPEResult *result)
{
    free(result->rules);
    free(result->data);
}