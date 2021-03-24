#include "fun.h"


bool readBorderType(FILE *in)
{
    char tmp;
    fscanf(in, "Granice: %cZamkniete / Otwarte\n", &tmp);
    printf("tmp = %c\n", tmp);

    if(tmp == '_')
        return false; //granice martwe zamkniete
    else
        return true; //granica otwarta
}


void readBoardSize(FILE *in, int *sizeX, int *sizeY)
{
    fscanf(in, "Rozmiar planszy: %dx%d\n", sizeX, sizeY);
    printf("Wczytano rozmiar tablicy: %dx%d\n", *sizeX, *sizeY);
}


Point** readPoint(FILE *in, int sizeX, int sizeY)
{    
    fscanf(in, "Punkty żywych komórek:\n"); //ominiecie tekstu

    //alokacja tablicy do gry
    Point **tab = (Point**) malloc(sizeof(Point) * sizeY);
    for(int i = 0; i < sizeY; i++)
        *(tab + i) = (Point*) malloc(sizeof(Point) * sizeX);

    for(int i = 0; i < sizeY; i++) 
        for(int j = 0; j < sizeX; j++)
        {
            tab[i][j].state = 0;
            tab[i][j].color = 0;
        }   

    //wypelienie talbicy do gry punktami
    int i = 0, bufX, bufY;
    while(fscanf(in, "(%d,%d)\n", &bufX, &bufY) == 2)
    {
        tab[bufX - 1][bufY - 1].state = 1;
        tab[bufX - 1][bufY - 1].color = (int)( rand() / (RAND_MAX + 0) * 255);

        printf("Wczytano punkt: (%d,%d)\n", bufX, bufY);
        i++;
    }

    return tab;
}


Point** move(Point **tab, int sizeX, int sizeY)
{
    int counter = 0;

    //tworzenie kopii glownej tablicy
    Point **Ctab = (Point**) malloc(sizeof(Point) * sizeY);
    for(int i = 0; i < sizeY; i++)
        *(Ctab + i) = (Point*) malloc(sizeof(Point) * sizeX);
    //przepisywanie wartosci
    for(int i = 0; i < sizeY; i++)
        for(int j = 0; j < sizeX; j++)
        {
            Ctab[i][j].state = tab[i][j].state;
            Ctab[i][j].color = tab[i][j].color;
        }

    //sprawdzanie przylegania
    for(int i = 0; i < sizeY; i++)
    {   
        for(int j = 0; j < sizeX; j++)
        { 
            
            //gorny pasek
            if(i > 0 && j > 0 && tab[i - 1][j - 1].state == 1)  //gorny lewy
                counter++;
            if(i > 0 && tab[i - 1][j].state == 1)  //gorny srodkowy
                counter++;
            if(i > 0 && j < sizeX - 1 && tab[i - 1][j + 1].state == 1)  //gorny prawy
                counter++;
            
            //dolny pasek pasek
            if(i < sizeY - 1 && j > 0 && tab[i + 1][j - 1].state == 1)  //dolny lewy
                counter++;
            if(i < sizeY - 1 && tab[i + 1][j].state == 1)  //dolny sordkowy
                counter++;
            if(i < sizeY - 1 && j < sizeX - 1 && tab[i + 1][j + 1].state == 1)  //dolny prawy
                counter++;

                // pozostale boki
            if(j > 0 && tab[i][j - 1].state == 1)  //srodkowy lewy
                counter++;
            if(j < sizeX - 1 && tab[i][j + 1].state == 1)  //srodkowy prawy
                counter++;

                //sprawdzenie czy powinny ozyc/zginac jakies komurki
            if(tab[i][j].state == 0 && counter == 3)
                Ctab[i][j].state = 1;
            
            else if(tab[i][j].state == 1 && (counter < 2 || counter > 3) )
                Ctab[i][j].state = 0;     
                 
            counter = 0;
        }
    }

    return Ctab;
}


void showTable(Point **tab, int sizeX, int sizeY)
{
    for(int i = 0; i < sizeY; i++)
    {   
        for(int j = 0; j < sizeX; j++)
        {
            if(tab[i][j].state == 1)
                printf(" *");
            else
                printf(" .");
        }
        printf("\n");
    }

    printf("\n");
}