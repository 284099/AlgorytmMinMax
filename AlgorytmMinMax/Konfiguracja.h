#pragma once

// Definicje kolorów ANSI dla konsoli (Styl Chess.com)
#define ANSI_RESET          "\033[0m"
#define ANSI_BG_JASNE       "\033[48;5;230m" // Kremowe t³o (jasne pola)
#define ANSI_BG_CIEMNE      "\033[48;5;34m"  // Zielone t³o (ciemne pola)
#define ANSI_TEKST_BIALY    "\033[38;5;15m"  // bia³y dla pionków gracza
#define ANSI_TEKST_CZARNY   "\033[38;5;16m"  // czarny dla pionków bota
#define ANSI_POGRUBIENIE    "\033[1m"

enum Pole { PUSTE = 0, BIALY_PIONEK = 1, BIALA_DAMKA = 2, CZARNY_PIONEK = 3, CZARNA_DAMKA = 4 };

struct Ruch {
    int zWiersza, zKolumny;
    int doWiersza, doKolumny;
    bool czyBicie;
};