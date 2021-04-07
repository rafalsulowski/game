prog: main.c fun.c fun.h make_png.c make_png.h
	cc main.c fun.c make_png.c -lm -lpng
