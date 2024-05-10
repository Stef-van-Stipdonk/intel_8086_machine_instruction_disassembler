#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

char nodisp[8][10] = {{"bx + si"}, {"bx + di"}, {"bp + si"}, {"bp + di"},
                      {"si"},      {"di"},      {"bp"},      {"bx"}};

static const uint8_t *read_file(const char *path, size_t *out_size) {
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

static inline const char *get_register_name(int index, int is_word) {
  const char *reg_byte[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
  const char *reg_word[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

  if (is_word) {
    return reg_word[index];
  }

  return reg_byte[index];
}

static void print_byte_as_binary(const uint8_t *byte, uint8_t amount) {
  for (int i = 0; i < amount; i++) {
    for (int y = 7; y >= 0; y--) {
      uint8_t op = byte[i];
      printf("%d", (op >> y) & 1);
    }
    printf(" ");
  }

  printf("\n");
}

static void disasm(const uint8_t *data, const size_t size) {
  const uint8_t *p = data;
  const uint8_t *p_end = p + size;

  while (p < p_end) {
    uint8_t op0 = *p;
    if ((op0 >> 2) == 0b100010 || (op0 >> 2) == 0b000000 ||
        (op0 >> 2) == 0b001010 ||
        (op0 >> 2) == 0b001110) { // MOV, ADD, SUB, CMP
      const int w = op0 & 1;
      const int d = (op0 >> 1) & 1;

      uint8_t op1 = p[1];
      const int rm = (op1) & 0b111;
      const int reg = (op1 >> 3) & 0b111;
      const int mod = (op1 >> 6) & 0b11;

      if ((op0 >> 2) == 0b100010)
        printf("mov ");
      else if ((op0 >> 2) == 0b000000) {
        printf("add ");
      } else if ((op0 >> 2) == 0b001010) {
        printf("sub ");
      } else if ((op0 >> 2) == 0b001110) {
        printf("cmp ");
      }

      if (mod == 0b00) {
        if (rm == 0b110) {
          if (d) {
            printf("%s, %i\n", get_register_name(reg, w), (uint8_t)rm);
          } else {
            printf("%i, %s\n", (uint8_t)rm, get_register_name(reg, w));
          }
        } else {
          if (d) {
            printf("%s, [%s]\n", get_register_name(reg, w), nodisp[rm]);
          } else {
            printf("[%s], %s\n", nodisp[rm], get_register_name(reg, w));
          }
        }
        p += 2;
      } else if (mod == 0b01) {
        if ((*(uint8_t *)(p + 2)) > 0) {
          if (d) {
            printf("%s, [%s + %i]\n", get_register_name(reg, w), nodisp[rm],
                   *(int8_t *)(p + 2));
          } else {
            printf("[%s + %i], %s\n", nodisp[rm], *(int8_t *)(p + 2),
                   get_register_name(reg, w));
          }
        } else {
          if (d) {
            printf("%s, [%s]\n", get_register_name(reg, w), nodisp[rm]);
          } else {
            printf("[%s], %s\n", nodisp[rm], get_register_name(reg, w));
          }
        }
        p += 3;
      } else if (mod == 0b10) {
        if ((*(uint16_t *)(p + 2)) > 0) {
          printf("%s, [%s + %i]\n", get_register_name(reg, w), nodisp[rm],
                 *(uint16_t *)(p + 2));
        } else {
          printf("%s, [%s]\n", get_register_name(reg, w), nodisp[rm]);
        }
        p += 4;
      } else if (mod == 0b11) {
        if (d) {
          printf("%s, %s\n", get_register_name(reg, w),
                 get_register_name(rm, w));
        } else {
          printf("%s, %s\n", get_register_name(rm, w),
                 get_register_name(reg, w));
        }

        p += 2;
      }
    } else if ((op0 >> 2) == 0b100000) { // ADD immediate to register/memory
      const int w = op0 & 1;
      const int s = (op0 >> 1) & 1;

      uint8_t op1 = p[1];
      const int rm = (op1) & 0b111;
      const int reg = (op1 >> 3) & 0b111;
      const int mod = (op1 >> 6) & 0b11;

      if (reg == 0b000) {
        printf("add ");
      } else if (reg == 0b101) {
        printf("sub ");
      } else if (reg == 0b111) {
        printf("cmp ");
      }

      if (mod == 0b00) {
        if (rm == 0b110) {
          if (w) {
            if (s) {
              printf("word [%i], %i\n", *(uint16_t *)(p + 2),
                     *(uint8_t *)(p + 4));
              p += 5;
            } else {
              printf("word [%i], %i\n", *(uint16_t *)(p + 2),
                     *(uint16_t *)(p + 4));
              p += 6;
            }
          } else {
            printf("byte [%i], %i\n", *(uint8_t *)(p + 2), *(uint8_t *)(p + 3));
            p += 4;
          }
        } else {
          if (w) {
            if (s) {
              printf("word [%s], %i\n", nodisp[rm], *(uint8_t *)(p + 2));
            } else {

              printf("word [%s], %i\n", nodisp[rm], *(uint16_t *)(p + 2));
            }
          } else {
            printf("byte [%s], %i\n", nodisp[rm], *(uint8_t *)(p + 2));
          }
          p += 3;
        }
      } else if (mod == 0b01) {
        exit(EXIT_FAILURE);
      } else if (mod == 0b10) {
        if (w) {
          if (s) {
            printf("word [%s + %i], %i\n", nodisp[rm], *(uint16_t *)(p + 2),
                   *(uint8_t *)(p + 4));
            p += 5;
          } else {
            printf("word [%s + %i], %i\n", nodisp[rm], *(uint16_t *)(p + 2),
                   *(uint16_t *)(p + 4));

            p += 6;
          }
        } else {
          printf("byte [%s + %i], %i\n", nodisp[rm], *(uint8_t *)(p + 2),
                 *(uint8_t *)(p + 3));
          p += 4;
        }
      } else if (mod == 0b11) {
        printf("%s, %i\n", get_register_name(rm, w), *(int8_t *)(p + 2));

        p += 3;
      }
    } else if ((op0 >> 1) == 0b0000010 || (op0 >> 1) == 0b0010110 ||
               (op0 >> 1) == 0b0011110) { // ADD, SUB immediate to accumulator
      const uint8_t w = op0 & 1;
      const uint8_t acummulator = 0b000;

      if ((op0 >> 1) == 0b0000010)
        printf("add ");
      else if ((op0 >> 1) == 0b0010110) {
        printf("sub ");
      } else if ((op0 >> 1) == 0b0011110) {
        printf("cmp ");
      }

      if (w) {
        printf("%s, %i\n", get_register_name(acummulator, w),
               *(int16_t *)(p + 1));

        p += 3;
      } else {
        printf("%s, %i\n", get_register_name(acummulator, w),
               *(int8_t *)(p + 1));

        p += 2;
      }
    } else if ((op0 >> 4) == 0b1011) { // MOV immediate to register
      const uint8_t w = (op0 >> 3) & 1;
      const uint8_t reg = op0 & 0b111;

      printf("mov ");

      if (w) {
        printf("%s, %i\n", get_register_name(reg, w), *(int16_t *)(p + 1));
        p += 3;
      } else {
        printf("%s, %i\n", get_register_name(reg, w), *(int8_t *)(p + 1));
        p += 2;
      }
    } else {
      printf("Not implemented instruction\n");
      print_byte_as_binary(p, 6);
      exit(EXIT_FAILURE);
    }
  }
}
int main(int argc, char **argv) {
  if (argc != 2) {
    exit(EXIT_FAILURE);
  }

  size_t size;
  const uint8_t *data = read_file(argv[1], &size);
  printf("; disassembly of %s, %zdb\n", argv[1], size);
  printf("bits 16\n");
  disasm(data, size);

  return EXIT_SUCCESS;
}
