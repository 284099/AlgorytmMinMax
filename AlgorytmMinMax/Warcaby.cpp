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

std::vector<Ruch> Warcaby::pobierzDozwoloneRuchy(int aktualnaTura) {
    vector<Ruch> zwykleRuchy;
    vector<Ruch> bicia;

    for (int w = 0; w < 8; w++) {
        for (int k = 0; k < 8; k++) {
            if (!nalezyDoAktualnego(plansza[w][k], aktualnaTura)) continue;

            Pole p = plansza[w][k];
            bool czyDamka = (p == BIALA_DAMKA || p == CZARNA_DAMKA);

            vector<pair<int, int>> kierunki;
            if (czyDamka || p == CZARNY_PIONEK) 
            {
                    kierunki.push_back({ 1, -1 }); 
                    kierunki.push_back({ 1, 1 }); 
            }
            if (czyDamka || p == BIALY_PIONEK) 
            { 
                kierunki.push_back({ -1, -1 }); 
                kierunki.push_back({ -1, 1 }); 
            }

            for (auto kierunek : kierunki) {
                int nw = w + kierunek.first;
                int nk = k + kierunek.second;

                if (nw >= 0 && nw < 8 && nk >= 0 && nk < 8) {
                    if (plansza[nw][nk] == PUSTE) {
                        zwykleRuchy.push_back({ w, k, nw, nk, false });
                    }
                    else if (czyPrzeciwnik(plansza[nw][nk], aktualnaTura)) {
                        int nnw = nw + kierunek.first;
                        int nnk = nk + kierunek.second;
                        if (nnw >= 0 && nnw < 8 && nnk >= 0 && nnk < 8 && plansza[nnw][nnk] == PUSTE) {
                            bicia.push_back({ w, k, nnw, nnk, true });
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
        int srodekWiersza = (r.zWiersza + r.doWiersza) / 2;
        int srodekKolumny = (r.zKolumny + r.doKolumny) / 2;
        plansza[srodekWiersza][srodekKolumny] = PUSTE;
    }

    if (p == BIALY_PIONEK && r.doWiersza == 0) p = BIALA_DAMKA;
    if (p == CZARNY_PIONEK && r.doWiersza == 7) p = CZARNA_DAMKA;

    plansza[r.doWiersza][r.doKolumny] = p;

    char zK = 'A' + r.zKolumny;
    char doK = 'A' + r.doKolumny;
    ostatniRuchOpis = string(1, zK) + to_string(8 - r.zWiersza) + " -> " + string(1, doK) + to_string(8 - r.doWiersza) + (r.czyBicie ? " [BICIE]" : "");

    tura = (tura == BIALY_PIONEK) ? CZARNY_PIONEK : BIALY_PIONEK;
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