#include "SM2025-LZW.h"
#include "SM2025-Zmienne.h"
#include "SM2025-Funkcje.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

int rozmiarSlownika = 0;
slowo slownik[65535];

void LZWInicjalizacja() {
    rozmiarSlownika = 0;

    for (int s = 0; s <= 17; s++) {
        slownik[s].kod = s;
        slownik[s].dlugosc = 1;
        slownik[s].element[0] = s;
        memset(&slownik[s].element[1], 0, sizeof(slownik[s].element) - 1);
        slownik[s].wSlowniku = true;
        rozmiarSlownika++;
    }
}

int LZWKompresja(int wejscie[], int dlugosc, std::ostream& out) {
    LZWInicjalizacja();

    slowo aktualneSlowo = noweSlowo();
    slowo slowoZnak;
    Uint8 znak;
    int kod = -1;
    int i = 0;
    int rozmiarWyjscia = 0;

    while (i < dlugosc) {
        znak = wejscie[i];
        cout << "pobieramy znak " << (int)znak << " z pozycji " << i << endl;

        slowoZnak = polaczSlowo(aktualneSlowo, znak);
        cout << "aktualne slowo: ";
        wyswietlSlowo(aktualneSlowo);
        cout << "slowo+znak: ";
        wyswietlSlowo(slowoZnak);

        kod = znajdzWSlowniku(slowoZnak);
        cout << "czy w slowniku? ";

        if (kod < 0) {
            cout << "NIE" << endl;

            if (aktualneSlowo.dlugosc > 0) {
                cout << "na wyjscie: [" << aktualneSlowo.kod << "]" << endl;
                out << aktualneSlowo.kod << " ";
                rozmiarWyjscia++;
            }

            if (rozmiarSlownika < 65535) {
                slowoZnak.kod = dodajDoSlownika(slowoZnak, true);
            } else {
                cout << "UWAGA: Slownik zapelniony!" << endl;
            }

            aktualneSlowo = noweSlowo(znak);
            int kodZnaku = znajdzWSlowniku(aktualneSlowo);
            if (kodZnaku >= 0) {
                aktualneSlowo = slownik[kodZnaku];
            } else {
                cout << "Dodaje pojedynczy znak do slownika: " << (int)znak << endl;
                aktualneSlowo.kod = dodajDoSlownika(aktualneSlowo, true);
            }
        }
        else {
            cout << "TAK: [" << kod << "]" << endl;
            aktualneSlowo = slowoZnak;
            aktualneSlowo.kod = kod;
            aktualneSlowo.wSlowniku = true;
        }

        i++;
    }

    cout << "koniec danych" << endl;

    if (aktualneSlowo.dlugosc > 0) {
        cout << "na wyjscie: [" << aktualneSlowo.kod << "]" << endl;
        out << aktualneSlowo.kod << " ";
        rozmiarWyjscia++;
    }

    cout << endl;
    cout << "aktualny slownik:" << endl;
    wyswietlSlownik();

    return rozmiarWyjscia;
}

void LZWDekompresja(const char* filename) {
    LZWInicjalizacja();

    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Nie można otworzyć pliku: " << filename << endl;
        return;
    }

    vector<int> skompresowane;
    int kod;
    while (inFile >> kod) {
        skompresowane.push_back(kod);
    }
    inFile.close();

    int dlugosc = skompresowane.size();

    slowo aktualneSlowo = noweSlowo();
    slowo poprzednieSlowo = noweSlowo();
    int i = 0;

    cout << "skompresowane wejscie (LZW):" << endl;
    for (int c = 0; c < dlugosc; c++) {
        cout << skompresowane[c] << " ";
    }
    cout << endl;
    cout << "wyjscie:" << endl;

    while (i < dlugosc) {
        kod = skompresowane[i];
        cout << "pobieramy znak " << kod << " z pozycji " << i << endl;

        if (kod < rozmiarSlownika) {
            cout << "czy w slowniku? TAK: [" << kod << "]" << endl;

            aktualneSlowo = slownik[kod];
            aktualneSlowo.wSlowniku = true;

            cout << "na wyjscie: ";
            for (int j = 0; j < aktualneSlowo.dlugosc; j++) {
                cout << (int)aktualneSlowo.element[j];
                if (j < aktualneSlowo.dlugosc - 1) cout << ", ";
            }
            cout << endl;

            if (poprzednieSlowo.dlugosc > 0 && rozmiarSlownika < 65535) {
                slowo nowe = polaczSlowo(poprzednieSlowo, aktualneSlowo.element[0]);
                if (znajdzWSlowniku(nowe) < 0) {
                    cout << "dodajemy do slownika: ";
                    dodajDoSlownika(nowe, true);
                }
            }

        } else {
            cout << "czy w slowniku? NIE (specjalny przypadek)" << endl;

            if (poprzednieSlowo.dlugosc > 0) {
                aktualneSlowo = polaczSlowo(poprzednieSlowo, poprzednieSlowo.element[0]);

                cout << "na wyjscie: ";
                for (int j = 0; j < aktualneSlowo.dlugosc; j++) {
                    cout << (int)aktualneSlowo.element[j];
                    if (j < aktualneSlowo.dlugosc - 1) cout << ", ";
                }
                cout << endl;

                if (rozmiarSlownika < 65535) {
                    cout << "dodajemy do slownika: ";
                    dodajDoSlownika(aktualneSlowo, true);
                } else {
                    cout << "UWAGA: Slownik zapelniony!" << endl;
                }
            }
        }

        poprzednieSlowo = aktualneSlowo;
        i++;
    }

    cout << "koniec danych" << endl;
    cout << endl;
    cout << "aktualny slownik:" << endl;
    wyswietlSlownik();
}

void wywolajKompresjeLZW() {

    int nieskompresowane[] = {0, 1, 0, 1, 0, 1, 2, 3, 2, 3, 2, 3, 4, 5, 4, 5, 6, 7, 6, 7};
    int dlugosc = 20;

    cout << "Wejscie:" << endl;
    for (int c = 0; c < dlugosc; c++) {
        cout << nieskompresowane[c] << " ";
    }
    cout << endl << endl;

    cout << "=== KOMPRESJA ===" << endl;

    ofstream outFile("wynik_lzw.txt");
    int rozmiarSkompresowanych = LZWKompresja(nieskompresowane, dlugosc, outFile);
    outFile.close();

    cout << endl << "Rozmiar danych przed kompresja: " << dlugosc << " elementow" << endl;
    cout << "Rozmiar danych po kompresji: " << rozmiarSkompresowanych << " elementow" << endl;

    if (rozmiarSkompresowanych > 0) {
        float stopienKompresji = (float)dlugosc / (float)rozmiarSkompresowanych;
        cout << "Stopien kompresji: " << stopienKompresji << endl;
    }

    cout << endl << "=== DEKOMPRESJA ===" << endl;
    LZWDekompresja("wynik_lzw.txt");
}

int dodajDoSlownika(slowo nowy, bool czywSlowniku) {
    if (rozmiarSlownika < 65535) {
        Uint16 nr = rozmiarSlownika;
        slownik[nr].kod = nr;
        slownik[nr].dlugosc = nowy.dlugosc;
        copy(begin(nowy.element), end(nowy.element), begin(slownik[nr].element));
        slownik[nr].wSlowniku = true;

        if (czywSlowniku) {
            wyswietlSlowo(slownik[nr]);
        }

        rozmiarSlownika++;
        return nr;
    }
    return -1;
}

slowo noweSlowo() {
    slowo noweSlowo;
    noweSlowo.kod = 0;
    noweSlowo.dlugosc = 0;
    memset(noweSlowo.element, 0, sizeof(noweSlowo.element));
    noweSlowo.wSlowniku = false;
    return noweSlowo;
}

slowo noweSlowo(Uint8 znak) {
    slowo noweSlowo;
    noweSlowo.kod = 0;
    noweSlowo.dlugosc = 1;
    noweSlowo.element[0] = znak;
    memset(&noweSlowo.element[1], 0, sizeof(noweSlowo.element) - 1);
    noweSlowo.wSlowniku = false;
    return noweSlowo;
}

slowo polaczSlowo(slowo aktualneSlowo, Uint8 znak) {
    slowo noweSl;

    if (aktualneSlowo.dlugosc < 4096) {
        noweSl.kod = 0;
        noweSl.dlugosc = aktualneSlowo.dlugosc + 1;

        for (int i = 0; i < aktualneSlowo.dlugosc; i++) {
            noweSl.element[i] = aktualneSlowo.element[i];
        }

        noweSl.element[aktualneSlowo.dlugosc] = znak;

        for (int i = noweSl.dlugosc; i < 4096; i++) {
            noweSl.element[i] = 0;
        }

        noweSl.wSlowniku = false;
        return noweSl;
    } else {
        cout << "Przekroczono rozmiar znakow w pojedynczym slowie" << endl;
        noweSl.kod = 0;
        noweSl.dlugosc = 1;
        noweSl.element[0] = znak;
        memset(&noweSl.element[1], 0, sizeof(noweSl.element) - 1);
        noweSl.wSlowniku = false;
        return noweSl;
    }
}

void wyswietlSlowo(slowo aktualneSlowo) {
    if (aktualneSlowo.wSlowniku) {
        cout << "[" << aktualneSlowo.kod << "]";
    } else {
        cout << "[x] ";
        for (int s = 0; s < aktualneSlowo.dlugosc; s++) {
            cout << (int)aktualneSlowo.element[s];
            if (s < aktualneSlowo.dlugosc - 1) {
                cout << ", ";
            }
        }
    }
    cout << endl;
}

int znajdzWSlowniku(slowo szukany) {
    for (int nr = 0; nr < rozmiarSlownika; nr++) {
        if (porownajSlowa(slownik[nr], szukany)) {
            return nr;
        }
    }
    return -1;
}

bool porownajSlowa(slowo slowo1, slowo slowo2) {
    if (slowo1.dlugosc != slowo2.dlugosc) {
        return false;
    }

    for (int i = 0; i < slowo1.dlugosc; i++) {
        if (slowo1.element[i] != slowo2.element[i]) {
            return false;
        }
    }

    return true;
}

void wyswietlSlownik() {
    for (int nr = 0; nr < rozmiarSlownika; nr++) {
        cout << "[" << nr << "] ";
        for (int s = 0; s < slownik[nr].dlugosc; s++) {
            cout << (int)slownik[nr].element[s];
            if (s < slownik[nr].dlugosc - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}
