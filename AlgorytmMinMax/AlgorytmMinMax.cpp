#include <iostream>
#include <string>
#include <vector>
#include "Warcaby.h"
#include "MinMax.h"

using namespace std;

int main() {
    Warcaby gra;
    gra.wyswietlInstrukcje();

    while (true) {
        gra.wyswietlPlansze();
        vector<Ruch> dozwoloneRuchy = gra.pobierzDozwoloneRuchy(gra.tura);

        if (dozwoloneRuchy.empty()) {
            if (gra.tura == BIALY_PIONEK) cout << "Koniec gry! Przegrana gracza (Brak ruchow)." << endl;
            else cout << "Koniec gry! Wygrana gracza (Bot nie ma ruchow)." << endl;
            break;
        }

        if (gra.tura == BIALY_PIONEK) {
            cout << "Ruch gracza (np. 'C3 D4'): ";
            string zPolaStr, doPolaStr;
            if (!(cin >> zPolaStr >> doPolaStr)) break;

            if (zPolaStr.length() < 2 || doPolaStr.length() < 2) {
                cout << "Niepoprawny format! Nalezy sprobowac ponownie." << endl;
                continue;
            }

            int zKolumny = toupper(zPolaStr[0]) - 'A';
            int zWiersza = '8' - zPolaStr[1];
            int doKolumny = toupper(doPolaStr[0]) - 'A';
            int doWiersza = '8' - doPolaStr[1];

            bool poprawny = false;
            Ruch wybranyRuch;
            for (const auto& r : dozwoloneRuchy) {
                if (r.zWiersza == zWiersza && r.zKolumny == zKolumny && r.doWiersza == doWiersza && r.doKolumny == doKolumny) {
                    poprawny = true;
                    wybranyRuch = r;
                    break;
                }
            }

            if (poprawny) {
                gra.wykonajRuch(wybranyRuch);
            }
            else {
                if (dozwoloneRuchy[0].czyBicie) {
                    cout << "RUCH NIELEGALNY! Istnieje obowiazkowe bicie!" << endl;
                }
                else {
                    cout << "RUCH NIELEGALNY! Nalezy wybrac poprawne pole." << endl;
                }
            }
        }
        else {
            cout << "Ruch bota (MinMax)... " << endl;
            Ruch ruchAI = znajdzNajlepszyRuchMinMax(gra, 6);
            gra.wykonajRuch(ruchAI);
        }
    }
    return 0;
}