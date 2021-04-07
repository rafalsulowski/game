#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <Windows.h>

#include "fun.h"
#include "make_png.h"


int main(int argc, char *argv[])
{
    srand(time(NULL));
    char *nazwa_out = argc > 3 ? argv[3] : "out.png";
    int g = argc > 2 ? atoi(argv[2]) : 50;
    FILE *in = argc > 1 ? fopen(argv[1], "r") : NULL;
    if(!in)
    {
        fprintf(stderr, "Cannot open a file: \"%s\" with input data", argv[1]);
        fclose(in);
        return 1;
    }

    int sizeX = 0, sizeY = 0;                   //rozmiar tablicy na ktorej odbywa sie gra 
    readBoardSize(in, &sizeX, &sizeY);          //wczytanie do sizeX i sizeY rozmiaru planszy
    bool border = readBorderType(in);           //true - granice otwarte, false - granice martwe zamkniete
    Point **tab = readPoint(in, sizeX, sizeY);  //wczytanie punktow z pliku

    showTable(tab, sizeX, sizeY);

    char c;
    printf("Press space and confirm enter to start, Ctrl+C to stop\n");
    do
    {
        scanf("%c", &c);
    } while (c != ' ');
    //system("cls");


    int i = 1;
    if(border) 
    { //granice zywe
        while(g!=i-1)
        {
            printf("iteration: %d\n", i++);
            tab = moveBorderIsLive(tab, sizeX, sizeY);
            showTable(tab, sizeX, sizeY);
		//printf("%s \n", tab[6][6].color);
            //Sleep(100);
            //system("cls");
        }
    }
    else
    { //granice martwe
        while(g!=i-1)
        {
            printf("iteration: %d\n", i++);
            tab = moveBorderIsDead(tab, sizeX, sizeY);
            //showTable(tab, sizeX, sizeY);
	    //	printf("%s \n", tab[6][6].color);
            //Sleep(100);
            //system("cls");
        }
    }
    
	generate_png(tab, sizeX, sizeY, nazwa_out);
    return 0;
}
