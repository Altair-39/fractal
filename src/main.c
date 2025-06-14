#include "./headers/fractal.h"
#include "./headers/rendering.h"
#include "./headers/config.h"
#include <SDL2/SDL.h>
#include <stdio.h>

FractalType currentFractal = FRACTAL_MANDELBROT;

int main() {
  printf("Choose a fractal to render:\n");
  printf("1 = Mandelbrot\n");
  printf("2 = Julia\n");
  printf("3 = Tricorn\n");
  printf("4 = Multibrot\n");
  printf("5 = Burning Ship\n");
  printf("6 = Sierpinski Carpet\n");
  printf("7 = Newton\n");
  int choice;
  if (scanf("%d", &choice) != 1) {
    printf("Invalid input.\n");
    return 1;
  }
  ColorConfig color_config = {6, 1, 4, 1, 2,1}; 

      if (!read_color_config(&color_config)) {
        printf("Using default color configuration\n");
    }
  init_palette(color_config);

  if (choice == 1) {
    currentFractal = FRACTAL_MANDELBROT;
  } else if (choice == 2) {
    currentFractal = FRACTAL_JULIA;
  } else if (choice == 3) {
    currentFractal = FRACTAL_TRICORN;
  } else if (choice == 4) {
    currentFractal = FRACTAL_MULTIBROT;
  } else if (choice == 5) {
    currentFractal = FRACTAL_BURNINGSHIP;
  } else if (choice == 6) {
    currentFractal = FRACTAL_SIERPINSKY;
  } else if (choice == 7) {
    currentFractal = FRACTAL_NEWTON;
  } else {
    printf("Invalid selection.\n");
    return 1;
  }

  assignFractal(currentFractal);

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window =
      SDL_CreateWindow("Fractal Viewer", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  int quit = 0;
  SDL_Event e;

  SDL_Texture *texture = create_texture(renderer);

  while (!quit) {
    render(renderer, texture);
    handleInput(&e, &quit);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
