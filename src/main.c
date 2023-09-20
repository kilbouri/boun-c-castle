#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>

#include "term.h"

typedef struct {
  unsigned short x, y;
  float hue;
  short dx, dy;
} square;

struct winsize sz;

#define MIN_X (1)
#define MAX_X (sz.ws_col)

#define MIN_Y (1)
#define MAX_Y (sz.ws_row)

void checkWallCollision(square *s);
void checkVerticalIntersquareCollision(square *s1, square *s2);

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
  sz.ws_row &= ~1; // the simulation requires that the height be even

  // we will have two squares, aligned vertically and starting on opposite sides
  square top = {
      .x = MIN_X,
      .y = MIN_Y,
      .hue = 0,
      .dx = 1,
      .dy = 1,
  };

  square bottom = {
      .x = MIN_X,
      .y = MAX_Y,
      .hue = 120,
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

    checkVerticalIntersquareCollision(&top, &bottom);

    moveTo(top.x, top.y);
    printColorHSV(top.hue, 0.75f, 0.75f);

    moveTo(bottom.x, bottom.y);
    printColorHSV(bottom.hue, 0.75f, 0.75f);

    top.x += top.dx;
    top.y += top.dy;

    bottom.x += bottom.dx;
    bottom.y += bottom.dy;

    top.hue = fmodf(top.hue + 0.25, 360);
    bottom.hue = fmodf(bottom.hue + 0.25, 360);

    usleep(40000);
  }
}

void checkWallCollision(square *s) {
  const int hitLeftWall = (s->dx < 0 && s->x <= MIN_X);
  const int hitRightWall = (s->dx > 0 && s->x >= MAX_X);

  if (hitLeftWall || hitRightWall) {
    s->dx *= -1;
  }

  const int hitCeiling = (s->dy < 0 && s->y <= MIN_Y);
  const int hitFloor = (s->dy > 0 && s->y >= MAX_Y);

  if (!hitCeiling && !hitFloor) {
    return;
  }

  s->dy *= -1;
}

void checkVerticalIntersquareCollision(square *s1, square *s2) {
  // they don't collide if they are moving in the same direction
  if (s1->dy == s2->dy) {
    return;
  }

  const int samePosition = s1->y == s2->y;
  const int adjacent = (s2->y + s2->dy) == s1->y;

  if (!samePosition && !adjacent) {
    return;
  }

  s1->dy *= -1;
  s2->dy *= -1;
}
