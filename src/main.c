#include <stdio.h>

#include "cli.h"

int main(int argc, char **argv)
{
    Args args = {0};
    parse_args(argc, argv, &args);
    printf("Mode: %d\n", args.mode);
    printf("Input Path: %s\n", args.input_path);
    printf("Output Path: %s\n", args.output_path);
    printf("Verbose: %d\n", args.verbose);

    return 0;
}