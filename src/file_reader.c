#include "file_reader.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const u8 *read_file(const char *path, size_t *out_size) {
  FILE *f = fopen(path, "rb");
  assert(f != NULL);
  assert(fseek(f, 0, SEEK_END) == 0);
  size_t size = ftell(f);
  assert(fseek(f, 0, SEEK_SET) == 0);
  u8 *data = malloc(size);
  assert(fread(data, size, 1, f) == 1);
  if (out_size)
    *out_size = size;
  fclose(f);
  return data;
}
