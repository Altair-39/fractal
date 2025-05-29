#include "headers/fractal.h"
#include <math.h>

/* Julia constant */

double juliaCr = -0.7;
double juliaCi = 0.27015;

/*
 * Fractal Functions
 */

static int mandelbrot(double x, double y) {
  double zr = 0, zi = 0;
  int n = 0;
  while (zr * zr + zi * zi <= 4.0 && n < MAX_ITER) {
    double tmp = zr * zr - zi * zi + x;
    zi = 2.0 * zr * zi + y;
    zr = tmp;
    n++;
  }
  return n;
}

static int julia(double x, double y, double cr, double ci) {
  int n = 0;
  while (x * x + y * y <= 4.0 && n < MAX_ITER) {
    double tmp = x * x - y * y + cr;
    y = 2.0 * x * y + ci;
    x = tmp;
    n++;
  }
  return n;
}

static int tricorn_set(double x, double y) {
  int n = 0;
  double pi = y, pr = x;
  while (x * x + y * y <= 4 && n < MAX_ITER) {
    double tmp = -2 * x * y + pi;
    x = x * x - y * y + pr;
    y = tmp;
    n++;
  }
  return n;
}

static int multibrot3(double x, double y) {
  double zr = 0, zi = 0;
  int n = 0;
  while (zr * zr + zi * zi <= 4.0 && n < MAX_ITER) {
    double zr2 = zr * zr;
    double zi2 = zi * zi;
    double zr3 = zr * (zr2 - 3 * zi2);
    double zi3 = zi * (3 * zr2 - zi2);
    zr = zr3 + x;
    zi = zi3 + y;
    n++;
  }
  return n;
}

static int burningShip(double x, double y) {
  double zr = 0, zi = 0;
  int n = 0;
  while (zr * zr + zi * zi <= 4.0 && n < MAX_ITER) {
    double tmp = zr * zr - zi * zi + x;
    zi = fabs(2.0 * zr * zi) + y;
    zr = fabs(tmp);
    n++;
  }
  return n;
}

static int sierpinskiCarpet(double x, double y) {
  x = (x + 1.0) / 2.0;
  y = (y + 1.0) / 2.0;

  int level = 0;
  while (x > 0 && y > 0 && level < MAX_ITER) {
    if ((int)(x * 3) % 3 == 1 && (int)(y * 3) % 3 == 1) {
      return level;
    }
    x *= 3;
    y *= 3;
    level++;
  }
  return MAX_ITER;
}

int getFractalValue(FractalType currentFractal, double x, double y) {
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
  case FRACTAL_SIERPINSKY:
    return sierpinskiCarpet(x, y);
  default:
    return 0;
  }
}
