#pragma once

#include <vector>
#include <string>
#include "Konfiguracja.h"

class Warcaby {
public:
    Pole plansza[8][8];
    int tura;
    std::string ostatniRuchOpis;

    Warcaby();
    void resetujPlansze();
    bool czyPrzeciwnik(Pole p, int aktualnaTura);
    bool nalezyDoAktualnego(Pole p, int aktualnaTura);
    std::vector<Ruch> pobierzDozwoloneRuchy(int aktualnaTura);
    void wykonajRuch(const Ruch& r);
    int ocenPozycje();
    void wyswietlPlansze();
    void wyswietlInstrukcje();

    // Dodano metodê do pobierania biæ tylko dla jednego pola
    std::vector<Ruch> pobierzBiciaZPola(int w, int k, int aktualnaTura);
};