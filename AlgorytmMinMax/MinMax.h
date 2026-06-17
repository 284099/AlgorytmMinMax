#pragma once

#include "Warcaby.h"
#include "Konfiguracja.h"

int algorytmMinMax(Warcaby& gra, int glebokosc, int alfa, int beta, bool czyMaksymalizacja);
Ruch znajdzNajlepszyRuchMinMax(Warcaby& gra, int glebokosc);