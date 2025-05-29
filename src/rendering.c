#include "./headers/rendering.h"
#include "./headers/fractal.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
/*
 * Globals
 */

double zoom = 1.0;
double offsetX = -0.5;
double offsetY = 0.0;

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
  const int samples = 2;
  const double invSamples = 1.0 / (samples * samples);
  const double invWidth = 4.0 / WIDTH;
  const double halfWidth = WIDTH / 2.0;
  const double halfHeight = HEIGHT / 2.0;

  void *pixels;
  int pitch;

  if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
    fprintf(stderr, "SDL_LockTexture failed: %s\n", SDL_GetError());
    return;
  }

#pragma omp parallel for collapse(2) schedule(dynamic)
  for (int py = 0; py < HEIGHT; py++) {
    Uint32 *row = (Uint32 *)((Uint8 *)pixels + py * pitch);
    for (int px = 0; px < WIDTH; px++) {
      double rSum = 0, gSum = 0, bSum = 0;

      for (int sx = 0; sx < samples; sx++) {
        for (int sy = 0; sy < samples; sy++) {
          double subX = px + (sx + 0.5) / samples;
          double subY = py + (sy + 0.5) / samples;

          double x0 = ((subX - halfWidth) * invWidth) * zoom + offsetX;
          double y0 = ((subY - halfHeight) * invWidth) * zoom + offsetY;

          int value = getFractalValue(x0, y0);

          if (value != MAX_ITER) {
            rSum += (value * 9) & 0xFF;
            gSum += (value * 2) & 0xFF;
            bSum += (value * 5) & 0xFF;
          }
        }
      }

      int r = (int)(rSum * invSamples);
      int g = (int)(gSum * invSamples);
      int b = (int)(bSum * invSamples);

      Uint32 color = (255 << 24) | (b << 16) | (g << 8) | r;

      row[px] = color;
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
