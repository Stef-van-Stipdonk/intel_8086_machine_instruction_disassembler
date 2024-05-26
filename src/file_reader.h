#ifndef FILE_READER_H
#define FILE_READER_H

#include "define.h"
#include <sys/types.h>

const u8 *read_file(const char *path, size_t *out_size);

#endif // FILE_READER_H
