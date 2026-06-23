#include "MinMax.h"
#include <climits>
#include <algorithm>

using namespace std;

// Pomocnicza funkcja generuj¹ca pe³ne stany po wykonaniu ca³ej serii biæ
void generujPelneRuchy(Warcaby gra, Ruch obecnyRuch, int aktualnaTura, vector<Warcaby>& wyniki) {
    gra.wykonajRuch(obecnyRuch);

    if (obecnyRuch.czyBicie) {
        auto kolejneBicia = gra.pobierzBiciaZPola(obecnyRuch.doWiersza, obecnyRuch.doKolumny, aktualnaTura);
        if (!kolejneBicia.empty()) {
            for (const auto& nastRuch : kolejneBicia) {
                generujPelneRuchy(gra, nastRuch, aktualnaTura, wyniki);
            }
            return;
        }
    }
    gra.tura = (aktualnaTura == BIALY_PIONEK) ? CZARNY_PIONEK : BIALY_PIONEK;
    wyniki.push_back(gra);
}

int algorytmMinMax(Warcaby& gra, int glebokosc, int alfa, int beta, bool czyMaksymalizacja) {
    if (glebokosc == 0) return gra.ocenPozycje();

    int aktualnaTura = czyMaksymalizacja ? CZARNY_PIONEK : BIALY_PIONEK;
    vector<Ruch> ruchy = gra.pobierzDozwoloneRuchy(aktualnaTura);

    if (ruchy.empty()) {
        return czyMaksymalizacja ? -1000 + (6 - glebokosc) : 1000 - (6 - glebokosc);
    }

    vector<Warcaby> mozliweStany;
    for (const auto& r : ruchy) {
        generujPelneRuchy(gra, r, aktualnaTura, mozliweStany);
    }

    if (czyMaksymalizacja) {
        int maksOcena = INT_MIN;
        for (const auto& stan : mozliweStany) {
            Warcaby tymczasowaGra = stan;
            int ocena = algorytmMinMax(tymczasowaGra, glebokosc - 1, alfa, beta, false);
            maksOcena = max(maksOcena, ocena);
            alfa = max(alfa, ocena);
            if (beta <= alfa) break;
        }
        return maksOcena;
    }
    else {
        int minOcena = INT_MAX;
        for (const auto& stan : mozliweStany) {
            Warcaby tymczasowaGra = stan;
            int ocena = algorytmMinMax(tymczasowaGra, glebokosc - 1, alfa, beta, true);
            minOcena = min(minOcena, ocena);
            beta = min(beta, ocena);
            if (beta <= alfa) break;
        }
        return minOcena;
    }
}

Ruch znajdzNajlepszyRuchMinMax(Warcaby& gra, int glebokosc) {
    vector<Ruch> ruchy = gra.pobierzDozwoloneRuchy(CZARNY_PIONEK);
    Ruch najlepszyRuch = ruchy[0];
    int najlepszaWartosc = INT_MIN;

    for (const auto& ruch : ruchy) {
        vector<Warcaby> stany;
        generujPelneRuchy(gra, ruch, CZARNY_PIONEK, stany);

        int najlepszyWynikSekwencji = INT_MIN;
        for (const auto& stan : stany) {
            Warcaby tymczasowaGra = stan;
            int wartoscPlanszy = algorytmMinMax(tymczasowaGra, glebokosc - 1, INT_MIN, INT_MAX, false);
            if (wartoscPlanszy > najlepszyWynikSekwencji) {
                najlepszyWynikSekwencji = wartoscPlanszy;
            }
        }

        if (najlepszyWynikSekwencji > najlepszaWartosc) {
            najlepszaWartosc = najlepszyWynikSekwencji;
            najlepszyRuch = ruch;
        }
    }
    return najlepszyRuch;
}