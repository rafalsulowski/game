#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <Windows.h>

#include "fun.h"


/*
Poprawny przyklad pliku z danymi do programu:

Rozmiar planszy: 20x20
Punkty żywych komórek:
(1,1)
(1,2)
(2,1)
(2,2)
(2,3)
(3,3)
Granice: _Zamkniete / Otwarte
*/



int main(int argc, char *argv[])
{
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
    int **tab = readPoint(in, sizeX, sizeY);    //wczytanie punktow z pliku


    showTable(tab, sizeX, sizeY);

    char c;
    printf("Press space and confirm enter to start, Ctrl+C to stop\n");
    do
    {
        scanf("%c", &c);
    } while (c != ' ');
    system("cls");
    
    int i = 1;
    while(true) //nieskonczona petla rysujaca w odstepach 100ms nastepna iteracje
    {
        printf("iteration: %d\n", i++);
        tab = move(tab, sizeX, sizeY);
        showTable(tab, sizeX, sizeY);

        Sleep(100);
        system("cls"); 
    }
       
    
    return 0;
}