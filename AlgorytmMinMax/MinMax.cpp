#include "MinMax.h"
#include <climits>
#include <algorithm>

using namespace std;

int algorytmMinMax(Warcaby& gra, int glebokosc, int alfa, int beta, bool czyMaksymalizacja) {
    if (glebokosc == 0) return gra.ocenPozycje();

    int aktualnaTura = czyMaksymalizacja ? CZARNY_PIONEK : BIALY_PIONEK;
    vector<Ruch> ruchy = gra.pobierzDozwoloneRuchy(aktualnaTura);

    if (ruchy.empty()) {
        return czyMaksymalizacja ? -1000 + (6 - glebokosc) : 1000 - (6 - glebokosc);
    }

    if (czyMaksymalizacja) {
        int maksOcena = INT_MIN;
        for (const auto& ruch : ruchy) {
            Warcaby tymczasowaGra = gra;
            tymczasowaGra.wykonajRuch(ruch);
            int ocena = algorytmMinMax(tymczasowaGra, glebokosc - 1, alfa, beta, false);
            maksOcena = max(maksOcena, ocena);
            alfa = max(alfa, ocena);
            if (beta <= alfa) break;
        }
        return maksOcena;
    }
    else {
        int minOcena = INT_MAX;
        for (const auto& ruch : ruchy) {
            Warcaby tymczasowaGra = gra;
            tymczasowaGra.wykonajRuch(ruch);
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
        Warcaby tymczasowaGra = gra;
        tymczasowaGra.wykonajRuch(ruch);
        int wartoscPlanszy = algorytmMinMax(tymczasowaGra, glebokosc - 1, INT_MIN, INT_MAX, false);
        if (wartoscPlanszy > najlepszaWartosc) {
            najlepszaWartosc = wartoscPlanszy;
            najlepszyRuch = ruch;
        }
    }
    return najlepszyRuch;
}