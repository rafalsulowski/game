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
    Point **tab = (Point**) malloc(sizeof(Point*) * sizeY);
    for(int i = 0; i < sizeY; i++)
        *(tab + i) = (Point*) malloc(sizeof(Point) * sizeX);

    for(int i = 0; i < sizeY; i++) 
        for(int j = 0; j < sizeX; j++)
        {
            tab[i][j].state = 0;
	        strcpy(tab[i][j].color, "000000000000000000000000");
        }   

    //wypelienie talbicy do gry zywymi punktami
    int i = 0, bufX, bufY;
    char bin_color[25];
    while(fscanf(in, "(%d,%d, %s )\n", &bufX, &bufY, bin_color ) == 3)
    {
        tab[bufX - 1][bufY - 1].state = 1;
        strcpy(tab[bufX - 1][bufY - 1].color, bin_color);
        printf("Wczytano zywy punkt: (%d,%d,%s)\n", bufX, bufY, bin_color);
        i++;
    }

    //wypelienie talbicy punktami scian
    fscanf(in, "Punkty scian:\n"); //ominiecie tekstu
    i = 0;
    while(fscanf(in, "(%d,%d)\n", &bufX, &bufY) == 2)
    {
        tab[bufX - 1][bufY - 1].state = 3;
        strcpy(tab[bufX - 1][bufY - 1].color, "000000000000000001111111");

        printf("Wczytano punkt sciany: (%d,%d)\n", bufX, bufY);
        i++;
    }

    return tab;
}


void shuffle(int *array, int n)
{
	srand(time(0));
	if(n > 1)
	{
		int i;
		for(i = 0; i < n - 1; i++)
		{
			int j = i + rand()/ (RAND_MAX / (n - i) + 1);
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}


void newColor(char parents[3][25], char res[25])
{
	srand(time(0));
	int i;
	int n = rand() % 24 + 1;
	int chooseParents[3] = {0, 1, 2};
	shuffle(chooseParents, 3);
	int bitsToSwap[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
	shuffle(bitsToSwap, 24);
	strcpy(res, parents[chooseParents[0]]);
	for(i = 0; i < n; i++)
		res[bitsToSwap[i]] = parents[chooseParents[1]][bitsToSwap[i]];
}


void moveBorderIsDead(Point **tab, int sizeX, int sizeY)
{
    int counter = 0;
    char colors[3][25];
    char new[25];

    //tworzenie kopii glownej tablicy
    Point **Ctab = (Point**) malloc(sizeof(Point*) * sizeY);
    for(int i = 0; i < sizeY; i++)
        *(Ctab + i) = (Point*) malloc(sizeof(Point) * sizeX);
    //przepisywanie wartosci
    for(int i = 0; i < sizeY; i++)
        for(int j = 0; j < sizeX; j++)
        {
            Ctab[i][j].state = tab[i][j].state;
	    strcpy(Ctab[i][j].color, tab[i][j].color);
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
	    {
		    if(counter <= 2)
		   	 strcpy(colors[counter], tab[i-1][j-1].color);
		    counter++;
	    }
            if(i > 0 && tab[i - 1][j].state == 1)  //gorny srodkowy
	    {
		    if(counter <= 2)
		    	strcpy(colors[counter], tab[i-1][j].color);
		    counter++;
	    }
            if(i > 0 && j < sizeX - 1 && tab[i - 1][j + 1].state == 1)  //gorny prawy
	    {
		    if (counter <= 2)
			    strcpy(colors[counter], tab[i-1][j+1].color);
		    counter++;
	    }

            //dolny pasek pasek
            if(i < sizeY - 1 && j > 0 && tab[i + 1][j - 1].state == 1)  //dolny lewy
	    {
		    if(counter <=2)
		    	strcpy(colors[counter], tab[i+1][j-1].color);
		    counter++;
	    }
            if(i < sizeY - 1 && tab[i + 1][j].state == 1)  //dolny sordkowy
	    {
		    if(counter <=2)
		    	strcpy(colors[counter], tab[i+1][j].color);
		    counter++;
	    }
            if(i < sizeY - 1 && j < sizeX - 1 && tab[i + 1][j + 1].state == 1)  //dolny prawy
	    {
		    if(counter <= 2)
		    	strcpy(colors[counter], tab[i+1][j+1].color);
		    counter++;
	    }
            
                // pozostale boki
            if(j > 0 && tab[i][j - 1].state == 1)  //srodkowy lewy
		{
		    if(counter <= 2)
			strcpy(colors[counter], tab[i][j-1].color);
		    counter++;
		}
            if(j < sizeX - 1 && tab[i][j + 1].state == 1)  //srodkowy prawy
	    {
		    if(counter <= 2)
		    	strcpy(colors[counter], tab[i][j+1].color);
		    counter++;
	    }

                //sprawdzenie czy powinny ozyc/zginac jakies komurki
            if(tab[i][j].state == 0 && counter == 3){
		    Ctab[i][j].state = 1;
		    newColor(colors, new);
		    strcpy(Ctab[i][j].color, new);
	    }
            else if(tab[i][j].state == 1 && (counter < 2 || counter > 3) )
	    {
		    Ctab[i][j].state = 0;
		    strcpy(Ctab[i][j].color, "000000000000000000000000");
	    }
            counter = 0;
        }
    }

    for(int i = 0; i < sizeY; i++)
        for(int j = 0; j < sizeX; j++)
        {
            tab[i][j].state = Ctab[i][j].state;
            strcpy(tab[i][j].color, Ctab[i][j].color);
        }

    for(int i = 0; i < sizeY; i++)
        free(Ctab[i]);
    free(Ctab);
}


void moveBorderIsLive(Point **tab, int sizeX, int sizeY)
{
    int counter = 0;
    char colors[3][25];
    char new[25];

    //tworzenie kopii glownej tablicy
    Point **Ctab = (Point**) malloc(sizeof(Point*) * sizeY);
    for(int i = 0; i < sizeY; i++)
        *(Ctab + i) = (Point*) malloc(sizeof(Point) * sizeX);
    //przepisywanie wartosci
    for(int i = 0; i < sizeY; i++)
        for(int j = 0; j < sizeX; j++)
        {
            Ctab[i][j].state = tab[i][j].state;
	        strcpy(Ctab[i][j].color, tab[i][j].color);
        }


    //sprawdzanie przylegania
    for(int i = 0; i < sizeY; i++)
    {   
        for(int j = 0; j < sizeX; j++)
        {
            //pominiecei komorek tworzacych sciany
            if(tab[i][j].state == 3)
                continue;
			
			//warunki wynikajace z zywej granicy
            if(i == 0 && j == 0)
                counter += 5;
            else if(i == 0 && j == sizeX - 1)
                counter += 5;
            else if(i == 0 && j > 0)
                {
					counter += 3;
					strcpy(colors[0], "111111111111111111111111");
					strcpy(colors[1], "111111111111111111111111");
					strcpy(colors[2], "111111111111111111111111");
				}
            else if(i > 0 && j == 0)
                {
					counter += 3;
					strcpy(colors[0], "111111111111111111111111");
					strcpy(colors[1], "111111111111111111111111");
					strcpy(colors[2], "111111111111111111111111");
				}
            else if(i == sizeY - 1 && j == 0)
                counter += 5;
            if(i == sizeY - 1 && j > 0)
                {
					counter += 3;
					strcpy(colors[0], "111111111111111111111111");
					strcpy(colors[1], "111111111111111111111111");
					strcpy(colors[2], "111111111111111111111111");
				}
            else if(i == sizeY - 1 && j == sizeX - 1)
                counter += 5;
            else if(i > 0 && j == sizeX - 1)
                {
					counter += 3;
					strcpy(colors[0], "111111111111111111111111");
					strcpy(colors[1], "111111111111111111111111");
					strcpy(colors[2], "111111111111111111111111");
				}
            
            //gorny pasek
            if(i > 0 && j > 0 && tab[i - 1][j - 1].state == 1)  //gorny lewy
	    {
		    if(counter <= 2)
		   	 strcpy(colors[counter], tab[i-1][j-1].color);
		    counter++;
	    }
            if(i > 0 && tab[i - 1][j].state == 1)  //gorny srodkowy
	    {
		    if(counter <= 2)
		    	strcpy(colors[counter], tab[i-1][j].color);
		    counter++;
	    }
            if(i > 0 && j < sizeX - 1 && tab[i - 1][j + 1].state == 1)  //gorny prawy
	    {
		    if (counter <= 2)
			    strcpy(colors[counter], tab[i-1][j+1].color);
		    counter++;
	    }

            //dolny pasek pasek
            if(i < sizeY - 1 && j > 0 && tab[i + 1][j - 1].state == 1)  //dolny lewy
	    {
		    if(counter <=2)
		    	strcpy(colors[counter], tab[i+1][j-1].color);
		    counter++;
	    }
            if(i < sizeY - 1 && tab[i + 1][j].state == 1)  //dolny sordkowy
	    {
		    if(counter <=2)
		    	strcpy(colors[counter], tab[i+1][j].color);
		    counter++;
	    }
            if(i < sizeY - 1 && j < sizeX - 1 && tab[i + 1][j + 1].state == 1)  //dolny prawy
	    {
		    if(counter <= 2)
		    	strcpy(colors[counter], tab[i+1][j+1].color);
		    counter++;
	    }
            
                // pozostale boki
            if(j > 0 && tab[i][j - 1].state == 1)  //srodkowy lewy
		{
		    if(counter <= 2)
			strcpy(colors[counter], tab[i][j-1].color);
		    counter++;
		}
            if(j < sizeX - 1 && tab[i][j + 1].state == 1)  //srodkowy prawy
	    {
		    if(counter <= 2)
		    	strcpy(colors[counter], tab[i][j+1].color);
		    counter++;
	    }

                //sprawdzenie czy powinny ozyc/zginac jakies komurki
            if(tab[i][j].state == 0 && counter == 3){
		    Ctab[i][j].state = 1;
		    newColor(colors, new);
		    strcpy(Ctab[i][j].color, new);
	    }
            else if(tab[i][j].state == 1 && (counter < 2 || counter > 3) )
	    {
		    Ctab[i][j].state = 0;
		    strcpy(Ctab[i][j].color, "000000000000000000000000");
	    }
            counter = 0;
        }
    }

    for(int i = 0; i < sizeY; i++)
        for(int j = 0; j < sizeX; j++)
        {
            tab[i][j].state = Ctab[i][j].state;
            strcpy(tab[i][j].color, Ctab[i][j].color);
        }

    for(int i = 0; i < sizeY; i++)
        free(Ctab[i]);
    free(Ctab);
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
