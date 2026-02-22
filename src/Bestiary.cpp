#include "Bestiary.h"
#include "TUI.h"
#include <fstream>
#include <iostream>
using namespace std;

BestiaryEntry bestiary[MAX_BESTIARY];
int bestiaryCount = 0;

void initBestiary() {
    bestiaryCount = 0;
    for (int i = 0; i < MAX_BESTIARY; i++) {
        bestiary[i] = {"", 0, 0, 0, 0, false};
    }
}

void logEncounter(const string& name, int maxHp, int atk, int def) {
    // Check if already in bestiary
    for (int i = 0; i < bestiaryCount; i++) {
        if (bestiary[i].name == name) return; // already logged
    }
    if (bestiaryCount >= MAX_BESTIARY) return;
    bestiary[bestiaryCount].name = name;
    bestiary[bestiaryCount].maxHp = maxHp;
    bestiary[bestiaryCount].attack = atk;
    bestiary[bestiaryCount].defense = def;
    bestiary[bestiaryCount].timesDefeated = 0;
    bestiary[bestiaryCount].encountered = true;
    bestiaryCount++;
}

void logKill(const string& name) {
    for (int i = 0; i < bestiaryCount; i++) {
        if (bestiary[i].name == name) {
            bestiary[i].timesDefeated++;
            return;
        }
    }
}

int getTotalKills() {
    int total = 0;
    for (int i = 0; i < bestiaryCount; i++)
        total += bestiary[i].timesDefeated;
    return total;
}

int getUniqueEncounters() {
    return bestiaryCount;
}

void saveBestiary(const string& filename) {
    ofstream f(filename);
    if (!f.is_open()) return;
    f << bestiaryCount << endl;
    for (int i = 0; i < bestiaryCount; i++) {
        f << bestiary[i].name << endl;
        f << bestiary[i].timesDefeated << " "
          << bestiary[i].maxHp << " "
          << bestiary[i].attack << " "
          << bestiary[i].defense << endl;
    }
    f.close();
}

void loadBestiary(const string& filename) {
    ifstream f(filename);
    if (!f.is_open()) { initBestiary(); return; }
    f >> bestiaryCount;
    f.ignore();
    for (int i = 0; i < bestiaryCount; i++) {
        getline(f, bestiary[i].name);
        f >> bestiary[i].timesDefeated
          >> bestiary[i].maxHp
          >> bestiary[i].attack
          >> bestiary[i].defense;
        f.ignore();
        bestiary[i].encountered = true;
    }
    f.close();
}

void showBestiary() {
    clearScreen();
    cout << endl;

    int w = 56;
    drawDoubleBox(w, " BESTIARY ", Color::BRIGHT_RED);

    drawBoxLine(w, Color::BRIGHT_WHITE + " Monster ditemukan: " + Color::BRIGHT_YELLOW + to_string(bestiaryCount)
        + Color::DIM + " | Total kills: " + Color::BRIGHT_RED + to_string(getTotalKills()) + Color::RESET, Color::BRIGHT_RED);

    drawSeparator(w, Color::BRIGHT_RED);

    if (bestiaryCount == 0) {
        drawBoxLine(w, Color::DIM + " Belum ada monster yang ditemui." + Color::RESET, Color::BRIGHT_RED);
    } else {
        // Header
        drawBoxLine(w, Color::BOLD + Color::BRIGHT_WHITE + " Nama              HP    ATK  DEF  Kills"
            + Color::RESET, Color::BRIGHT_RED);
        drawSeparator(w, Color::BRIGHT_RED);

        for (int i = 0; i < bestiaryCount; i++) {
            BestiaryEntry &b = bestiary[i];
            string line = Color::WHITE + " " + padRight(b.name, 18)
                + padRight(to_string(b.maxHp), 6)
                + padRight(to_string(b.attack), 5)
                + padRight(to_string(b.defense), 5)
                + Color::BRIGHT_YELLOW + to_string(b.timesDefeated) + Color::RESET;
            drawBoxLine(w, line, Color::BRIGHT_RED);
        }
    }

    drawDoubleBoxEnd(w, Color::BRIGHT_RED);
    pausePrompt();
}
