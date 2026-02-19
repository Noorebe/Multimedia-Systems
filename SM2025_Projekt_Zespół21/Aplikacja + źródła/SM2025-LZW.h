// Algorytm LZW - kompresja i dekompresja
#ifndef SM2025_LZW_H_INCLUDED
#define SM2025_LZW_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <ostream>

struct slowo {
    Uint16 kod;
    Uint8 dlugosc;
    Uint8 element[4096];
    bool wSlowniku;
};

// Funkcje g³ówne - zgodnie z instrukcj¹
int LZWKompresja(int wejscie[], int dlugosc, std::ostream& out);
void LZWDekompresja(const char* filename);
void wywolajKompresjeLZW();

// Funkcje pomocnicze
void LZWInicjalizacja();
int dodajDoSlownika(slowo nowy, bool czywSlowniku = true);
slowo noweSlowo();
slowo noweSlowo(Uint8 znak);
slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak);
void wyswietlSlowo(slowo aktualneSlowo);
int znajdzWSlowniku(slowo szukany);
bool porownajSlowa(slowo slowo1, slowo slowo2);
void wyswietlSlownik();

#endif // SM2025_LZW_H_INCLUDED
