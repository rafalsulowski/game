#ifndef _FUN_H_
#define _FUN_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

//Nizej bede uzywal slow tablica - plansza zamiennie (beda znaczyly to samo)



//Struktura ktora oprocz wartosci czy komorka zyje bedzie przechowywac informacje o kolorze komorki
// state - okresla czy komorka zyje (1) lub nie zyje (0)
// color - okresla kolor komorki, zakres 0-255 (jeden bajt)
typedef struct pkt {
    int state;
    char color[25];
} Point;


/*Funkja wczytujaca punkty z pliku i zwracajaca talibce do gry wraz z naniesionymi punktami
Return: tablica (macierz) z punkatmi, gotowa do rozgrywki
Argumenty:
File - plik wejsciowy z danymi
int, int - rozmiar tablicy wczytany wczesniej
*/
Point** readPoint(FILE*, int , int);


/*Funkja wczytujaca typ granicy do wyboru: zamknieta martwa (komorki nie oddzialywuja z tablica) otwarta (komurki
sa przenoszone po dojsciu do granicy na drugi koniec palnszy) WYBOR GRANICY W PLIKU KONFIGURACYJYM Z POGLOGA PRZED NAZWA
NP. chcemy w programie granice otwarta to w pliku konfiguracyjnym zaznaczmy tak: Granice: Zamkniete / _Otwarte
jak chcemy zamknieta to: Granice: _Zamkniete / Otwarte

Return: true - granica otwarta, false - granica martwa zamknieta
Argumenty:
File - plik wejsciowy z danymi
int, int - rozmiar tablicy wczytany wczesniej
*/
bool readBorderType(FILE*);


/*Funkja wczytujaca rozmiar planszy do gry
Return: void
Argumenty:
File - plik wejsciowy z danymi
int, int - rozmiar planszy do wczytania
*/
void readBoardSize(FILE*, int*, int*);


/*Funkja wykonujaca jedna iteracje, sprawdza przyleganie komorek i analizuje czy komorka ginie/umiera
Return: zmodyfikowana tablica (macierz) z punkatmi.
Argumenty:
int** - tablica z punktami
int, int - rozmiar tablicy wczytany wczesniej
*/
Point** moveBorderIsLive(Point**, int, int);   //dla granicy zywej
Point** moveBorderIsDead(Point**, int, int);   //dla granicy martwej


/*Funkja rysujaca na ekranie plansze
Return: void
Argumenty:
int** - plansza
int, int - rozmiar tablicy wczytany wczesniej
*/
void showTable(Point**, int, int);

void shuffle(int *array, int n);
void newColor(char parents[3][24], char res[24]);

#endif
