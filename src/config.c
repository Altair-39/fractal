#include "./headers/config.h"
#include <stdio.h>

int read_color_config(ColorConfig *config) {
  FILE *file = fopen("config.txt", "r");
  if (!file) {
    fprintf(stderr, "Error: Could not open config file\n");
    return 0;
  }

  int read = fscanf(file, "%d %d %d %d %d %d", &config->r_mult, &config->g_mult,
                    &config->b_mult, &config->r_shift, &config->g_shift,
                    &config->b_shift);

  fclose(file);

  if (read != 6) {
    fprintf(stderr, "Error: Invalid config file format\n");
    return 0;
  }

  return 1;
}
