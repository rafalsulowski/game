#ifndef _MAKE_PNG_H_
#define _MAKE_PNG_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <png.h>

void binToRGB(char *, int*, int*, int*);
void write_png_file(char*);
void generate_png(Point **, int, int, char*);

#endif
