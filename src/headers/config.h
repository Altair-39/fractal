typedef struct {
  int r_mult;
  int g_mult;
  int b_mult;
  int r_shift;
  int g_shift;
  int b_shift;
} ColorConfig;

extern ColorConfig color_config;

void init_palette(ColorConfig config);

int read_color_config(ColorConfig *config);
