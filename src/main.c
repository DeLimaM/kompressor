#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "io.h"
#include "bpe.h"

int main(int argc, char **argv)
{
    Args args = {0};
    if (parse_args(argc, argv, &args) != 0)
        return 1;

    size_t size;
    uint8_t *data = read_file(args.input_path, &size);
    if (!data)
        return 1;

    if (args.mode == MODE_COMPRESS)
    {
        printf("Input: %zu bytes\n", size);
        BPEResult *result = bpe_compress(data, size);
        if (!result)
        {
            fprintf(stderr, "Compression failed\n");
            free(data);
            return 1;
        }
        printf("Compressed: %zu bytes (%d rules)\n", result->size, result->rule_count);
        printf("Ratio: %.1f%%\n", 100.0 * (double)result->size / (double)size);

        for (int i = 0; i < result->rule_count; i++)
        {
            printf("  Rule %d: 0x%02X -> (0x%02X, 0x%02X)\n",
                   i, result->rules[i].symbol,
                   result->rules[i].pair[0], result->rules[i].pair[1]);
        }

        bpe_free(result);
    }

    free(data);
    return 0;
}