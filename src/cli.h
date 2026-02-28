#ifndef CLI_H
#define CLI_H

typedef enum { MODE_NONE, MODE_COMPRESS, MODE_DECOMPRESS } MODE;

typedef struct {
  MODE mode;
  char *input_path;
  char *output_path;
  int verbose;
} Args;

int parse_args(int argc, char **argv, Args *args);

void print_usage(void);

#endif
