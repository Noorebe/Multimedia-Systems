// metody do algorytmu kwantyzacji (MedianCut)
#ifndef SM2025_MEDIANCUT_H_INCLUDED
#define SM2025_MEDIANCUT_H_INCLUDED

#include <SDL2/SDL.h>

#endif // SM2025_MEDIANCUT_H_INCLUDED

void paletaMedianCutBW();
void paletaMedianCutKolor();
void medianCutBW(int start, int koniec, int iteracja);
void medianCutKolor(int start, int koniec, int iteracja);
void sortujKubelekBW(int start, int koniec);
void sortujKubelek(int start, int koniec, Uint8 sortowanie);
int znajdzSasiadaBW(Uint8 wartosc);
int znajdzSasiada(SDL_Color kolor);
Uint8 najwiekszaRoznica(int start, int koniec);

