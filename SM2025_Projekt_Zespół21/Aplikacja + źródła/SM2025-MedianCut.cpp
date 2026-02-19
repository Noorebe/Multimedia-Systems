// metody do algorytmu kwantyzacji (MedianCut)
#include <iostream>;
#include "SM2025-MedianCut.h"
#include "SM2025-Zmienne.h"
#include "SM2025-Funkcje.h"
#include "SM2025-Paleta.h"
#include "SM2025-Barwy.h"

using namespace std;

/*
Funkcja wykorzystuje algorytm Median Cut w celu znalezienia najbardziej uniwersalnych 256 odcieni szaroœci do opisania podanego obrazu.
Tworzy paletê dedykowan¹ ze znalezionych kolorów.
*/
void paletaMedianCutBW()
{
    cout << "Trwa dzia³anie algorytmu Median Cut...\n";
    ileKubelkow = 0;
    ileKolorow = 0;
    czyscPalete();
    SDL_Color kolor;
    int szary{};
    int numer{};
    int index{};
    for(int y = 0; y < wysokosc / 2; y++)
    {
        for(int x = 0; x < szerokosc / 2; x++)
        {
            kolor=getPixel(x, y);
            szary = (0.299*kolor.r + 0.587*kolor.g + 0.114*kolor.b); //konwertuj na szary8bit
            obrazekSzarosc[numer] = szary;
            numer++;
        }
    }
    sortujKubelekBW(0, numer); //dla szaroœci sortujemy tylko 1 raz
    medianCutBW(0, numer - 1, 8); //8 iteracji dla 8 bitow, wykonuj rekurencyjnie

    narysujZDedykowanejPaletySzarosci();
}
/*
Rekurencyjna wersja funkcji realizuj¹ca algorytm Median Cut.
@param start pocz¹tek obecnego kube³ka
@param koniec koniec obecnego kube³ka
@param iteracja ile jeszcze podzia³ów ma zostaæ wykonanych rekurencyjnie
*/
void medianCutBW(int start, int koniec, int iteracja)
{
    if(iteracja > 0) //dziel na pó³, dopóki nie dojdziesz do ostatniej iteracji
    {
        int srodek = (start + koniec + 1) / 2;
        medianCutBW(start, srodek - 1, iteracja - 1);
        medianCutBW(srodek, koniec, iteracja - 1);
    }else //w ostatniej iteracji
    {
        int sumaBW{};
        for(int i = start; i <= koniec; i++)
        {
            sumaBW += obrazekSzarosc[i];
        }
        //uœrednij wartoœci wszystkich pikseli w kube³ku i utwórz jeden odcieñ opisuj¹cy wszystkie te barwy
        Uint8 noweBW = sumaBW / (koniec + 1 - start);
        SDL_Color nowyKolor = {noweBW, noweBW, noweBW};
        paleta_dedykowana[ileKubelkow] = nowyKolor;
        ileKubelkow++;
    }
}
/*
Funkcja sortuje piksele obrazu tak, aby najbardziej podobne znalaz³y siê obok siebie.
@param start pocz¹tek sortowanej tablicy
@param koniec koniec sortowanej tablicy
*/
void sortujKubelekBW(int start, int koniec)
{
    int minimum{};
    for(int i = start; i <= koniec; i++)
    {
        minimum = i;
        for(int j = i; j <= koniec; j++)
        {
            if(obrazekSzarosc[j] < obrazekSzarosc[minimum])
            {
                minimum = j;
            }
        }
        swap(obrazekSzarosc[i], obrazekSzarosc[minimum]);
    }
}
/*
Funkcja okreœlaj¹ca najbardziej podobny odcieñ szaroœci z palety dedykowanej do wskazanego koloru.
@param wartosc liczba 8 bitowa okreœlaj¹ca pierwotny odcieñ
@return index w tablicy paleta_dedykowana okreœlaj¹cy najbardzie podobny kolor
*/
int znajdzSasiadaBW(Uint8 wartosc)
{
    int minimum{999};
    int indexMin{0};
    int odleglosc{};
    for(int i = 0; i < 256; i++)
    {
        odleglosc = abs(wartosc - paleta_dedykowana[i].r);
        if(odleglosc < minimum)
        {
            minimum = odleglosc;
            indexMin = i;
        }
    }
    return indexMin;
}

/*
Funkcja wykorzystuje algorytm Median Cut w celu znalezienia najbardziej uniwersalnych 256 barw do opisania podanego obrazu.
Tworzy paletê dedykowan¹ ze znalezionych kolorów.
*/
void paletaMedianCutKolor()
{
    ileKubelkow = 0;
    ileKolorow = 0;
    czyscPalete();
    SDL_Color kolor;
    int numer{}; //index pierwszego wolnego miejsca w tablicy obraz
    int index{};
    for(int y = 0; y < wysokosc / 2; y++)
    {
        for(int x = 0; x < szerokosc / 2; x++)
        {
            kolor=getPixel(x, y);
            obraz[numer] = {kolor.r, kolor.g, kolor.b};
            numer++;
        }
    }
    cout << "Trwa dzia³anie algorytmu Median Cut ukoñczono " << nrIteracji << "/256 etapów\n";
    nrIteracji++;
    sortujKubelek(0, numer, najwiekszaRoznica(0, numer)); //pierwsze sortowanie
    medianCutKolor(0, numer - 1, 8); //8 iteracji dla 8 bitow

    numer = 0;

    for(int y = 0; y < wysokosc / 2; y++)
    {
        for(int x = 0; x < szerokosc / 2; x++)
        {
            kolor = getPixel(x, y);
            index = znajdzSasiada(kolor); //znajduje najbardziej podobny kolor w tablicy wynikowej median Cut
            setPixel(x, y, paleta_dedykowana[index].r, paleta_dedykowana[index].g, paleta_dedykowana[index].b);
        }
    }
    narysujPalete(0, 200, paleta_dedykowana);
    SDL_UpdateWindowSurface(window);
}

/*
Rekurencyjna wersja funkcji realizuj¹ca algorytm Median Cut.
@param start pocz¹tek obecnego kube³ka
@param koniec koniec obecnego kube³ka
@param iteracja ile jeszcze podzia³ów ma zostaæ wykonanych rekurencyjnie
*/
void medianCutKolor(int start, int koniec, int iteracja)
{
    if(iteracja > 0)
    {
        nrIteracji++;
        cout << "Trwa dzia³anie algorytmu Median Cut ukoñczono " << nrIteracji << "/256 etapów\n";
        Uint8 sortowanie = najwiekszaRoznica(start, koniec); //znajduje sk³adow¹, dla której w tym kube³ku ró¿nica miêdzy wartoœci¹ najwiêksz¹ i najmniejsz¹ jest najwiêksza
        sortujKubelek(start, koniec, sortowanie); //sortuje kolory w kube³ku na podstawie znalezionej sk³adowej
        int srodek = (start + koniec + 1) / 2; //znajduje po³owê kube³ka, dziel¹c go na 2 nowe
        medianCutKolor(start, srodek - 1, iteracja - 1); //wykonaj rekurencyjnie dla pierwszej po³owy
        medianCutKolor(srodek, koniec, iteracja - 1); //wykonaj rekurencyjnie dla drugiej po³owy
    }else //gdy wykonano wszystkie podzia³y, czyli podzielono na dok³adnie 256 kube³ki
    {
        int sumaR{};
        int sumaG{};
        int sumaB{};
        for(int i = start; i <= koniec; i++) //dla ka¿dego koloru w kube³ku oblicza œredni¹ sum sk³adowych tworz¹c uœredniony kolor reprezentuj¹cy wszystkie te kolory
        {
            sumaR += obraz[i].r;
            sumaG += obraz[i].g;
            sumaB += obraz[i].b;
        }
        int sredniaR = sumaR / (koniec + 1 - start);
        int sredniaG = sumaG / (koniec + 1 - start);
        int sredniaB = sumaB / (koniec + 1 - start);
        SDL_Color nowyKolor = {sredniaR, sredniaG, sredniaB};
        paleta_dedykowana[ileKubelkow] = nowyKolor; //zapisuje kolor do palety dedykowanej
        ileKubelkow++;
    }
}

/*
Funkcja znajduje sk³adow¹, dla której rónica pomiêdzy najmniejsz¹ a najwiêksz¹ wartoœci¹ w podanym zakresie jest najwiêksza.
@param start pocz¹tek tablicy obraz, w którym szukana jest ró¿nica
@param koniec koniec tablicy obraz, w którym szukana jest ró¿nica
@return 1 -> gdy rónica sk³adowej R jest najwiêksza, 2 -> gdy rónica sk³adowej G jest najwiêksza, 3 -> gdy rónica sk³adowej B jest najwiêksza
*/
Uint8 najwiekszaRoznica(int start, int koniec)
{
    int minR = start;
    int minG = start;
    int minB = start;
    int maxR = start;
    int maxG = start;
    int maxB = start;
    Uint8 roznica{};
    for(int i = start; i <= koniec; i++)
    {
        if(obraz[i].r < obraz[minR].r)
        {
            minR = i;
        }
        if(obraz[i].g < obraz[minG].g)
        {
            minG = i;
        }
        if(obraz[i].b < obraz[minB].b)
        {
            minB = i;
        }
        if(obraz[i].r > obraz[maxR].r)
        {
            maxR = i;
        }
        if(obraz[i].g > obraz[maxG].g)
        {
            maxG = i;
        }
        if(obraz[i].b > obraz[maxB].b)
        {
            maxB = i;
        }
    }
    int roznicaR = obraz[maxR].r - obraz[minR].r;
    int roznicaG = obraz[maxG].g - obraz[minG].g;
    int roznicaB = obraz[maxB].b - obraz[minB].b;
    int rozmicaM = max(max(roznicaB, roznicaG), roznicaR);
    if(rozmicaM == roznicaR)
    {
        roznica = 1;
    }else if(rozmicaM == roznicaG)
    {
        roznica = 2;
    }else
    {
        roznica = 3;
    }
    return roznica;
}
/*
Funkcja sortuj¹ca tablicê obraz wed³ug wskazanego kana³u.
@param start pocz¹tek tablicy obraz, od którego zaczyna siê sortowanie
@param koniec koniec tablicy obraz, na którym koñczy siê sortowanie
@param sortowanie kana³, wed³ug którego siê sortuje 1-> R, 2-> G, 3-> B
*/
void sortujKubelek(int start, int koniec, Uint8 sortowanie)
{
    int minimum{};
    for(int i = start; i <= koniec; i++)
    {
        minimum = i;
        for(int j = i; j <= koniec; j++)
        {
            switch(sortowanie)
            {
            case 1:
                if(obraz[j].r < obraz[minimum].r)
                {
                    minimum = j;
                }
                break;
            case 2:
                if(obraz[j].g < obraz[minimum].g)
                {
                    minimum = j;
                }
                break;
            case 3:
                if(obraz[j].b < obraz[minimum].b)
                {
                    minimum = j;
                }
                break;
            }
        }
        swap(obraz[i], obraz[minimum]);
    }

}
/*
Funkcja okreœlaj¹ca najbardziej podobny kolor z palety dedykowanej do wskazanego koloru.
@param kolor kolor wejœciowy
@return index w tablicy paleta_dedykowana okreœlaj¹cy najbardzie podobny kolor
*/
int znajdzSasiada(SDL_Color kolor)
{
    int minimum{99999};
    int indexMin;
    SDL_Color kolorPaleta;
    float odleglosc;


    for(int i = 0; i < 256; i++) //8bit
    {
        kolorPaleta = paleta_dedykowana[i];
        odleglosc = sqrt((kolor.r - kolorPaleta.r)*(kolor.r - kolorPaleta.r) +
                         (kolor.g - kolorPaleta.g)*(kolor.g - kolorPaleta.g) +
                         (kolor.b - kolorPaleta.b)*(kolor.b - kolorPaleta.b));
        if(odleglosc < minimum)
        {
            minimum = odleglosc;
            indexMin = i;
        }
    }
    return indexMin;

}
