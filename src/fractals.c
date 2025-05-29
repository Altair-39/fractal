#include "headers/fractal.h"
#include <math.h>
#include <string.h>

FractalFunc fractalFn;

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

static int julia(double x, double y) {
  int n = 0;
  double ci = juliaCi;
  double cr = juliaCr;
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

void assignFractal(FractalType currentFractal) {
  switch (currentFractal) {
  case FRACTAL_MANDELBROT:
    fractalFn = mandelbrot;
    break;
  case FRACTAL_TRICORN:
    fractalFn = tricorn_set;
    break;
  case FRACTAL_MULTIBROT:
    fractalFn = multibrot3;
    break;
  case FRACTAL_BURNINGSHIP:
    fractalFn = burningShip;
    break;
  case FRACTAL_SIERPINSKY:
    fractalFn = sierpinskiCarpet;
    break;
  case FRACTAL_JULIA:
    fractalFn = julia;
    break;
  default:
    fractalFn = NULL;
  }
}

inline int getFractalValue(double x, double y) { return fractalFn(x, y); }
