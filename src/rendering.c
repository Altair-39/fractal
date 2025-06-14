#include "./headers/rendering.h"
#include "./headers/fractal.h"
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

void init_palette() {
    if (colorPalette) free(colorPalette);
    colorPalette = malloc(MAX_ITER * sizeof(Uint32));
    for (int i = 0; i < MAX_ITER; i++) {
        int r = (i * 9) & 0xFF;
        int g = (i * 2) & 0xFF;
        int b = (i * 5) & 0xFF;
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
  Uint32 *colorPalette = malloc(MAX_ITER * sizeof(Uint32));
  for (int i = 0; i < MAX_ITER; i++) {
    int r = (i * 9) & 0xFF;
    int g = (i * 2) & 0xFF;
    int b = (i * 5) & 0xFF;
    colorPalette[i] = (255 << 24) | (b << 16) | (g << 8) | r;
  }

  if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
    fprintf(stderr, "SDL_LockTexture failed: %s\n", SDL_GetError());
    return;
  }

  const double zoomedInvWidth = invWidth * zoom;
  const double zoomOffsetX = offsetX - halfWidth * zoomedInvWidth;

#pragma omp parallel for schedule(dynamic, 16) collapse(2)
  for (int py = 0; py < HEIGHT; py++) {
    Uint32 *row = (Uint32 *)((Uint8 *)pixels + py * pitch);

    const double y_base = (py - halfHeight) * zoomedInvWidth + offsetY;

    for (int px = 0; px < WIDTH; px++) {
      double x0 = px * zoomedInvWidth + zoomOffsetX;
      double y0 = y_base;

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
