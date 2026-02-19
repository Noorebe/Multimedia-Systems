#ifndef SM2025_PLIKI_H_INCLUDED
#define SM2025_PLIKI_H_INCLUDED

#include <SDL2/SDL.h>
#include <string>
#include <fstream>

struct NaglowekSM {
    char sygnatura[2];
    Uint16 szer;
    Uint16 wys;
    char trybID;
};

void uruchomInterfejs();
void zapisAutorski(std::string nazwa, char tryb);
void odczytAutorski(std::string nazwa);

#endif
