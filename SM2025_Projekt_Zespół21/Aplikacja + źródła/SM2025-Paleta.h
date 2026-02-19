// funkcje do redukcji kolorów i tworzenia palet
#ifndef SM2025_PALETA_H_INCLUDED
#define SM2025_PALETA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>
using namespace std;
void paletaNarzucona(int xPocz = 0, int yPocz = 0);
void narysujPalete(int, int, SDL_Color[]);
void narysujPaleteSzarosci(int poczX, int poczY, SDL_Color pal8[]);
void paletaNarzuconaSzarosci(int xPocz = 0, int yPocz = 0);
void paletaNarzuconaDitteringKolor();
void paletaNarzuconaDitteringSzary();
void paletaWykryta();
void czyscPalete();
void paletaDedykowana();
void paletaDedykowanaDithering();
void wizualizacjaRGB555_565();
#endif // SM2025_PALETA_H_INCLUDED
