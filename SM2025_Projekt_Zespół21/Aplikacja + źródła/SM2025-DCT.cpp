#include "SM2025-Funkcje.h"
#include "SM2025-Zmienne.h"
#include "SM2025-Paleta.h"
#include "SM2025-MedianCut.h"
#include "SM2025-Pliki.h"
#include "SM2025-Barwy.h"
#include "SM2025-DCT.h"

#define ROZMIAR_ZYGZAKA (rozmiarBloku * rozmiarBloku)



void wyswietlDane(macierz blok)
{
    cout << "Dane pikselowe w macierzy: "<< endl;
    for(int y=0; y<rozmiarBloku; y++)
    {
        for(int x=0; x<rozmiarBloku; x++)
        {
            cout << setw(4)<<(int)blok.dane[x][y]<<" ";
        }
        cout << endl;
    }
}

void wyswietlDCT(macierz blok)
{
    cout << "Wspolczynnik transformaty w macierzy: "<< endl;
    for(int y=0; y<rozmiarBloku; y++)
    {
        for(int x=0; x<rozmiarBloku; x++)
        {
            cout << fixed << setw(6)<<setprecision(2)<<blok.dct[x][y]<<" ";
        }
        cout << endl;
    }
}

void Funkcja1DCT()
{
    float kolor =0;
    for(int y=0; y<rozmiarBloku; y++)
    {
        for(int x=0; x<rozmiarBloku; x++)
        {
            setPixel(x+rozmiarBloku, y+rozmiarBloku, kolor, kolor, kolor);
            kolor += 256.0/(rozmiarBloku*rozmiarBloku);
        }
    }
    for(int y=1; y<rozmiarBloku; y+=2)
    {
        for(int x=1; x<rozmiarBloku; x+=2)
        {
            setPixel(x+3*rozmiarBloku, y+rozmiarBloku, 255, 255, 255);
            setPixel(x+3*rozmiarBloku-1, y+rozmiarBloku-1, 128, 128, 128);
            setPixel(x+3*rozmiarBloku, y+rozmiarBloku-1,0,0,0);
        }
    }
    SDL_UpdateWindowSurface(window);
}

void Funkcja2DCT()
{
    macierz blokDCT;
    macierz blokDane;
    macierz noweDane;
    cout<<"Pierwszy blok"<<endl;
    for(int y=0; y<rozmiarBloku; y++)
    {
        for(int x=0; x<rozmiarBloku; x++)
        {
            blokDane.dane[x][y] = getPixel(x+rozmiarBloku, y+rozmiarBloku).r;
            blokDane.dct[x][y] = 0;
        }
    }
    wyswietlDane(blokDane);
    cout << endl;
    blokDCT = dct(blokDane.dane);
    blokDCT.dct[2][2] = 200;
    wyswietlDCT(blokDCT);
    cout<<endl;
    noweDane = idct(blokDCT.dct);
    wyswietlDane(noweDane);
    cout << endl;
    for(int y=0; y<rozmiarBloku; y++){
        for(int x=0; x<rozmiarBloku; x++){
            setPixel(x+rozmiarBloku, y+3*rozmiarBloku, noweDane.dane[x][y], noweDane.dane[x][y], noweDane.dane[x][y]);
        }
    }

    cout <<"Drugi blok"<<endl;
    for(int y=0; y<rozmiarBloku; y++)
    {
        for(int x=0; x<rozmiarBloku; x++)
        {
            blokDane.dane[x][y]= getPixel(x+3*rozmiarBloku, y+rozmiarBloku).r;
            blokDane.dct[x][y] = 0;
        }
    }
    wyswietlDane(blokDane);
    cout<<endl;
    blokDCT = dct(blokDane.dane);
    blokDCT.dct[2][2] = 200;
    wyswietlDCT(blokDCT);
    cout << endl;
    noweDane=idct(blokDCT.dct);
    wyswietlDane(noweDane);
    cout << endl;
    for(int y=0; y<rozmiarBloku; y++){
        for(int x=0; x<rozmiarBloku; x++){
            setPixel(x+3*rozmiarBloku, y+3*rozmiarBloku,noweDane.dane[x][y], noweDane.dane[x][y], noweDane.dane[x][y]);
        }
    }

    SDL_UpdateWindowSurface(window);

}

macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku])
{
    float wynik[rozmiarBloku][rozmiarBloku];

    for(int v=0; v< rozmiarBloku; ++v)
    {
        for(int u=0; u< rozmiarBloku; ++u)
        {
            const double cu = (u==0) ? 1.0 / sqrt(2) : 1.0;
            const double cv = (v==0) ? 1.0 / sqrt(2) : 1.0;
            double wspolczynnikDCT=0;

            for(int y=0; y<rozmiarBloku; ++y)
            {
                for(int x=0; x< rozmiarBloku; ++x)
                {

                    double uCosFactor =cos((double)(2*x+1)*M_PI*(double)u / (2*(double)rozmiarBloku));
                    double vCosFactor =cos((double)(2*y+1)*M_PI*(double)v / (2*(double)rozmiarBloku));
                    double pixel = (double)wartosci[x][y];
                    wspolczynnikDCT += pixel*uCosFactor * vCosFactor;
                }
            }
            wspolczynnikDCT *= (2.0/(double)rozmiarBloku)*cu*cv;
            wynik[u][v] = wspolczynnikDCT;
        }
    }

    macierz rezultat;
    for(int j=0; j<rozmiarBloku; j++)
    {
        for(int i=0; i<rozmiarBloku; i++)
        {
            rezultat.dct[i][j] = wynik[i][j];
            rezultat.dane[i][j]= wartosci[i][j];
        }
    }
    return rezultat;
}

macierz idct(float DCT[rozmiarBloku][rozmiarBloku]){
    int wynik[rozmiarBloku][rozmiarBloku];

    for(int x=0; x<rozmiarBloku; ++x){
        for(int y=0; y<rozmiarBloku; ++y){
            double pixel = 0;

            for(int u=0; u<rozmiarBloku; ++u){
                for(int v=0; v<rozmiarBloku; ++v){
                    const double cu = (u==0) ? 1.0 / sqrt(2) : 1.0;
                    const double cv = (v==0) ? 1.0 / sqrt(2) : 1.0;

                    double uCosFactor =cos((double)(2*x+1)*M_PI*(double)u / (2*(double)rozmiarBloku));
                    double vCosFactor =cos((double)(2*y+1)*M_PI*(double)v / (2*(double)rozmiarBloku));
                    double wspolczynnikDCT =DCT[u][v];
                    pixel+= wspolczynnikDCT*uCosFactor*cu*vCosFactor*cv;
                }
            }
            pixel *=(2.0 /(double)rozmiarBloku);
            wynik[x][y] = round(pixel);
        }
    }
    macierz rezultat;
    for(int j =0; j<rozmiarBloku; j++){
        for(int i=0; i<rozmiarBloku; i++){
            if(wynik[i][j]>255) wynik[i][j]=255;
            if(wynik[i][j]<0) wynik[i][j]=0;
            rezultat.dane[i][j]=wynik[i][j];
            rezultat.dct[i][j]=DCT[i][j];
        }
    }
    return rezultat;
}

void konwersjaDoSkaliSzarosci()
{
    for(int y = 0; y < wysokosc; y++)
    {
        for(int x = 0; x < szerokosc; x++)
        {
            SDL_Color pix = getPixel(x, y);

            Uint8 Y = (Uint8)(
                0.299 * pix.r +
                0.587 * pix.g +
                0.114 * pix.b
            );

            setPixel(x, y, Y, Y, Y);
        }
    }

    SDL_UpdateWindowSurface(window);
}

void PodzialNaBloki()
{
    macierz blokDane, blokDCT;

    for(int by = 0; by + rozmiarBloku <= wysokosc; by += rozmiarBloku)
    {
        for(int bx = 0; bx + rozmiarBloku <= szerokosc; bx += rozmiarBloku)
        {
            for(int y = 0; y < rozmiarBloku; y++)
            {
                for(int x = 0; x < rozmiarBloku; x++)
                {
                    blokDane.dane[x][y] =
                        getPixel(bx + x, by + y).r;
                    blokDane.dct[x][y] = 0.0f;
                }
            }

            blokDCT = dct(blokDane.dane);
        }
    }
}

void zbierzZygzakiem(float dct[rozmiarBloku][rozmiarBloku],
                     float zygzak[ROZMIAR_ZYGZAKA])
{
    int x = 0, y = 0, i = 0;
    bool gora = true;

    while(i < ROZMIAR_ZYGZAKA)
    {
        zygzak[i++] = dct[x][y];

        if(gora)
        {
            if(y == 0 || x == rozmiarBloku - 1)
            {
                gora = false;
                if(x < rozmiarBloku - 1) x++;
                else y++;
            }
            else { x++; y--; }
        }
        else
        {
            if(x == 0 || y == rozmiarBloku - 1)
            {
                gora = true;
                if(y < rozmiarBloku - 1) y++;
                else x++;
            }
            else { x--; y++; }
        }
    }
}

void modyfikujZygzak(float zygzak[ROZMIAR_ZYGZAKA], int ileZostawic)
{
    for(int i = 0; i < ROZMIAR_ZYGZAKA; i++)
    {
        if(i >= ileZostawic)
            zygzak[i] = 0.0f;

    }
}

void odbudujZygzak(float zygzak[ROZMIAR_ZYGZAKA],
                   float dct[rozmiarBloku][rozmiarBloku])
{
    int x = 0, y = 0, i = 0;
    bool gora = true;

    while(i < ROZMIAR_ZYGZAKA)
    {
        dct[x][y] = zygzak[i++];

        if(gora)
        {
            if(y == 0 || x == rozmiarBloku - 1)
            {
                gora = false;
                if(x < rozmiarBloku - 1) x++;
                else y++;
            }
            else { x++; y--; }
        }
        else
        {
            if(x == 0 || y == rozmiarBloku - 1)
            {
                gora = true;
                if(y < rozmiarBloku - 1) y++;
                else x++;
            }
            else { x--; y++; }
        }
    }
}

macierz iDCT(macierz blokDCT)
{
    return idct(blokDCT.dct);
}

void RysujBlok(macierz blok, int bx, int by)
{
    for(int y = 0; y < rozmiarBloku; y++)
    {
        for(int x = 0; x < rozmiarBloku; x++)
        {
            Uint8 v = blok.dane[x][y];
            setPixel((bx + x) + szerokosc / 2, by + y , v, v, v);
        }
    }
}



void wyswietlZygzak(float zygzak[ROZMIAR_ZYGZAKA])
{
    cout << "Zygzak:" << endl;
    for(int i = 0; i < ROZMIAR_ZYGZAKA; i++)
        cout << fixed << setprecision(2) << zygzak[i] << " ";
    cout << endl << endl;
}


void KompresjaBloku(int bx, int by, int ileZostawic)
{
    macierz blokDane, blokDCT, noweDane;
    float zygzak[ROZMIAR_ZYGZAKA];

    for(int y = 0; y < rozmiarBloku; y++)
        for(int x = 0; x < rozmiarBloku; x++)
            blokDane.dane[x][y] = getPixel(bx + x, by + y).r;

    blokDCT = dct(blokDane.dane);

    zbierzZygzakiem(blokDCT.dct, zygzak);
    wyswietlZygzak(zygzak);

    modyfikujZygzak(zygzak, ileZostawic);
    odbudujZygzak(zygzak, blokDCT.dct);

    noweDane = idct(blokDCT.dct);
    RysujBlok(noweDane, bx, by);

    SDL_UpdateWindowSurface(window);
}


void KompresjaCalegoObrazu(int ileZostawic)
{
    for(int by = 0; by + rozmiarBloku <= wysokosc; by += rozmiarBloku)
    {
        for(int bx = 0; bx + rozmiarBloku <= szerokosc; bx += rozmiarBloku)
        {
            KompresjaBloku(bx, by, ileZostawic);
        }
    }
}
