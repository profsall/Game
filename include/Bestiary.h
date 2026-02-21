#ifndef BESTIARY_H
#define BESTIARY_H

#include <string>

struct BestiaryEntry {
    std::string name;
    int timesDefeated;
    int maxHp;
    int attack;
    int defense;
    bool encountered;
};

const int MAX_BESTIARY = 30;

extern BestiaryEntry bestiary[MAX_BESTIARY];
extern int bestiaryCount;

// Functions
void initBestiary();
void logEncounter(const std::string& name, int maxHp, int atk, int def);
void logKill(const std::string& name);
int getTotalKills();
int getUniqueEncounters();

// Persistence
void saveBestiary(const std::string& filename);
void loadBestiary(const std::string& filename);

// UI
void showBestiary();

#endif
