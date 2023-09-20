#include <stdio.h>
#include <math.h>
#include "term.h"

#define ANSI_ESCAPE "\033["

#define Max(a, b) ((a) > (b) ? (a) : (b))
#define Min(a, b) ((a) > (b) ? (b) : (a))
#define Clamp(val, min, max) (Max((min), Min((val), (max))))

#define RGB_CHANNEL_FROM_01(val) (255 * (val))

void moveTo(unsigned short x, unsigned short y) {
  printf(ANSI_ESCAPE "%hu;%huf", y, x);
}

void clearScreen() { //
  printf(ANSI_ESCAPE "2J");
}

void printColorRGB(unsigned red, unsigned green, unsigned blue) {
  const unsigned r = Clamp(red, 0, 255);
  const unsigned g = Clamp(green, 0, 255);
  const unsigned b = Clamp(blue, 0, 255);

  printf(ANSI_ESCAPE "48;2;%u;%u;%um ", r, g, b);
}

inline static float HSVChannelFunction(float hue, float sat, float val,
                                       float n) {
  const float k = fmodf(n + hue / 60, 6);
  return val - (val * sat * Max(0, Min(4 - k, 1)));
}

void printColorHSV(float hue, float sat, float val) {
  float h = Clamp(hue, 0, 360) / 60;
  float s = Clamp(sat, 0, 1);
  float v = Clamp(val, 0, 1);

  unsigned short r, g, b;

  if (s == 0) {
    r = v * 255;
    g = v * 255;
    b = v * 255;
  } else {
    if (h >= 6) {
      h = 0;
    }

    float var_i = (int)h;
    float var_1 = v * (1 - s);
    float var_2 = v * (1 - s * (h - var_i));
    float var_3 = v * (1 - s * (1 - (h - var_i)));

    float rPrime, gPrime, bPrime;

    if (var_i == 0) {
      rPrime = v;
      gPrime = var_3;
      bPrime = var_1;
    } else if (var_i == 1) {
      rPrime = var_2;
      gPrime = v;
      bPrime = var_1;
    } else if (var_i == 2) {
      rPrime = var_1;
      gPrime = v;
      bPrime = var_3;
    } else if (var_i == 3) {
      rPrime = var_1;
      gPrime = var_2;
      bPrime = v;
    } else if (var_i == 4) {
      rPrime = var_3;
      gPrime = var_1;
      bPrime = v;
    } else {
      rPrime = v;
      gPrime = var_1;
      bPrime = var_2;
    }

    r = rPrime * 255;
    g = gPrime * 255;
    b = bPrime * 255;
  }

  printColorRGB(r, g, b);
}

void printReset() { //
  printf(ANSI_ESCAPE "0m");
}
