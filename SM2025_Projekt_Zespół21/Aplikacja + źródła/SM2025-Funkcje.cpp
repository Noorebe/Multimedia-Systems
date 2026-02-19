#include "SM2025-Funkcje.h"
#include "SM2025-Zmienne.h"
#include "SM2025-Paleta.h"
#include "SM2025-MedianCut.h"
#include "SM2025-Pliki.h"
#include "SM2025-Barwy.h"
#include "SM2025-LZW.h"
#include "SM2025-DCT.h"
#include <iostream>


void Funkcja1() {
    wywolajKompresjeLZW();
    SDL_UpdateWindowSurface(window);
}


void Funkcja2() {
    Funkcja1DCT();
    Funkcja2DCT();
    konwersjaDoSkaliSzarosci();
    KompresjaCalegoObrazu(64);
    SDL_UpdateWindowSurface(window);
}

void Funkcja3() {
    paletaDedykowana();
    SDL_UpdateWindowSurface(window);
}

void Funkcja4() {
    paletaMedianCutKolor();
    SDL_UpdateWindowSurface(window);
}

void Funkcja5() {
    paletaMedianCutBW();
    SDL_UpdateWindowSurface(window);
}

void Funkcja6() {
    wywolajYUV();
    wywolajYIQ();
    wywolajYCbCr();
    wywolajHSL();
    SDL_UpdateWindowSurface(window);
}

void Funkcja7() {
    wywolajHSL();
    SDL_UpdateWindowSurface(window);
}

void Funkcja8() {
    podprobkowanieYUV420();
    podprobkowanieYIQ420();
    podprobkowanieYCbCr420();
    SDL_UpdateWindowSurface(window);
}


void Funkcja9() {
    for (int yy=0; yy<wysokosc/2; yy++){
        for (int xx=0; xx<szerokosc/2; xx++){
            SDL_Color p = getPixel(xx, yy);
            setPixel(xx + szerokosc/2, yy, p.r, p.g, p.b);
        }
    }

    applyFilter(0, 0, 2);

    reverseFilter(0, 0, 2);

    SDL_UpdateWindowSurface(window);
}

/*
Funkcja ograniczająca wartość liczby do zakresu <0;255>
@param wartosc liczba do ograniczenia
@return ograniczona liczba
*/
Uint8 clamp8bit(float wartosc)
{
    if(wartosc > 255)
    {
        wartosc = 255;
    }else if(wartosc < 0)
    {
        wartosc = 0;
    }
    return wartosc;
}

/*
Funkcja oczekująca na akcję użytkownika.
*/
void idle()
{
    cout << "Press any key to continue...";
    SDL_Event event;
    while (SDL_WaitEvent(&event))
    {
        if(event.type == SDL_KEYDOWN)
        {
            break;
        }
    }
    cout << endl;
}

/*
Konwersja koloru 24bit do 8 bit.
@return liczba 8 bitowa reprezentująca wejściowy kolor.
*/
Uint8 z24bitowNa8bitow(SDL_Color kolor)
{
    int R, G, B;
    Uint8 kolor8bit;
    R = kolor.r;
    G = kolor.g;
    B = kolor.b;
    int newR = round(R*7.0/255.0);
    int newG = round(G*7.0/255.0);
    int newB = round(B*3.0/255.0);
    kolor8bit = (newR<<5) | (newG<<2) | newB;
    return kolor8bit;
}

/*
Konwersja koloru 24bit do odcieni szarości opisanych na 8 bitach.
@return liczba 8 bitowa reprezentująca wejściowy kolor w skali szarości.
*/
Uint8 z24bitowNa8bitowSzarosci(SDL_Color kolor)
{
    return (0.299*kolor.r + 0.587*kolor.g + 0.114*kolor.b);
}

/*
Konwersja koloru 8 bitowego na skalę 24 bitową.
@return kolor opisany przez liczbę wejściową.
*/
SDL_Color z8bitowNa24bity(Uint8 kolor)
{
    SDL_Color kolor24bit;
    int newR, newG, newB;
    newR = (kolor  & (0b11100000)) >> 5;
    newG = (kolor  & (0b00011100)) >> 2;
    newB = (kolor  & (0b00000011));

    kolor24bit.r = newR * 255.0/7.0;
    kolor24bit.g = newG * 255.0/7.0;
    kolor24bit.b = newB * 255.0/3.0;
    return kolor24bit;
}

/*
Konwersja koloru 8 bitowego w skali szarości na skalę 24 bitową.
@return kolor opisany przez liczbę wejściową.
*/
SDL_Color z8bitowNa24bitySzarosci(Uint8 kolor)
{
    SDL_Color kolor24bit;
    Uint8 szary8bit = kolor;
    kolor24bit.r = szary8bit;
    kolor24bit.g = szary8bit;
    kolor24bit.b = szary8bit;
    return kolor24bit;
}

/*
Funkcja sprawdzająca, czy wskazany kolor znajduje się w palecie dedykowanej.
@param kolor kolor wejściowy
@return index w tabeli paleta_dedykowana, pod którym znajduje się ten kolor
*/
int sprawdzKolor(SDL_Color kolor)
{
    if(ileKolorow > 0)
    {
        for(int i = 0; i < ileKolorow; i++)
        {
            if(porownajKolory(kolor, paleta_dedykowana[i]))
            {
                return i;
            }
        }
    }
    return dodajKolor(kolor);
}

/*
Funkcja porównująca 2 kolory.
@param kolor1 pierwszy kolor do porównania
@param kolor2 drugi kolor do porównania
@return true -> kolory są identyczne, false -> kolory są różne
*/
bool porownajKolory(SDL_Color kolor1, SDL_Color kolor2)
{
    if(kolor1.r != kolor2.r)
    {
        return false;
    }
    if(kolor1.g != kolor2.g)
    {
        return false;
    }
    if(kolor1.b != kolor2.b)
    {
        return false;
    }
    return true;
}

/*
Funkcja dodająca wskazany kolor na koniec palety dedykowanej.
@param kolor kolor do dodania
@return index dodanego koloru w tablicy paleta_dedykowana
*/
int dodajKolor(SDL_Color kolor)
{
    int aktualnyKolor = ileKolorow;
    paleta_dedykowana[aktualnyKolor] = kolor;
    ileKolorow++;
    return aktualnyKolor;
}

/*
Funkcja konwertująca bazową tablicę bayera tak, aby operowała na zakresie 2bitów. (dla składowej czerwonej i zielonej)
*/
void zaktualizujTabliceBayera4_2bit()
{
    int zakresWartosci{85};
    float zakres = zakresWartosci;
    int rozmiar{4};
    float podzial = zakres*1.0/(rozmiar*rozmiar);
    for(int i = 0; i < rozmiar; i++)
    {
        for(int j = 0; j < rozmiar; j++)
        {
            zaktualizowanaTablicaBayera4[i][j] = (tablicaBayera4[i][j]*podzial)-podzial/2;
        }
    }
}

/*
Funkcja konwertująca bazową tablicę bayera tak, aby operowała na zakresie 5bitów. (dla skali szarości)
*/
void zaktualizujTabliceBayera4_8bit()
{
    int zakresWartosci{255};
    float zakres = zakresWartosci;
    int rozmiar{4};
    float podzial = zakres*1.0/(rozmiar*rozmiar);
    for(int i = 0; i < rozmiar; i++)
    {
        for(int j = 0; j < rozmiar; j++)
        {
            zaktualizowanaTablicaBayera4[i][j] = (tablicaBayera4[i][j]*podzial)-podzial/2;
        }
    }
}

/*
Funkcja konwertująca bazową tablicę bayera tak, aby operowała na zakresie 1 bitu (dla składowej niebieskiej)
*/
void zaktualizujTabliceBayera4_1bit()
{
    int zakresWartosci{256};
    float zakres = zakresWartosci;
    int rozmiar{4};
    float podzial = zakres*1.0/(rozmiar*rozmiar);
    for(int i = 0; i < rozmiar; i++)
    {
        for(int j = 0; j < rozmiar; j++)
        {
            zaktualizowanaTablicaBayera4_1[i][j] = (tablicaBayera4[i][j]*podzial)-podzial/2;
        }
    }
}

/*
Funkcja Konwertuje obraz na skalę szarości i konwertuje go na posiadaną paletę dedykowaną.
*/
void narysujZDedykowanejPaletySzarosci()
{
    SDL_Color kolor;
    Uint8 szary{};
    Uint8 index{};
    for(int y = 0; y < wysokosc / 2; y++)
    {
        for(int x = 0; x < szerokosc / 2; x++)
        {
            kolor=getPixel(x, y);
            szary = (0.299*kolor.r + 0.587*kolor.g + 0.114*kolor.b);
            index = znajdzSasiadaBW(szary);
            setPixel(x, y, paleta_dedykowana[index].r, paleta_dedykowana[index].g, paleta_dedykowana[index].b);
        }
    }
    narysujPalete(0, 200, paleta_dedykowana);
    SDL_UpdateWindowSurface(window);
}


void setRGB555(int xx, int yy, Uint8 r, Uint8 g, Uint8 b)
{
    if (r > 31) r = 31;
    if (g > 31) g = 31;
    if (b > 31) b = 31;

    setPixel(xx, yy, r << 3, g << 3, b << 3);
}

void setRGB555(int xx, int yy, Uint16 rgb555)
{
    Uint8 r = (rgb555 >> 10) & 0x1F;
    Uint8 g = (rgb555 >>  5) & 0x1F;
    Uint8 b =  rgb555        & 0x1F;

    setPixel(xx, yy, r << 3, g << 3, b << 3);
}

void setRGB565(int xx, int yy, Uint8 r, Uint8 g, Uint8 b)
{
    if (r > 31) r = 31;
    if (g > 63) g = 63;
    if (b > 31) b = 31;

    setPixel(xx, yy, r << 3, g << 2, b << 3);
}

void setRGB565(int xx, int yy, Uint16 rgb565)
{
    Uint8 r = (rgb565 >> 11) & 0x1F;
    Uint8 g = (rgb565 >>  5) & 0x3F;
    Uint8 b =  rgb565        & 0x1F;

    setPixel(xx, yy, r << 3, g << 2, b << 3);
}

static inline Uint8 clamp255_int(int v) {
    if (v < 0) return 0;
    if (v > 255) return 255;
    return (Uint8)v;
}

SDL_Color getRGB555D(int xx, int yy)
{
    SDL_Color p = getPixel(xx, yy);

    int bx = xx & 3, by = yy & 3;
    float T = (tablicaBayera4[by][bx] - 0.5f) / 16.0f;

    int R = (int)(p.r + T * 8.0f);
    int G = (int)(p.g + T * 8.0f);
    int B = (int)(p.b + T * 8.0f);

    SDL_Color o;
    o.r = (Uint8)(((clamp255_int(R) >> 3) & 0x1F) << 3);
    o.g = (Uint8)(((clamp255_int(G) >> 3) & 0x1F) << 3);
    o.b = (Uint8)(((clamp255_int(B) >> 3) & 0x1F) << 3);
    o.a = 255;
    return o;
}

Uint16 getRGB555D_(int xx, int yy)
{
    SDL_Color p = getPixel(xx, yy);

    int bx = xx & 3, by = yy & 3;
    float T = (tablicaBayera4[by][bx] - 0.5f) / 16.0f;

    int R = (int)(p.r + T * 8.0f);
    int G = (int)(p.g + T * 8.0f);
    int B = (int)(p.b + T * 8.0f);

    Uint16 r5 = (Uint16)((clamp255_int(R) >> 3) & 0x1F);
    Uint16 g5 = (Uint16)((clamp255_int(G) >> 3) & 0x1F);
    Uint16 b5 = (Uint16)((clamp255_int(B) >> 3) & 0x1F);

    return (Uint16)((r5 << 10) | (g5 << 5) | b5);
}

SDL_Color getRGB565D(int xx, int yy)
{
    SDL_Color p = getPixel(xx, yy);

    int bx = xx & 3, by = yy & 3;
    float T = (tablicaBayera4[by][bx] - 0.5f) / 16.0f;

    int R = (int)(p.r + T * 8.0f);
    int G = (int)(p.g + T * 4.0f);
    int B = (int)(p.b + T * 8.0f);

    SDL_Color o;
    o.r = (Uint8)(((clamp255_int(R) >> 3) & 0x1F) << 3);
    o.g = (Uint8)(((clamp255_int(G) >> 2) & 0x3F) << 2);
    o.b = (Uint8)(((clamp255_int(B) >> 3) & 0x1F) << 3);
    o.a = 255;
    return o;
}

Uint16 getRGB565D_(int xx, int yy)
{
    SDL_Color p = getPixel(xx, yy);

    int bx = xx & 3, by = yy & 3;
    float T = (tablicaBayera4[by][bx] - 0.5f) / 16.0f;

    int R = (int)(p.r + T * 8.0f);
    int G = (int)(p.g + T * 4.0f);
    int B = (int)(p.b + T * 8.0f);

    Uint16 r5 = (Uint16)((clamp255_int(R) >> 3) & 0x1F);
    Uint16 g6 = (Uint16)((clamp255_int(G) >> 2) & 0x3F);
    Uint16 b5 = (Uint16)((clamp255_int(B) >> 3) & 0x1F);

    return (Uint16)((r5 << 11) | (g6 << 5) | b5);
}


int ByteRunKompresja(int wejscie[], int dlugosc, std::ostream& out) {
    int i = 0;
    int rozmiar = 0;

    while (i < dlugosc) {
        if ((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1])) {
            int j = 0;
            while ((i + j < dlugosc - 1) &&
                   (wejscie[i + j] == wejscie[i + 1 + j]) &&
                   (j < 127)) {
                j++;
            }
            out << -(j + 1) << " " << wejscie[i] << " ";
            rozmiar += 2;
            i += (j + 1);
        } else {
            int j = 0;
            while ((i + j < dlugosc - 1) &&
                   (wejscie[i + j] != wejscie[i + j + 1]) &&
                   (j < 128)) {
                j++;
            }
            if ((i + j == dlugosc - 1) && (j < 128)) j++;
            out << (j - 1) << " ";
            for (int k = 0; k < j; k++) out << wejscie[i + k] << " ";
            rozmiar += 1 + j;
            i += j;
        }
    }
    return rozmiar;
}

void ByteRunDekompresja(const char* filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return;

    int x = 0, y = 0;
    int n, val;

    while (inFile >> n) {
        if (n < 0) {
            inFile >> val;
            for (int i = 0; i < -n; i++) {
                int r = (val >> 16) & 0xFF;
                int g = (val >> 8) & 0xFF;
                int b = val & 0xFF;
                setPixel(x+szerokosc/2, y, r, g, b);
                x++;
                if (x >= szerokosc / 2) { x = 0; y++; }
            }
        } else {
            for (int i = 0; i <= n; i++) {
                inFile >> val;
                int r = (val >> 16) & 0xFF;
                int g = (val >> 8) & 0xFF;
                int b = val & 0xFF;
                setPixel(x+szerokosc/2, y, r, g, b);
                x++;
                if (x >= szerokosc / 2) { x = 0; y++; }
            }
        }
    }
    SDL_UpdateWindowSurface(window);
    inFile.close();
}


int RLEKompresja(int wejscie[], int dlugosc, std::ostream& out)
{
    int i = 0;
    int rozmiar = 0;

    while (i < dlugosc)
    {
        if ((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1]))
        {
            int j = 0;
            while ((i + j < dlugosc - 1) &&
                   (wejscie[i + j] == wejscie[i + j + 1]) &&
                   (j < 254))
            {
                j++;
            }

            int n = j + 1;
            int val = wejscie[i];

            out << n << " " << val << " ";
            rozmiar += 2;

            i += n;
        }
        else
        {
            int j = 0;
            while ((i + j < dlugosc - 1) &&
                   (wejscie[i + j] != wejscie[i + j + 1]) &&
                   (j < 254))
            {
                j++;
            }

            if ((i + j == dlugosc - 1) && (j < 254))
            {
                j++;
            }

            int n = j;

            out << 0 << " " << n << " ";
            rozmiar += 2;

            for (int k = 0; k < n; k++)
            {
                out << wejscie[i + k] << " ";
                rozmiar++;
            }

            if (n % 2 != 0)
            {
                out << 0 << " ";
                rozmiar++;
            }
            i += n;
        }
    }

    return rozmiar;
}


void RLEDekompresja(const char* filename)
{
    std::ifstream inFile(filename);
    if (!inFile.is_open()) return;

    int x = 0, y = 0;
    int tag;

    while (inFile >> tag)
    {
        if (tag == 0)
        {
            int n;
            inFile >> n;

            for (int i = 0; i < n; i++)
            {
                int val;
                inFile >> val;

                int r = (val >> 16) & 0xFF;
                int g = (val >> 8)  & 0xFF;
                int b =  val        & 0xFF;

                setPixel(x + szerokosc/2, y, r, g, b);
                x++;
                if (x >= szerokosc / 2) { x = 0; y++; }
            }
            if (n % 2 != 0)
            {
                int dummy;
                inFile >> dummy;
            }
        }
        else
        {
            int n = tag;
            int val;
            inFile >> val;

            int r = (val >> 16) & 0xFF;
            int g = (val >> 8)  & 0xFF;
            int b =  val        & 0xFF;

            for (int i = 0; i < n; i++)
            {
                setPixel(x + szerokosc/2, y, r, g, b);
                x++;
                if (x >= szerokosc / 2) { x = 0; y++; }
            }
        }
    }

    SDL_UpdateWindowSurface(window);
    inFile.close();
}


void pobierzDaneDoKompresji(int wejscie[]) {
    int idx = 0;
    for (int yy = 0; yy < wysokosc / 2; yy++) {
        for (int xx = 0; xx < szerokosc / 2; xx++) {
            SDL_Color pixel = getPixel(xx, yy);
            int kolor = (pixel.r << 16) | (pixel.g << 8) | pixel.b;
            wejscie[idx++] = kolor;
        }
    }
}

void wywolajKompresje() {
    int dlugosc = (szerokosc/2) * (wysokosc/2);
    int* nieskompresowane = new int[dlugosc];

    pobierzDaneDoKompresji(nieskompresowane);

    std::ofstream outFile("wynik.txt");
    int rozmiarKompresji = ByteRunKompresja(nieskompresowane, dlugosc, outFile);
    outFile.close();

    std::cout << "Rozmiar danych po kompresji: " << rozmiarKompresji << " elementow" << std::endl;

    ByteRunDekompresja("wynik.txt");
}

void wywolajKompresjeRLE()
{
    int dlugosc = (szerokosc/2) * (wysokosc/2);
    int* nieskompresowane = new int[dlugosc];

    pobierzDaneDoKompresji(nieskompresowane);

    std::ofstream outFile("wynik.txt");
    int rozmiarKompresji = RLEKompresja(nieskompresowane, dlugosc, outFile);
    outFile.close();

    std::cout << "Rozmiar danych po kompresji (RLE): "
              << rozmiarKompresji << " elementow" << std::endl;

    RLEDekompresja("wynik.txt");

    delete[] nieskompresowane;
}


void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc))
  {
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);
    int bpp = screen->format->BytesPerPixel;

    Uint8 *p1 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2) * bpp;
    Uint8 *p2 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2) * bpp;
    Uint8 *p3 = (Uint8 *)screen->pixels + (y*2) * screen->pitch + (x*2+1) * bpp;
    Uint8 *p4 = (Uint8 *)screen->pixels + (y*2+1) * screen->pitch + (x*2+1) * bpp;

    switch(bpp)
    {
        case 1:
            *p1 = pixel;
            *p2 = pixel;
            *p3 = pixel;
            *p4 = pixel;
            break;

        case 2:
            *(Uint16 *)p1 = pixel;
            *(Uint16 *)p2 = pixel;
            *(Uint16 *)p3 = pixel;
            *(Uint16 *)p4 = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p1[0] = (pixel >> 16) & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = pixel & 0xff;
                p2[0] = (pixel >> 16) & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = pixel & 0xff;
                p3[0] = (pixel >> 16) & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = pixel & 0xff;
                p4[0] = (pixel >> 16) & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = pixel & 0xff;
            } else {
                p1[0] = pixel & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = (pixel >> 16) & 0xff;
                p2[0] = pixel & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = (pixel >> 16) & 0xff;
                p3[0] = pixel & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = (pixel >> 16) & 0xff;
                p4[0] = pixel & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p1 = pixel;
            *(Uint32 *)p2 = pixel;
            *(Uint32 *)p3 = pixel;
            *(Uint32 *)p4 = pixel;
            break;
        }
    }
}

void setPixelSurface(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
  if ((x>=0) && (x<szerokosc*2) && (y>=0) && (y<wysokosc*2))
  {
    Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);
    int bpp = screen->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    switch(bpp)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
        }
    }
}

SDL_Color getPixel(int x, int y) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        char* pPosition=(char*)screen->pixels ;
        pPosition+=(screen->pitch*y*2) ;
        pPosition+=(screen->format->BytesPerPixel*x*2);
        memcpy(&col, pPosition, screen->format->BytesPerPixel);
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}

SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface) {
    SDL_Color color ;
    Uint32 col = 0 ;
    if ((x>=0) && (x<szerokosc) && (y>=0) && (y<wysokosc)) {
        char* pPosition=(char*)surface->pixels ;
        pPosition+=(surface->pitch*y) ;
        pPosition+=(surface->format->BytesPerPixel*x);
        memcpy(&col, pPosition, surface->format->BytesPerPixel);
        SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    }
    return ( color ) ;
}


void ladujBMP(char const* nazwa, int x, int y) {
    SDL_Surface* bmp = SDL_LoadBMP(nazwa);
    if (!bmp)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    }
    else
    {
        SDL_Color kolor;
        for (int yy=0; yy<bmp->h; yy++) {
			for (int xx=0; xx<bmp->w; xx++) {
				kolor = getPixelSurface(xx, yy, bmp);
				setPixel(xx, yy, kolor.r, kolor.g, kolor.b);
			}
        }
		SDL_FreeSurface(bmp);
        SDL_UpdateWindowSurface(window);
    }
}


void czyscEkran(Uint8 R, Uint8 G, Uint8 B)
{
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_UpdateWindowSurface(window);
}


SDL_Color getRGB555(int xx, int yy)
{
    SDL_Color kolor = getPixel(xx, yy);

    kolor.r = ((kolor.r >> 3) & 0x1F) << 3;
    kolor.g = ((kolor.g >> 3) & 0x1F) << 3;
    kolor.b = ((kolor.b >> 3) & 0x1F) << 3;

    return kolor;
}

Uint16 getRGB555_(int xx, int yy)
{
    SDL_Color kolor = getPixel(xx, yy);

    Uint16 r = (kolor.r >> 3) & 0x1F;
    Uint16 g = (kolor.g >> 3) & 0x1F;
    Uint16 b = (kolor.b >> 3) & 0x1F;

    return (r << 10) | (g << 5) | b;
}

SDL_Color getRGB565(int xx, int yy)
{
    SDL_Color kolor = getPixel(xx, yy);

    kolor.r = ((kolor.r >> 3) & 0x1F) << 3;
    kolor.g = ((kolor.g >> 2) & 0x3F) << 2;
    kolor.b = ((kolor.b >> 3) & 0x1F) << 3;

    return kolor;
}

Uint16 getRGB565_(int xx, int yy)
{
    SDL_Color kolor = getPixel(xx, yy);

    Uint16 r = (kolor.r >> 3) & 0x1F;
    Uint16 g = (kolor.g >> 2) & 0x3F;
    Uint16 b = (kolor.b >> 3) & 0x1F;

    return (r << 11) | (g << 5) | b;
}
