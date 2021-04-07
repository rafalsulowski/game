prog: main.o fun.o fun.h make_png.o make_png.h
	cc -o prog main.c fun.c make_png.c -lm -lpng

main.o: main.c
	cc main.c
fun.o: fun.c fun.h
	cc -c fun.c
make_png.o: make_png.c make_png.h
	cc -c make_png.c -lm

.PHONY: clean

clean:
    rm -f prog main.o fun.o make_png.o
