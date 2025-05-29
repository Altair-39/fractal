#ifndef FRACTAL_H
#define FRACTAL_H

#define MAX_ITER 1000
typedef enum {
  FRACTAL_MANDELBROT,
  FRACTAL_JULIA,
  FRACTAL_TRICORN,
  FRACTAL_MULTIBROT,
  FRACTAL_BURNINGSHIP,
  FRACTAL_SIERPINSKY,
} FractalType;

int getFractalValue(FractalType currentFractal, double x, double y);

#endif // !FRACTAL_H
