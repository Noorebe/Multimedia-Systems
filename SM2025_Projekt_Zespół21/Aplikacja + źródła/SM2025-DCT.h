#ifndef SM2025_DCT_H_INCLUDED
#define SM2025_DCT_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <ostream>
#include <iomanip>
#include <cmath>

#define ROZMIAR_ZYGZAKA (rozmiarBloku * rozmiarBloku)


const int rozmiarBloku = 16;
void Funkcja1DCT();
void Funkcja2DCT();


struct macierz {
    float dct[rozmiarBloku][rozmiarBloku];
    Uint8 dane[rozmiarBloku][rozmiarBloku];
};

void wyswietlDane(macierz blok);
void wyswietlDCT(macierz blok);

void konwersjaDoSkaliSzarosci();
void PodzialNaBloki();
void zbierzZygzakiem(float dct[rozmiarBloku][rozmiarBloku],
                     float zygzak[ROZMIAR_ZYGZAKA]);
void modyfikujZygzak(float zygzak[ROZMIAR_ZYGZAKA], int ileZostawic);
void odbudujZygzak(float zygzak[ROZMIAR_ZYGZAKA],
                   float dct[rozmiarBloku][rozmiarBloku]);
macierz iDCT(macierz blokDCT);
void RysujBlok(macierz blok, int bx, int by);
void wyswietlZygzak(float zygzak[ROZMIAR_ZYGZAKA]);
void KompresjaBloku(int bx, int by, int ileZostawic);
void KompresjaCalegoObrazu(int ileZostawic);


macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]);
macierz idct(float DCT[rozmiarBloku][rozmiarBloku]);

#endif // SM2025_DCT_H_INCLUDED
