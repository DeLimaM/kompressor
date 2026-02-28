#include <stdio.h>
#include <string.h>

#include "cli.h"

int parse_args(int argc, char **argv, Args *args)
{
    if (argc < 4)
    {
        print_usage();
        return -1;
    }

    if (strcmp(argv[1], "compress") == 0)
    {
        args->mode = MODE_COMPRESS;
    }
    else if (strcmp(argv[1], "decompress") == 0)
    {
        args->mode = MODE_DECOMPRESS;
    }
    else
    {
        print_usage();
        return -1;
    }

    args->input_path = argv[2];
    args->output_path = argv[3];
    args->verbose = 0;

    for (int i = 4; i < argc; i++)
    {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
        {
            args->verbose = 1;
        }
        else
        {
            print_usage();
            return -1;
        }
    }

    return 0;
}

void print_usage(void)
{
    printf("Usage: kompressor <mode> <input_path> <output_path> [options]\n");
    printf("Modes:\n");
    printf("  compress   Compress the input file\n");
    printf("  decompress Decompress the input file\n");
    printf("Options:\n");
    printf("  -v, --verbose  Enable verbose output\n");
}