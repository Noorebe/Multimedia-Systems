// zmienne globalne
#ifndef SM2025_ZMIENNE_H_INCLUDED
#define SM2025_ZMIENNE_H_INCLUDED

#include <SDL2/SDL.h>

#define szerokosc 640
#define wysokosc 400


#define tytul "SM2025 - Projekt - Zespol XX"


extern SDL_Window* window;
extern SDL_Surface* screen;

extern SDL_Color paleta8[szerokosc*wysokosc];
extern int ileKolorow;

extern SDL_Color paleta8k[256];
extern SDL_Color paleta8s[256];
extern int ileKolorow;
extern int nrIteracji;

extern SDL_Color paletaNarzucona8bit[256];
extern int tablicaBayera4[4][4];
extern float zaktualizowanaTablicaBayera4[4][4];
extern float zaktualizowanaTablicaBayera4_1[4][4];

//MedianCut
extern int ileKubelkow;
extern Uint8 obrazekSzarosc[320*200];
extern SDL_Color obraz[320*200];


extern SDL_Color paleta_dedykowana[256];
extern float paletaBledow[256];


#endif // SM2025_ZMIENNE_H_INCLUDED
