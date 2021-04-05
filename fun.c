#include "fun.h"


bool readBorderType(FILE *in)
{
    char *tmp = (char*) malloc(sizeof(char) * 15);
    fscanf(in, "Granice (zmakniete / otwarte): %s\n", tmp);
    printf("typ granic (zamkniete/otwarte) = %s\n", tmp);

    if(!strcmp(tmp, "zamkniete"))
        return false; //granice martwe zamkniete (zero skojarzyc z zerową wartością komorki na granicy)
    else
        return true; //granica zywa
}


void readBoardSize(FILE *in, int *sizeX, int *sizeY)
{
    fscanf(in, "Rozmiar planszy (szerokosc x wysokosc): %dx%d\n", sizeX, sizeY);
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

    //wypelienie talbicy do gry zywymi punktami
    int i = 0, bufX, bufY;
    while(fscanf(in, "(%d,%d)\n", &bufX, &bufY) == 2)
    {
        tab[bufX - 1][bufY - 1].state = 1;
        tab[bufX - 1][bufY - 1].color = (int)( rand() / (RAND_MAX + 0) * 255);

        printf("Wczytano zywy punkt: (%d,%d)\n", bufX, bufY);
        i++;
    }

    //wypelienie talbicy punktami scian
    fscanf(in, "Punkty scian:\n"); //ominiecie tekstu
    i = 0;
    while(fscanf(in, "(%d,%d)\n", &bufX, &bufY) == 2)
    {
        tab[bufX - 1][bufY - 1].state = 3;
        tab[bufX - 1][bufY - 1].color = 0;

        printf("Wczytano punkt sciany: (%d,%d)\n", bufX, bufY);
        i++;
    }

    return tab;
}


Point** moveBorderIsDead(Point **tab, int sizeX, int sizeY)
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
            //pominiecei komorek tworzacych sciany
            if(tab[i][j].state == 3)
                continue;
            
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


Point** moveBorderIsLive(Point **tab, int sizeX, int sizeY)
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
            //pominiecei komorek tworzacych sciany
            if(tab[i][j].state == 3)
                continue;
            
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
            if(tab[i][j].state == 3)
                printf(" &");
            else if(tab[i][j].state == 1)
                printf(" *");
            else
                printf(" .");
        }
        printf("\n");
    }

    printf("\n");
}