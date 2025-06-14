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
  double q = (x - 0.25) * (x - 0.25) + y * y;
  if (q * (q + (x - 0.25)) <= 0.25 * y * y ||
      ((x + 1) * (x + 1) + y * y <= 0.0625))
    return MAX_ITER;

  double zr = 0, zi = 0;
  int n = 0;
  while (n < MAX_ITER) {
    double zr2 = zr * zr;
    double zi2 = zi * zi;
    if (zr2 + zi2 > 4.0)
      break;
    zi = 2.0 * zr * zi + y;
    zr = zr2 - zi2 + x;
    n++;
  }
  return n;
}

static int julia(double x, double y) {
  const double ci = juliaCi;
  const double cr = juliaCr;
  int n = 0;

  while (n < MAX_ITER) {
    const double x2 = x * x;
    const double y2 = y * y;
    if (x2 + y2 > 4.0)
      break;

    y = 2.0 * x * y + ci;
    x = x2 - y2 + cr;
    n++;
  }
  return n;
}

static int tricorn_set(double x, double y) {
  const double pr = x;
  const double pi = y;
  int n = 0;

  while (n < MAX_ITER) {
    const double x2 = x * x;
    const double y2 = y * y;
    if (x2 + y2 > 4.0)
      break;

    y = -2.0 * x * y + pi;
    x = x2 - y2 + pr;
    n++;
  }
  return n;
}

static int multibrot3(double x, double y) {
  double zr = 0, zi = 0;
  int n = 0;

  while (n < MAX_ITER) {
    const double zr2 = zr * zr;
    const double zi2 = zi * zi;

    if (zr2 + zi2 > 4.0)
      break;

    zr = zr * (zr2 - 3 * zi2) + x;
    zi = zi * (3 * zr2 - zi2) + y;
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

static int newton(double x, double y) {
  int n = 0;
  double tolerance = 1e-6;
  double zr = x;
  double zi = y;

  while (n < MAX_ITER) {
    double r2 = zr * zr, i2 = zi * zi;
    double r3 = r2 * zr - 3 * zr * i2;
    double i3 = 3 * r2 * zi - zi * i2;

    double fr = r3 - 1;
    double fi = i3;

    double dr = 3 * (zr * zr - zi * zi);
    double di = 6 * zr * zi;

    double denom = dr * dr + di * di;
    if (denom == 0.0)
      break;

    double new_zr = zr - (fr * dr + fi * di) / denom;
    double new_zi = zi - (fi * dr - fr * di) / denom;

    if (fabs(new_zr - zr) < tolerance && fabs(new_zi - zi) < tolerance)
      break;

    zr = new_zr;
    zi = new_zi;
    n++;
  }

  return n;
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
  case FRACTAL_NEWTON:
    fractalFn = newton;
    break;
  default:
    fractalFn = NULL;
  }
}

inline int getFractalValue(double x, double y) { return fractalFn(x, y); }
