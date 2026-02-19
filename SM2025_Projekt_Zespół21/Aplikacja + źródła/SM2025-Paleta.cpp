// funkcje do redukcji kolorów i tworzenia palet
#include "SM2025-Paleta.h"
#include "SM2025-Zmienne.h"
#include "SM2025-Funkcje.h"
#include "SM2025-MedianCut.h"
#include "SM2025-Barwy.h"
/*
Funkcja tworzy paletę narzuconą opisaną na 8 bitach i konwertuje wyświetlany obraz tak, aby jego kolory były opisane na 8 bitach.
@param xPocz współrzędna x górnego lewego rogu obrazu
@param yPocz współrzędna y górnego lewego rogu obrazu
*/

void paletaNarzucona(int xPocz, int yPocz)
{
    SDL_Color kolor, newColor;
    Uint8 kolor8bit;
    for(int y = 0; y < wysokosc / 2; y++)
    {
        for(int x = 0; x < szerokosc / 2; x++)
        {
            kolor=getPixel(x, y);
            kolor8bit = z24bitowNa8bitow(kolor); //przeskalowanie zakresu tak, aby zmieścił się w 8 bitach
            newColor = z8bitowNa24bity(kolor8bit); //powrót do 24 bitów w celu wyświetlenia na ekranie
            setPixel(x + xPocz, y + yPocz, newColor.r, newColor.g, newColor.b);
        }
    }
    for(int i = 0; i < 256; i++) //utworzenie palety narzuconej składającej się z 256 barw
    {
        paletaNarzucona8bit[i] = z8bitowNa24bity(i);
    }
    narysujPalete(0, 200, paletaNarzucona8bit); //wyświetlenie palety
    SDL_UpdateWindowSurface(window);
}
/*
Funkcja tworzy paletę narzuconą składającą się jedynie z odcieni szarości opisaną na 8 bitach i konwertuje wyświetlany obraz na odcienie szarości i opisuje je na 8 bitach.
@param xPocz współrzędna x górnego lewego rogu obrazu
@param yPocz współrzędna y górnego lewego rogu obrazu
*/
void paletaNarzuconaSzarosci(int xPocz, int yPocz)
{
    SDL_Color kolor, newColor;
    Uint8 szary8bit;
    for(int y = 0; y < wysokosc / 2; y++)
    {
        for(int x = 0; x < szerokosc / 2; x++)
        {
            kolor=getPixel(x, y); //pobierz piksel
            szary8bit = (0.299*kolor.r + 0.587*kolor.g + 0.114*kolor.b);
            newColor = z8bitowNa24bitySzarosci(szary8bit); //powrót do 24 bitów w celu wyświetlenia na ekranie
            setPixel(x + xPocz, y + yPocz, newColor.r, newColor.g, newColor.b);
        }
    }
    SDL_UpdateWindowSurface(window);
    for(int i = 0; i < 256; i++) //utworzenie palety narzuconej składającej się z 256 odcieni szarości
    {
        paletaNarzucona8bit[i] = z8bitowNa24bitySzarosci(i);
    }
    narysujPalete(0, 200, paletaNarzucona8bit); //wyświetlenie palety
    SDL_UpdateWindowSurface(window);
}
/*
Funkcja rysuje wskazaną paletę 256 kolorów w podanym miejscu
@param poczX docelowa współrzędna x początku palety
@param poczY docelowa współrzędna y początku palety
@param paleta paleta do wyświetlenia
*/
void narysujPalete(int poczX, int poczY, SDL_Color paleta[])
{
    int x, y;

    for(int i = 0; i < 256; i++) //256 kolory dla 8 bitów
    {
        y = i / 32;
        x = i % 32;
        for(int row = 0; row < 10; row++)
        {
            for(int col = 0; col < 10; col++)
            {
                setPixel(x * 10 + row + poczX, y * 10 + col + poczY, paleta[i].r, paleta[i].g, paleta[i].b);
            }
        }
    }
}


/*
Funkcja zeruje wartości tablicy globalnej przechowującej informacje o palecie narzuconej obrazu.
*/
void czyscPalete()
{
    for(int i = 0; i < ileKolorow; i++)
    {
        paleta_dedykowana[i] = {0,0,0};
    }
    for(int i = 0; i < 256; i++)
    {
        paletaNarzucona8bit[i] = {0, 0, 0};
    }
    ileKolorow = 0;
}
/*
Funkcja analizuje obraz tworząc dedykowaną paletę kolorów na podstawie najczęściej występujących barw.
Dla obrazu skłądającego się z 256 lub mniej kolorów, program automatycznie stworzy paletę wykrytą zawierającą wszystkie kolory.
Dla obrazu o większej ilości kolorów, zastosowany zostanie algorytm medianCut w celu redukcji kolorów do 256.
*/
void paletaDedykowana()
{
    czyscPalete();
    SDL_Color kolor;
    for(int y = 0; y < wysokosc / 2; y++)
    {
        for(int x = 0; x < wysokosc / 2; x++)
        {
            kolor = getPixel(x, y);
            sprawdzKolor(kolor); //sprawdź czy kolor znajduje się już w palecie, jeśli nie, dodaje go
            if(ileKolorow > 256) //przerwij analize obrazu, paleta musi byc wygenerowana algorytmem Median Cut
            {
                break;
            }
        }
        if(ileKolorow > 256) //przerwij analize obrazu, paleta musi byc wygenerowana algorytmem Median Cut
        {
            break;
        }
    }
    if(ileKolorow <= 256) //obraz składa się z 256 lub mniej kolorów, można stworzyć paletę wykrytą
    {
        cout << "Paleta spełnia ograniczenia 8-bit -> generuje paletę wykrytą" << endl;
        narysujPalete(0, 200, paleta_dedykowana);
    }else
    {
        cout << "Paleta przekracza ograniczenia 8-bit -> generuje palete wykorzystujac algorytm Median Cut" << endl;
        paletaMedianCutKolor();
    }

    SDL_UpdateWindowSurface(window);
}

void wizualizacjaRGB555_565()
{
    const int w = szerokosc / 2;
    const int h = wysokosc  / 2;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            Uint16 p555  = getRGB555_(x, y);
            Uint16 p565  = getRGB565_(x, y);
            Uint16 p555D = getRGB555D_(x, y);
            Uint16 p565D = getRGB565D_(x, y);

            setRGB565(x,       y,       p565);
            setRGB555(x + w,   y,       p555);
            setRGB565(x,       y + h,   p565D);
            setRGB555(x + w,   y + h,   p555D);
        }
    }

    SDL_UpdateWindowSurface(window);
}

