#ifndef TERM_H
#define TERM_H

void moveTo(unsigned short x, unsigned short y);
void clearScreen();
void printColorRGB(unsigned r, unsigned g, unsigned b);
void printColorHSV(float hue, float sat, float val);
void printReset();

#endif // TERM_H
