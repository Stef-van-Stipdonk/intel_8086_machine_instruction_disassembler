#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LENGTH(xs) (sizeof(xs) / sizeof(xs[0]))

static uint8_t *read_file(const char *path, size_t *out_size) {
  FILE *f = fopen(path, "rb");
  assert(f != NULL);
  assert(fseek(f, 0, SEEK_END) == 0);
  size_t size = ftell(f);
  assert(fseek(f, 0, SEEK_SET) == 0);
  uint8_t *data = malloc(size);
  assert(fread(data, size, 1, f) == 1);
  if (out_size)
    *out_size = size;
  fclose(f);
  return data;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: %s <binary>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  size_t size;
  uint8_t *data = read_file(argv[1], &size);
  printf("; disassembly of %s, %zdb\n", argv[1], size);
  printf("bits 16\n");
  //	disasm(data, size);

  return EXIT_SUCCESS;
}
