#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>

#include "term.h"

#define SQUARE_SIZE 6

typedef struct {
  unsigned short x, y;
  unsigned short size;
  float hue, sat, val;
  short dx, dy;
} square;

struct winsize sz;

#define MIN_X (1)
#define MAX_X (sz.ws_col)

#define MIN_Y (1)
#define MAX_Y (sz.ws_row)

void checkWallCollision(square *s);
void checkVerticalIntersquareCollision(square *s1, square *s2);
void printSquare(square s);

int main(void) {
  // this must be the first operation, before any IO
  setbuf(stdout, NULL);
  clearScreen();

  // get terminal size.
  // todo: this code assumes the size does not change
  //       - can we handle size changes efficiently?
  //       - what should we do if the window changes size?
  // the answer to the first is yes.
  // https://psychocod3r.wordpress.com/2019/02/25/how-to-get-the-dimensions-of-a-linux-terminal-window-in-c/
  ioctl(0, TIOCGWINSZ, &sz);

  // we will have two squares, aligned vertically and starting on opposite sides
  square top = {
      .x = MIN_X,
      .y = MIN_Y,
      .size = SQUARE_SIZE,
      .hue = 0,
      .sat = 0.8f,
      .val = 0.8f,
      .dx = 1,
      .dy = 1,
  };

  square bottom = {
      .x = MIN_X,
      .y = MAX_Y - (SQUARE_SIZE - 1),
      .hue = 120,
      .sat = 0.8f,
      .val = 0.8f,
      .size = SQUARE_SIZE,
      .dx = 1,
      .dy = -1,
  };

  for (unsigned short y = 0; y < MAX_Y; y++) {
    for (unsigned short x = 0; x < MAX_X; x++) {
      printColorRGB(220, 220, 220);
    }
  }

  while (420) {
    checkWallCollision(&top);
    checkWallCollision(&bottom);

    // will they collide or be adjacent after the move?
    const unsigned short topBottom = top.y + top.size - 1;
    if (topBottom >= (bottom.y + bottom.dy)) {
      top.dy *= -1;
      bottom.dy *= -1;
    }

    printSquare(top);
    printSquare(bottom);

    top.x += top.dx;
    top.y += top.dy;

    bottom.x += bottom.dx;
    bottom.y += bottom.dy;

    top.hue = fmodf(top.hue + 0.25, 360);
    bottom.hue = fmodf(bottom.hue + 0.25, 360);

    usleep(75000);
  }
}

void checkWallCollision(square *s) {
  const int hitLeftWall = (s->dx < 0 && s->x <= MIN_X);
  const int hitRightWall = (s->dx > 0 && (s->x + s->size - 1) >= MAX_X);

  if (hitLeftWall || hitRightWall) {
    s->dx *= -1;
  }

  const int hitCeiling = (s->dy < 0 && s->y <= MIN_Y);
  const int hitFloor = (s->dy > 0 && (s->y + s->size - 1) >= MAX_Y);

  if (!hitCeiling && !hitFloor) {
    return;
  }

  s->dy *= -1;
}

void printSquare(square s) {
  for (unsigned short offsetY = 0; offsetY < s.size; offsetY++) {
    moveTo(s.x, s.y + offsetY);

    for (unsigned short offsetX = 0; offsetX < s.size; offsetX++) {
      printColorHSV(s.hue, s.sat, s.val);
    }
  }
}
