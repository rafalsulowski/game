#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <png.h>
#include "fun.h"

int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * row_pointers;

void binToRGB(char *bin, int *r, int *g, int *b);

void write_png_file(char* file_name) {
  FILE *fp = fopen(file_name, "wb");
  if (!fp)
    printf("[write_png_file] File %s could not be opened for writing", file_name);

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    printf("[write_png_file] png_create_write_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    printf("[write_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[write_png_file] Error during init_io");

  png_init_io(png_ptr, fp);

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[write_png_file] Error during writing header");

  png_set_IHDR(png_ptr, info_ptr, width, height,
   bit_depth, color_type, PNG_INTERLACE_NONE,
   PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[write_png_file] Error during writing bytes");

  png_write_image(png_ptr, row_pointers);

  if (setjmp(png_jmpbuf(png_ptr)))
    printf("[write_png_file] Error during end of write");

  png_write_end(png_ptr, NULL);

  for (y=0; y<height; y++)
    free(row_pointers[y]);
  free(row_pointers);

  fclose(fp);
}

void generate_png(Point **tab, int sizeX, int sizeY, char *file_out) {
  width = sizeX;
  height = sizeY;
  bit_depth = 8;
  color_type = PNG_COLOR_TYPE_RGB;
  int R, G, B;

  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height *20);
  for (y=0; y<height*20; y++)
    row_pointers[y] = (png_byte*) malloc(3 * sizeof(png_byte) * width *20);

  for (y=0; y<height; y++) {
	  for(int h=0; h<20;h++) {
    png_bytep row = row_pointers[20*y+h];
    for (x=0; x<width; x++) {
 		
	binToRGB(tab[x][y].color, &R, &G, &B);
	png_bytep px = &(row[x * 60]);    
  	for(int g=0;g<20;g++) {
	px[3*g]=R;
	px[3*g+1]=G;
	px[3*g+2]=B;
	}
	
    }
	  }
  }
	width=sizeX*20;
	height=sizeY*20;
  write_png_file(file_out);
}

void binToRGB(char *bin, int *r, int *g, int *b) {
	int czer=0, ziel=0, nieb=0, i, j, c;
	char d;
	for(i=0; i<3; i++) {
		for(j=7;j>=0;j--) {
			c= i*8+j;
			d= bin[c];
			if(i==0) {
				if(d=='1')
						czer += pow(2,7-j);
			}

			else if(i==1) {
				if(d=='1')
						ziel += pow(2,7-j);
			}

			else if(i==2) {
				if(d=='1')
						nieb += pow(2,7-j);
			}
		}
	}

	*r=czer;
	*g=ziel;
	*b=nieb;
}

