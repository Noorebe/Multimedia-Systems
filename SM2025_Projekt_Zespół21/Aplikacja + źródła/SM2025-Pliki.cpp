#include "SM2025-Pliki.h"
#include "SM2025-Zmienne.h"
#include "SM2025-Funkcje.h"
#include "SM2025-Barwy.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Pomocnicza funkcja do odœwie¿ania okna i obs³ugi zdarzeñ
void odswiez() {
    SDL_PumpEvents();
    SDL_UpdateWindowSurface(window);
}

// Funkcja zapisuj¹ca tylko konkretny fragment ekranu do BMP (usuwa czarne pasy)
void zapiszFragmentEkranu(string nazwa, int x, int y, int w, int h) {
    SDL_Surface* fragment = SDL_CreateRGBSurface(0, w, h, 32,
        screen->format->Rmask, screen->format->Gmask,
        screen->format->Bmask, screen->format->Amask);

    if (fragment == NULL) return;
    SDL_Rect rectZrodlo = { x, y, w, h };
    SDL_BlitSurface(screen, &rectZrodlo, fragment, NULL);
    SDL_SaveBMP(fragment, nazwa.c_str());
    SDL_FreeSurface(fragment);
}

void kopiujDoWyniku() {
    for (int y = 0; y < wysokosc / 2; y++) {
        for (int x = 0; x < szerokosc / 2; x++) {
            SDL_Color p = getPixel(x, y);
            setPixel(x + szerokosc / 2, y, p.r, p.g, p.b);
        }
    }
}

void zapisAutorski(string nazwa, char tryb) {
    ofstream plik(nazwa, ios::binary);
    if (!plik) {
        cout << "BLAD: Nie mozna utworzyc pliku " << nazwa << endl;
        return;
    }

    NaglowekSM nag = {{'S', 'M'}, (Uint16)(szerokosc / 2), (Uint16)(wysokosc / 2), tryb};
    plik.write((char*)&nag, sizeof(nag));

    kopiujDoWyniku();

    if (tryb == 'D' || tryb == 'F' || tryb == 'H' || tryb == 'J') {
        applyFilter(szerokosc / 2, 0, 2);
    }
    if (tryb == 'K') {
        podprobkowanieYCbCr420();
    }

    vector<int> dane;
    for (int y = 0; y < wysokosc / 2; y++) {
        for (int x = 0; x < szerokosc / 2; x++) {
            SDL_Color p = getPixel(x + szerokosc / 2, y);
            if (tryb == 'A' || tryb == 'B') {
                Uint16 p16 = (tryb == 'B') ? getRGB565D_(x, y) : getRGB565_(x, y);
                dane.push_back(p16);
            } else {
                int kolor = (p.r << 16) | (p.g << 8) | p.b;
                dane.push_back(kolor);
            }
        }
    }

    if (tryb == 'E' || tryb == 'F' || tryb == 'I' || tryb == 'J') {
        RLEKompresja(dane.data(), dane.size(), plik);
    } else {
        for (int v : dane) {
            if (tryb <= 'B') plik.write((char*)&v, 2);
            else plik.write((char*)&v, 3);
        }
    }
    plik.close();
    odswiez();
}

void odczytAutorski(string nazwa) {
    ifstream plik(nazwa, ios::binary);
    if (!plik) {
        cout << "BLAD: Nie mozna otworzyc pliku " << nazwa << endl;
        return;
    }

    NaglowekSM nag;
    plik.read((char*)&nag, sizeof(nag));

    cout << "Dekodowanie formatu SM (Tryb: " << nag.trybID << ")..." << endl;

    for(int y=0; y<nag.wys; y++)
        for(int x=0; x<nag.szer; x++)
            setPixel(x, y, 0, 0, 0);

    if (nag.trybID == 'E' || nag.trybID == 'F' || nag.trybID == 'I' || nag.trybID == 'J') {
        int x = 0, y = 0;
        int tag, val;
        while (plik >> tag) {
            if (tag == 0) {
                int n; plik >> n;
                for (int i = 0; i < n; i++) {
                    plik >> val;
                    setPixel(x, y, (val >> 16) & 0xFF, (val >> 8) & 0xFF, val & 0xFF);
                    x++; if (x >= nag.szer) { x = 0; y++; }
                }
                if (n % 2 != 0) { int dummy; plik >> dummy; }
            } else {
                int n = tag;
                plik >> val;
                for (int i = 0; i < n; i++) {
                    setPixel(x, y, (val >> 16) & 0xFF, (val >> 8) & 0xFF, val & 0xFF);
                    x++; if (x >= nag.szer) { x = 0; y++; }
                }
            }
        }
    } else {
        for (int y = 0; y < nag.wys; y++) {
            for (int x = 0; x < nag.szer; x++) {
                if (nag.trybID <= 'B') {
                    Uint16 p16 = 0;
                    plik.read((char*)&p16, 2);
                    setRGB565(x, y, p16);
                } else {
                    Uint8 r, g, b;
                    plik.read((char*)&r, 1);
                    plik.read((char*)&g, 1);
                    plik.read((char*)&b, 1);
                    setPixel(x, y, r, g, b);
                }
            }
        }
    }

    if (nag.trybID == 'D' || nag.trybID == 'F' || nag.trybID == 'H' || nag.trybID == 'J') {
        reverseFilter(0, 0, 2);
    }

    plik.close();
    odswiez();
}

void uruchomInterfejs() {
    string plikWe, plikWy;
    cout << "======================================================" << endl;
    cout << "           SYSTEM KONWERSJI OBRAZOW SM2025            " << endl;
    cout << "======================================================" << endl;
    cout << "KROK 1: Podaj nazwe pliku wejsciowego." << endl;
    cout << "   -> Dla konwersji na autorski: np. obrazek1.bmp" << endl;
    cout << "   -> Dla odczytu autorskiego: np. wynik.bin" << endl;
    cout << "Wybor: "; cin >> plikWe;

    cout << "\nKROK 2: Podaj nazwe pliku wyjsciowego." << endl;
    cout << "   -> Przyklad: wynik.bin (jesli wejscie to .bmp)" << endl;
    cout << "   -> Przyklad: wynik.bmp (jesli wejscie to .bin)" << endl;
    cout << "Wybor: "; cin >> plikWy;

    if (plikWe.find(".bin") != string::npos) {
        odczytAutorski(plikWe);
        zapiszFragmentEkranu(plikWy, 0, 0, szerokosc, wysokosc);
        cout << "\n>>> Sukces! Odtworzono plik i zapisano jako " << plikWy << endl;
    } else {
        ladujBMP(plikWe.c_str(), 0, 0);
        odswiez();

        int g, d, p, k, m;
        char tryb = 'A';

        cout << "\n    PARAMETRY ZAPISU    " << endl;
        cout << "Glebia: [1] 16-bit, [2] 24-bit: "; cin >> g;

        if (g == 1) {
            cout << "Czy zastosowac dithering? [1-Tak, 0-Nie]: "; cin >> d;
            cout << "Czy zastosowac predykcje? [1-Tak, 0-Nie]: "; cin >> p;
            cout << "Czy zastosowac kompresje bezstratna (RLE)? [1-Tak, 0-Nie]: "; cin >> k;
            tryb = (d == 1) ? 'B' : 'A';
        } else {
            cout << "Model: [1] RGB, [2] YCbCr: "; cin >> m;
            if (m == 1) {
                cout << "Czy zastosowac kompresje bezstratna (RLE)? [1-Tak, 0-Nie]: "; cin >> k;
                cout << "Czy zastosowac predykcje? [1-Tak, 0-Nie]: "; cin >> p;
                if (k == 0) tryb = (p == 0) ? 'C' : 'D';
                else tryb = (p == 0) ? 'E' : 'F';
            } else {
                cout << "Rodzaj kompresji:\n   [0] Brak\n   [1] Bezstratna RLE\n   [2] Stratna (Podprobkowanie skladowych)\nWybor: "; cin >> k;
                if (k != 2) {
                    cout << "Czy zastosowac predykcje? [1-Tak, 0-Nie]: "; cin >> p;
                    if (k == 0) tryb = (p == 0) ? 'G' : 'H';
                    else tryb = (p == 0) ? 'I' : 'J';
                } else tryb = 'K';
            }
        }

        cout << "\n>>> Przetwarzanie... Prosze czekac." << endl;
        zapisAutorski(plikWy, tryb);
        cout << ">>> GOTOWE! Wynik zapisano w pliku: " << plikWy << " (Tryb " << tryb << ")" << endl;
        cout << ">>> Na ekranie widzisz: LEWO (Oryginal) | PRAWO (Wynik konwersji)" << endl;
    }
}
