#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

char nodisp[8][10] = {{"bx + si"}, {"bx + di"}, {"bp + si"},
                      {"bp + di"}, {"si"},      {"di"},
                      {"bp"},      {"bx"}

};

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

static inline const char *get_register_name(int index, int is_word) {
  const char *reg_byte[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
  const char *reg_word[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

  if (is_word) {
    return reg_word[index];
  }

  return reg_byte[index];
}

static void disasm(uint8_t *data, size_t size) {
  uint8_t *p = data;
  uint8_t *p_end = p + size;

  while (p < p_end) {
    uint8_t op0 = *p;
    if ((op0 >> 2) == 0b100010) { // MOV

      const int w = op0 & 1;
      const int d = (op0 >> 1) & 1;

      const int is_word = w;
      const int is_source_in_reg = !d;

      uint8_t op1 = p[1];
      int rm = (op1) & 0b111;
      int reg = (op1 >> 3) & 0b111;
      int mod = (op1 >> 6) & 0b11;

      if (mod == 0b11) { // Register mode (no displacement)
        int src, dest;

        if (is_source_in_reg) {
          src = reg;
          dest = rm;
        } else {
          src = rm;
          dest = reg;
        }

        printf("mov %s, %s ; Register mode (no displacement)\n",
               get_register_name(dest, is_word),
               get_register_name(src, is_word));

        p += 2;
      } else if (mod == 0b01) { // Memory Mode, 8-bit displacement follows
        if (*((uint8_t *)p + 2) == 0) {
          if (is_source_in_reg) {
            printf("mov [%s], %s ; Memory mode, no displacement follows\n",
                   nodisp[rm], get_register_name(reg, w));
          } else {
            printf("mov %s, [%s] ; Memory mode, no displacement follows\n",
                   get_register_name(reg, w), nodisp[rm]);
          }
        } else {
          printf(
              "mov %s, [%s + %i] ; Memory mode, 8 bit displacement follows\n",
              get_register_name(reg, w), nodisp[rm], *((uint8_t *)p + 2));
        }
        p += 3;
      } else if (mod == 0b10) { // Memory Mode, 16-bit displacement follows
        printf("mov %s, [%s + %i] ; Memory mode, 16 bit displacement follows\n",
               get_register_name(reg, w), nodisp[rm], *((uint16_t *)(p + 2)));
        p += 4;
      } else if (mod == 0b00) { // Memory Mode, no displacement follows
        if (rm == 0b110) {
          p += 4;
        } else {
          if (is_source_in_reg) {
            printf("mov [%s], %s ; Memory mode (no displacement)\n", nodisp[rm],
                   get_register_name(reg, w));
          } else {
            printf("mov %s, [%s] ; Memory mode (no displacement)\n",
                   get_register_name(reg, w), nodisp[rm]);
          }

          p += 2;
        }
      }
    } else if ((op0 >> 4) == 0b1011) {
      int reg = (op0) & 0b111;
      int w = (op0 >> 3) & 1;

      if (w == 1) {
        printf("mov %s, %i ; Immediate to register 16 bit\n",
               get_register_name(reg, w), *((uint16_t *)(p + 1)));
        p += 3;
      } else {
        printf("mov %s, %i ; Immediate to register 8 bit\n",
               get_register_name(reg, w), *((uint8_t *)(p + 1)));

        p += 2;
      }

    } else {
      p++;
    }
  }
}
int main(int argc, char **argv) {
  if (argc != 2) {
    exit(EXIT_FAILURE);
  }

  size_t size;
  uint8_t *data = read_file(argv[1], &size);
  printf("; disassembly of %s, %zdb\n", argv[1], size);
  printf("bits 16\n");
  disasm(data, size);

  return EXIT_SUCCESS;
}
