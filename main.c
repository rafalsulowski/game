#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


#include "fun.h"
#include "make_png.h"


int main(int argc, char *argv[])
{
    if(argc==1) {
    printf("\n Symulator gry w zycie\n Program pobiera liste punktow i drukuje obraz tych punktow po podanej liczbie iteracji.\n\n Linia polecenia: \n\n  ./prog <plik> <liczba> <plik2> \n plik - nazwa z plikiem opisujacym punkty\n liczba - liczba iteracji (dni gry w zycie)\n plik2 - nazwa pliku do ktorego program zapisze obraz po wykonanych iteracjach (nazwa musi miec rozszerzenie .png)\n\n ");
    return 1;
    }
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

    if(sizeX <= 50 && sizeY <= 50)
    {
        showTable(tab, sizeX, sizeY);

        char c;
        printf("Press space and confirm enter to start, Ctrl+C to stop\n");
        do
        {
            scanf("%c", &c);
        } while (c != ' ');
        
       // system("cls");
    }
    else{
        printf("Widok koncowy zostanie zapisany jako %s\n", nazwa_out);
    }

    int i = 1;
    if(border) 
    { //granice zywe
        while(g!=i-1)
        {
            moveBorderIsLive(tab, sizeX, sizeY);
            
            if(sizeX <= 50 && sizeY <= 50)
            {
                printf("iteration: %d\n", i++);
                showTable(tab, sizeX, sizeY);

		printf("\033[2J");
		printf("\033[0;0f");
		usleep(150000);
		
	    }
        }
    }
    else
    { //granice martwe
        while(g!=i-1)
        {
            moveBorderIsDead(tab, sizeX, sizeY);
            
            if(sizeX <= 50 && sizeY <= 50)
            {
                printf("iteration: %d\n", i++);
                showTable(tab, sizeX, sizeY);
		printf("\033[2J");
		printf("\033[0;0f");
		usleep(150000);
            }
        }
    }
	printf("iteration: %d\n", g);	
	showTable(tab, sizeX, sizeY);    
	
    generate_png(tab, sizeX, sizeY, nazwa_out);

    printf("Program poprawnie zakonczyl dzialanie\n");
    return 0;
}
