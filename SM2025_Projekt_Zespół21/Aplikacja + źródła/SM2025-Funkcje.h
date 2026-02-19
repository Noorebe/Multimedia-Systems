// podstawowe funkcje
#ifndef SM2025_FUNKCJE_H_INCLUDED
#define SM2025_FUNKCJE_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <map> // potrzebne do mapowania
#include <vector>
#include <ostream>


void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void Funkcja7();
void Funkcja8();
void Funkcja9();
Uint8 clamp8bit(float wartosc);
void idle();
int sprawdzKolor(SDL_Color kolor);
bool porownajKolory(SDL_Color kolor1, SDL_Color kolor2);
void zaktualizujTabliceBayera4_2bit();
void zaktualizujTabliceBayera4_8bit();
void zaktualizujTabliceBayera4_1bit();
int dodajKolor(SDL_Color kolor);
Uint8 z24bitowNa8bitow(SDL_Color kolor);
Uint8 z24bitowNa8bitowSzarosci(SDL_Color kolor);
SDL_Color z8bitowNa24bity(Uint8 kolor);
SDL_Color z8bitowNa24bitySzarosci(Uint8 kolor);


void narysujZDedykowanejPaletySzarosci();

void setRGB555(int xx, int yy, Uint8 r, Uint8 g, Uint8 b);
void setRGB555(int xx, int yy, Uint16 rgb555);
void setRGB565(int xx, int yy, Uint8 r, Uint8 g, Uint8 b);
void setRGB565(int xx, int yy, Uint16 rgb565);

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);
void ladujBMP(char const* nazwa, int x, int y);

SDL_Color getRGB555(int xx, int yy);
Uint16 getRGB555_(int xx, int yy);
SDL_Color getRGB565(int xx, int yy);
Uint16 getRGB565_(int xx, int yy);

SDL_Color getRGB555D(int xx, int yy);
Uint16    getRGB555D_(int xx, int yy);
SDL_Color getRGB565D(int xx, int yy);
Uint16    getRGB565D_(int xx, int yy);

int ByteRunKompresja(int wejscie[], int dlugosc, std::ostream& out);

void ByteRunDekompresja(const char* filename);
void pobierzDaneDoKompresji(int wejscie[]);
void wywolajKompresje();

int RLEKompresja(int wejscie[], int dlugosc, std::ostream& out);
void RLEDekompresja(const char* filename);
void wywolajKompresjeRLE();


#endif
