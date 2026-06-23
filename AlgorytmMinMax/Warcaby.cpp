#include "Warcaby.h"
#include <iostream>
#include <algorithm>

using namespace std;

Warcaby::Warcaby() {
    resetujPlansze();
}

void Warcaby::resetujPlansze() {
    tura = BIALY_PIONEK;
    ostatniRuchOpis = "Brak (Poczatek gry)";
    for (int w = 0; w < 8; w++) {
        for (int k = 0; k < 8; k++) {
            plansza[w][k] = PUSTE;
            if ((w + k) % 2 != 0) {
                if (w < 3) plansza[w][k] = CZARNY_PIONEK;
                else if (w > 4) plansza[w][k] = BIALY_PIONEK;
            }
        }
    }
}

bool Warcaby::czyPrzeciwnik(Pole p, int aktualnaTura) {
    if (aktualnaTura == BIALY_PIONEK || aktualnaTura == BIALA_DAMKA) {
        return p == CZARNY_PIONEK || p == CZARNA_DAMKA;
    }
    else {
        return p == BIALY_PIONEK || p == BIALA_DAMKA;
    }
}

bool Warcaby::nalezyDoAktualnego(Pole p, int aktualnaTura) {
    if (aktualnaTura == BIALY_PIONEK || aktualnaTura == BIALA_DAMKA) {
        return p == BIALY_PIONEK || p == BIALA_DAMKA;
    }
    else {
        return p == CZARNY_PIONEK || p == CZARNA_DAMKA;
    }
}

// Pobieranie dostêpnych biæ dla wybranego pionka z uwzglêdnieniem d³ugiego zasiêgu damki
std::vector<Ruch> Warcaby::pobierzBiciaZPola(int w, int k, int aktualnaTura) {
    vector<Ruch> bicia;
    if (!nalezyDoAktualnego(plansza[w][k], aktualnaTura)) return bicia;

    Pole p = plansza[w][k];
    bool czyDamka = (p == BIALA_DAMKA || p == CZARNA_DAMKA);

    vector<pair<int, int>> kierunki = { {1, -1}, {1, 1}, {-1, -1}, {-1, 1} };

    for (auto kierunek : kierunki) {
        int nw = w + kierunek.first;
        int nk = k + kierunek.second;

        if (czyDamka) {
            // Logika dalekiego bicia dla damki
            bool znalezionoPrzeciwnika = false;
            int wrogWiersz = -1, wrogKolumna = -1;

            while (nw >= 0 && nw < 8 && nk >= 0 && nk < 8) {
                if (plansza[nw][nk] == PUSTE) {
                    if (znalezionoPrzeciwnika) {
                        // Jeœli wczeœniej miniêto wroga, ka¿de puste pole za nim jest prawid³owym l¹dowaniem bicia
                        bicia.push_back({ w, k, nw, nk, true });
                    }
                }
                else if (czyPrzeciwnik(plansza[nw][nk], aktualnaTura)) {
                    if (znalezionoPrzeciwnika) break; // Napotkanie drugiego wroga blokuje liniê
                    znalezionoPrzeciwnika = true;
                    wrogWiersz = nw;
                    wrogKolumna = nk;
                }
                else {
                    break; // Napotkanie w³asnego pionka blokuje liniê
                }
                nw += kierunek.first;
                nk += kierunek.second;
            }
        }
        else {
            // Logika bicia dla zwyk³ego pionka (zasiêg o 1 pole przed siebie/za siebie)
            if (nw >= 0 && nw < 8 && nk >= 0 && nk < 8) {
                if (czyPrzeciwnik(plansza[nw][nk], aktualnaTura)) {
                    int nnw = nw + kierunek.first;
                    int nnk = nk + kierunek.second;
                    if (nnw >= 0 && nnw < 8 && nnk >= 0 && nnk < 8 && plansza[nnw][nnk] == PUSTE) {
                        bicia.push_back({ w, k, nnw, nnk, true });
                    }
                }
            }
        }
    }
    return bicia;
}

// Aktualizacja g³ównej metody generowania ruchów, aby równie¿ uwzglêdnia³a daleki zasiêg damki
std::vector<Ruch> Warcaby::pobierzDozwoloneRuchy(int aktualnaTura) {
    vector<Ruch> zwykleRuchy;
    vector<Ruch> bicia;

    for (int w = 0; w < 8; w++) {
        for (int k = 0; k < 8; k++) {
            if (!nalezyDoAktualnego(plansza[w][k], aktualnaTura)) continue;

            Pole p = plansza[w][k];
            bool czyDamka = (p == BIALA_DAMKA || p == CZARNA_DAMKA);

            // Wyszukiwanie biæ dla danego pola
            vector<Ruch> lokalneBicia = pobierzBiciaZPola(w, k, aktualnaTura);
            bicia.insert(bicia.end(), lokalneBicia.begin(), lokalneBicia.end());

            // Generowanie zwyk³ych ruchów, jeœli lista biæ jest pusta
            if (bicia.empty()) {
                vector<pair<int, int>> kierunki;
                if (czyDamka || p == CZARNY_PIONEK) {
                    kierunki.push_back({ 1, -1 });
                    kierunki.push_back({ 1, 1 });
                }
                if (czyDamka || p == BIALY_PIONEK) {
                    kierunki.push_back({ -1, -1 });
                    kierunki.push_back({ -1, 1 });
                }

                for (auto kierunek : kierunki) {
                    int nw = w + kierunek.first;
                    int nk = k + kierunek.second;

                    if (czyDamka) {
                        while (nw >= 0 && nw < 8 && nk >= 0 && nk < 8 && plansza[nw][nk] == PUSTE) {
                            zwykleRuchy.push_back({ w, k, nw, nk, false });
                            nw += kierunek.first;
                            nk += kierunek.second;
                        }
                    }
                    else {
                        if (nw >= 0 && nw < 8 && nk >= 0 && nk < 8 && plansza[nw][nk] == PUSTE) {
                            zwykleRuchy.push_back({ w, k, nw, nk, false });
                        }
                    }
                }
            }
        }
    }
    return !bicia.empty() ? bicia : zwykleRuchy;
}

void Warcaby::wykonajRuch(const Ruch& r) {
    Pole p = plansza[r.zWiersza][r.zKolumny];
    plansza[r.zWiersza][r.zKolumny] = PUSTE;

    if (r.czyBicie) {
        // Wyznaczanie kierunku ruchu w celu znalezienia i usuniêcia pionka przeciwnika
        int kierunekWiersza = (r.doWiersza > r.zWiersza) ? 1 : -1;
        int kierunekKolumny = (r.doKolumny > r.zKolumny) ? 1 : -1;

        int sprawdzanyWiersz = r.zWiersza + kierunekWiersza;
        int sprawdzanyKolumna = r.zKolumny + kierunekKolumny;

        // Przeszukiwanie linii w celu wyczyszczenia napotkanego wroga
        while (sprawdzanyWiersz != r.doWiersza && sprawdzanyKolumna != r.doKolumny) {
            if (plansza[sprawdzanyWiersz][sprawdzanyKolumna] != PUSTE) {
                plansza[sprawdzanyWiersz][sprawdzanyKolumna] = PUSTE;
                break;
            }
            sprawdzanyWiersz += kierunekWiersza;
            sprawdzanyKolumna += kierunekKolumny;
        }
    }

    if (p == BIALY_PIONEK && r.doWiersza == 0) p = BIALA_DAMKA;
    if (p == CZARNY_PIONEK && r.doWiersza == 7) p = CZARNA_DAMKA;

    plansza[r.doWiersza][r.doKolumny] = p;

    char zK = 'A' + r.zKolumny;
    char doK = 'A' + r.doKolumny;
    ostatniRuchOpis = string(1, zK) + to_string(8 - r.zWiersza) + " -> " + string(1, doK) + to_string(8 - r.doWiersza) + (r.czyBicie ? " [BICIE]" : "");
}

int Warcaby::ocenPozycje() {
    int wynik = 0;
    for (int w = 0; w < 8; w++) {
        for (int k = 0; k < 8; k++) {
            if (plansza[w][k] == CZARNY_PIONEK) wynik += 10;
            else if (plansza[w][k] == CZARNA_DAMKA) wynik += 17;
            else if (plansza[w][k] == BIALY_PIONEK) wynik -= 10;
            else if (plansza[w][k] == BIALA_DAMKA) wynik -= 17;
        }
    }
    return wynik;
}

void Warcaby::wyswietlInstrukcje() {
    cout << "========================================================\n";
    cout << "                    ZASADY GRY                          \n";
    cout << "========================================================\n";
    cout << "1. Gra pionkami BIALYMI (O) nalezy do gracza, Bot gra CZARNYMI (O).\n";
    cout << "2. Zwykle pionki poruszaja sie o 1 pole do przodu po skosie.\n";
    cout << "3. BICIE JEST OBOWIAZKOWE! W przypadku mozliwosci bicia, nalezy je wykonac.\n";
    cout << "4. Pionek na koncu planszy staje sie Damka (K) i moze sie cofac.\n";
    cout << "========================================================\n";
    cout << "                INSTRUKCJA STEROWANIA                   \n";
    cout << "========================================================\n";
    cout << "Ruch nalezy wpisac, podajac pole startowe i docelowe rozdzielone spacja.\n";
    cout << "Wielkosc liter nie ma znaczenia (np. 'c3 d4' lub 'C3 D4').\n";
    cout << "Przyklad bicia: 'C3 E5'\n";
    cout << "========================================================\n\n";
}

void Warcaby::wyswietlPlansze() {
    cout << "--------------------------------------------\n";
    cout << " OSTATNI RUCH: " << ostatniRuchOpis << "\n";
    cout << "--------------------------------------------\n";
    cout << "\n    A  B  C  D  E  F  G  H\n";
    for (int w = 0; w < 8; w++) {
        cout << " " << 8 - w << " ";
        for (int k = 0; k < 8; k++) {
            if ((w + k) % 2 == 0) cout << ANSI_BG_JASNE;
            else cout << ANSI_BG_CIEMNE;

            switch (plansza[w][k]) {
            case BIALY_PIONEK: cout << ANSI_TEKST_BIALY << ANSI_POGRUBIENIE << " O " << ANSI_RESET; break;
            case BIALA_DAMKA:  cout << ANSI_TEKST_BIALY << ANSI_POGRUBIENIE << " K " << ANSI_RESET; break;
            case CZARNY_PIONEK: cout << ANSI_TEKST_CZARNY << ANSI_POGRUBIENIE << " O " << ANSI_RESET; break;
            case CZARNA_DAMKA:  cout << ANSI_TEKST_CZARNY << ANSI_POGRUBIENIE << " K " << ANSI_RESET; break;
            default:            cout << "   " << ANSI_RESET; break;
            }
        }
        cout << " " << 8 - w << "\n";
    }
    cout << "    A  B  C  D  E  F  G  H\n\n";
}