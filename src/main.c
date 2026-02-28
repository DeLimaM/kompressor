#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "io.h"

int main(int argc, char **argv)
{
    Args args = {0};
    parse_args(argc, argv, &args);
    printf("Mode: %d\n", args.mode);
    printf("Input Path: %s\n", args.input_path);
    printf("Output Path: %s\n", args.output_path);
    printf("Verbose: %d\n", args.verbose);

    uint8_t *data;
    size_t size;
    data = read_file(args.input_path, &size);
    if (data)
    {
        printf("Read %zu bytes from %s\n", size, args.input_path);
        free(data);
    }

    if (write_file(args.output_path, data, size) == 0)
    {
        printf("Wrote %zu bytes to %s\n", size, args.output_path);
    }

    return 0;
}