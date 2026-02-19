// zmienne globalne
#include "SM2025-Zmienne.h"

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

SDL_Color paleta8[szerokosc*wysokosc];
int ileKolorow = 0;

SDL_Color paleta8k[256];
SDL_Color paleta8s[256];


SDL_Color paleta_dedykowana[256]; //tablica 256 kolorów paley dedykowanej


SDL_Color paletaNarzucona8bit[256];
//Lookup-table dla tablicy Bayera 4x4
int tablicaBayera4[4][4] = {{6, 14, 8, 16},
                            {10, 2, 12, 4},
                            {7, 15, 5, 13},
                            {11, 3, 9, 1}
                            };
float zaktualizowanaTablicaBayera4[4][4]{};
float zaktualizowanaTablicaBayera4_1[4][4]{};
float paletaBledow[256]{};
//MedianCut
int ileKubelkow = 0;
int nrIteracji = 0;
Uint8 obrazekSzarosc[320*200];
SDL_Color obraz[320*200];
