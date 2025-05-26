#include <SDL2/SDL.h>
#include <omp.h>
#include <stdio.h>

/*
 * Config
 */

#define WIDTH 800
#define HEIGHT 600
#define MAX_ITER 1000

/*
 * Globals
 */

double zoom = 1.0;
double offsetX = -0.5;
double offsetY = 0.0;

typedef enum {
  FRACTAL_MANDELBROT,
  FRACTAL_JULIA,
  FRACTAL_TRICORN,
  FRACTAL_MULTIBROT,
  FRACTAL_BURNINGSHIP,
} FractalType;

FractalType currentFractal = FRACTAL_MANDELBROT;

/* Julia constant */

double juliaCr = -0.7;
double juliaCi = 0.27015;

/*
 * Fractal Functions
 */

int mandelbrot(double cr, double ci) {
  double zr = 0, zi = 0;
  int n = 0;
  while (zr * zr + zi * zi <= 4.0 && n < MAX_ITER) {
    double tmp = zr * zr - zi * zi + cr;
    zi = 2.0 * zr * zi + ci;
    zr = tmp;
    n++;
  }
  return n;
}

int julia(double zr, double zi, double cr, double ci) {
  int n = 0;
  while (zr * zr + zi * zi <= 4.0 && n < MAX_ITER) {
    double tmp = zr * zr - zi * zi + cr;
    zi = 2.0 * zr * zi + ci;
    zr = tmp;
    n++;
  }
  return n;
}

int tricorn_set(double zr, double zi) {
  int n = 0;
  double pi = zi, pr = zr;
  while (zr * zr + zi * zi <= 4 && n < MAX_ITER) {
    double tmp = -2 * zr * zi + pi;
    zr = zr * zr - zi * zi + pr;
    zi = tmp;
    n++;
  }
  return n;
}

int multibrot3(double cr, double ci) {
  double zr = 0, zi = 0;
  int n = 0;
  while (zr * zr + zi * zi <= 4.0 && n < MAX_ITER) {
    double zr2 = zr * zr;
    double zi2 = zi * zi;
    double zr3 = zr * (zr2 - 3 * zi2);
    double zi3 = zi * (3 * zr2 - zi2);
    zr = zr3 + cr;
    zi = zi3 + ci;
    n++;
  }
  return n;
}

int burningShip(double cr, double ci) {
  double zr = 0, zi = 0;
  int n = 0;
  while (zr * zr + zi * zi <= 4.0 && n < MAX_ITER) {
    double tmp = zr * zr - zi * zi + cr;
    zi = fabs(2.0 * zr * zi) + ci;
    zr = fabs(tmp);
    n++;
  }
  return n;
}

int getFractalValue(double x, double y) {
  switch (currentFractal) {
  case FRACTAL_MANDELBROT:
    return mandelbrot(x, y);
  case FRACTAL_JULIA:
    return julia(x, y, juliaCr, juliaCi);
  case FRACTAL_TRICORN:
    return tricorn_set(x, y);
  case FRACTAL_MULTIBROT:
    return multibrot3(x, y);
  case FRACTAL_BURNINGSHIP:
    return burningShip(x, y);
  default:
    return 0;
  }
}

/*
 * Rendering
 */

void render(SDL_Renderer *renderer) {
  int samples = 2;
  double invSamples = 1.0 / (samples * samples);

#pragma omp parallel for collapse(2)
  for (int px = 0; px < WIDTH; px++) {
    for (int py = 0; py < HEIGHT; py++) {
      double rSum = 0, gSum = 0, bSum = 0;

      for (int sx = 0; sx < samples; sx++) {
        for (int sy = 0; sy < samples; sy++) {
          double subX = px + (sx + 0.5) / samples;
          double subY = py + (sy + 0.5) / samples;

          double x0 = ((subX - WIDTH / 2.0) * 4.0 / WIDTH) * zoom + offsetX;
          double y0 = ((subY - HEIGHT / 2.0) * 4.0 / WIDTH) * zoom + offsetY;

          int value = getFractalValue(x0, y0);

          if (value == MAX_ITER) {
            rSum += 0;
            gSum += 0;
            bSum += 0;
          } else {
            rSum += (value * 9) % 256;
            gSum += (value * 2) % 256;
            bSum += (value * 5) % 256;
          }
        }
      }

      int r = (int)(rSum * invSamples);
      int g = (int)(gSum * invSamples);
      int b = (int)(bSum * invSamples);

#pragma omp critical
      { // SDL is not thread-safe, so wrap this in a critical section
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawPoint(renderer, px, py);
      }
    }
  }

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

/* Main */

int main() {
  printf("Choose a fractal to render:\n");
  printf("1 = Mandelbrot\n");
  printf("2 = Julia\n");
  printf("3 = Tricorn\n");
  printf("4 = Multibrot\n");
  printf("5 = Burning Ship\n");
  int choice;
  if (scanf("%d", &choice) != 1) {
    printf("Invalid input.\n");
    return 1;
  }

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
  } else {
    printf("Invalid selection.\n");
    return 1;
  }

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window =
      SDL_CreateWindow("Fractal Viewer", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  int quit = 0;
  SDL_Event e;

  while (!quit) {
    render(renderer);
    handleInput(&e, &quit);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
