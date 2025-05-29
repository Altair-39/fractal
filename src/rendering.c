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
extern FractalType currentFractal;

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
  int samples = 2;
  double invSamples = 1.0 / (samples * samples);

  Uint32 *pixels = malloc(WIDTH * HEIGHT * sizeof(Uint32));
  if (!pixels) {
    fprintf(stderr, "Failed to allocate pixel buffer\n");
    return;
  }

#pragma omp parallel for collapse(2) schedule(dynamic)
  for (int px = 0; px < WIDTH; px++) {
    for (int py = 0; py < HEIGHT; py++) {
      double rSum = 0, gSum = 0, bSum = 0;

      for (int sx = 0; sx < samples; sx++) {
        for (int sy = 0; sy < samples; sy++) {
          double subX = px + (sx + 0.5) / samples;
          double subY = py + (sy + 0.5) / samples;

          double x0 = ((subX - WIDTH / 2.0) * 4.0 / WIDTH) * zoom + offsetX;
          double y0 = ((subY - HEIGHT / 2.0) * 4.0 / WIDTH) * zoom + offsetY;

          int value = getFractalValue(currentFractal, x0, y0);

          if (value != MAX_ITER) {
            rSum += (value * 9) % 256;
            gSum += (value * 2) % 256;
            bSum += (value * 5) % 256;
          }
        }
      }

      int r = (int)(rSum * invSamples);
      int g = (int)(gSum * invSamples);
      int b = (int)(bSum * invSamples);

      Uint32 color = (255 << 24) | (r << 16) | (g << 8) | b;
      pixels[py * WIDTH + px] = color;
    }
  }

  SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, texture, NULL, NULL);
  SDL_RenderPresent(renderer);

  free(pixels);
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
