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

                // Obsluga wielokrotnego bicia dla gracza w petli (dowolna liczba bic)
                if (wybranyRuch.czyBicie) {
                    int obecnyWiersz = wybranyRuch.doWiersza;
                    int obecnyKolumna = wybranyRuch.doKolumny;
                    auto kolejneBicia = gra.pobierzBiciaZPola(obecnyWiersz, obecnyKolumna, BIALY_PIONEK);

                    while (!kolejneBicia.empty()) {
                        gra.wyswietlPlansze();

                        cout << "[SERIA BIC!] Wymagane jest kontynuowanie bicia pionkiem z pola "
                            << (char)('A' + obecnyKolumna) << (8 - obecnyWiersz) << endl;
                        cout << "Podaj kolejny ruch (np. 'C3 E5'): ";

                        string nZPolaStr, nDoPolaStr;
                        if (!(cin >> nZPolaStr >> nDoPolaStr)) break;

                        if (nZPolaStr.length() < 2 || nDoPolaStr.length() < 2) {
                            cout << "Niepoprawny format! Nalezy sprobowac ponownie." << endl;
                            continue;
                        }

                        int nZKolumny = toupper(nZPolaStr[0]) - 'A';
                        int nZWiersza = '8' - nZPolaStr[1];
                        int nDoKolumny = toupper(nDoPolaStr[0]) - 'A';
                        int nDoWiersza = '8' - nDoPolaStr[1];

                        bool nPoprawny = false;
                        Ruch nWybranyRuch;
                        for (const auto& r : kolejneBicia) {
                            if (r.zWiersza == nZWiersza && r.zKolumny == nZKolumny &&
                                r.doWiersza == nDoWiersza && r.doKolumny == nDoKolumny) {
                                nPoprawny = true;
                                nWybranyRuch = r;
                                break;
                            }
                        }

                        if (nPoprawny) {
                            gra.wykonajRuch(nWybranyRuch);
                            obecnyWiersz = nWybranyRuch.doWiersza;
                            obecnyKolumna = nWybranyRuch.doKolumny;
                            kolejneBicia = gra.pobierzBiciaZPola(obecnyWiersz, obecnyKolumna, BIALY_PIONEK);
                        }
                        else {
                            cout << "RUCH NIELEGALNY! Nalezy kontynuowac bicie z pola "
                                << (char)('A' + obecnyKolumna) << (8 - obecnyWiersz) << "!" << endl;
                        }
                    }
                }

                // Po zakonczeniu serii bic lub po zwyklym ruchu nastepuje zmiana tury
                gra.tura = CZARNY_PIONEK;
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
            // Obsluga ruchu bota (MinMax) wraz z pelna seria bic
            cout << "Ruch bota (MinMax)... " << endl;
            Ruch ruchAI = znajdzNajlepszyRuchMinMax(gra, 6);
            gra.wykonajRuch(ruchAI);

            if (ruchAI.czyBicie) {
                int obecnyWierszAI = ruchAI.doWiersza;
                int obecnyKolumnaAI = ruchAI.doKolumny;
                auto kolejneBiciaAI = gra.pobierzBiciaZPola(obecnyWierszAI, obecnyKolumnaAI, CZARNY_PIONEK);

                while (!kolejneBiciaAI.empty()) {
                    // Krotka pauza lub wyswietlenie planszy, aby widoczny byl kazdy etap bicia bota
                    gra.wyswietlPlansze();
                    cout << "Bot kontynuuje serie bicia... " << endl;

                    // Wybór kolejnego bicia (w tym miejscu pobierany jest pierwszy dostepny ruch z brzegu, 
                    // poniewaz MinMax obliczyl juz optymalna sciezke wewnatrz generujPelneRuchy)
                    Ruch nRuchAI = kolejneBiciaAI[0];
                    gra.wykonajRuch(nRuchAI);

                    obecnyWierszAI = nRuchAI.doWiersza;
                    obecnyKolumnaAI = nRuchAI.doKolumny;
                    kolejneBiciaAI = gra.pobierzBiciaZPola(obecnyWierszAI, obecnyKolumnaAI, CZARNY_PIONEK);
                }
            }

            // Po zakonczeniu serii bic bota nastepuje zmiana tury
            gra.tura = BIALY_PIONEK;
        }
    }
    return 0;
}