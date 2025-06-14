#include "./headers/rendering.h"
#include "./headers/fractal.h"
#include "./headers/config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <string.h>
/*
 * Globals
 */

double zoom = 1.0;
double offsetX = -0.5;
double offsetY = 0.0;

Uint32 *colorPalette = NULL;
void init_palette(ColorConfig config) {
    if (colorPalette) free(colorPalette);
    colorPalette = malloc(MAX_ITER * sizeof(Uint32));
    
    for (int i = 0; i < MAX_ITER; i++) {
        int r = (i * config.r_mult + config.r_shift) & 0xFF;
        int g = (i * config.g_mult + config.g_shift) & 0xFF;
        int b = (i * config.b_mult + config.b_shift) & 0xFF;
        colorPalette[i] = (0xFF000000) | (b << 16) | (g << 8) | r;
    }
}
/*
 * Create the texture
 */

SDL_Texture *create_texture(SDL_Renderer *renderer) {
  return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                           SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
}

/*
 * Rendering
 */


void render(SDL_Renderer *renderer, SDL_Texture *texture) {
  const double invWidth = 4.0 / WIDTH;
  const double halfWidth = WIDTH / 2.0;
  const double halfHeight = HEIGHT / 2.0;

  void *pixels;
  int pitch;

  if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
    fprintf(stderr, "SDL_LockTexture failed: %s\n", SDL_GetError());
    return;
  }

  const double zoomedInvWidth = invWidth * zoom;
  const double zoomOffsetX = offsetX - halfWidth * zoomedInvWidth;
  const double zoomOffsetY = offsetY;

#pragma omp parallel for schedule(static, 1)
  for (int py = 0; py < HEIGHT; py++) {
    Uint32 * restrict row = (Uint32 *)((Uint8 *)pixels + py * pitch);
    double y0 = (py - halfHeight) * zoomedInvWidth + zoomOffsetY;

    for (int px = 0; px < WIDTH; px++) {
      double x0 = px * zoomedInvWidth + zoomOffsetX;
      int value = getFractalValue(x0, y0);
      row[px] = colorPalette[value];
    }
  }

  SDL_UnlockTexture(texture);
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

/*
 * Input Handling
 */

void handleInput(SDL_Event *e, int *quit) {
  while (SDL_PollEvent(e)) {
    if (e->type == SDL_QUIT ||
        (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE)) {
      *quit = 1;
    } else if (e->type == SDL_KEYDOWN) {
      double panAmount = zoom * 0.1;
      switch (e->key.keysym.sym) {
      case SDLK_w:
        offsetY -= panAmount;
        break;
      case SDLK_s:
        offsetY += panAmount;
        break;
      case SDLK_a:
        offsetX -= panAmount;
        break;
      case SDLK_d:
        offsetX += panAmount;
        break;
      case SDLK_q:
        zoom *= 0.8;
        break;
      case SDLK_e:
        zoom *= 1.25;
        break;
      }
    }
  }
}
