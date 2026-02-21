#ifndef TOWER_H
#define TOWER_H
#include "Entity.h"

using std::string;

struct TowerInfo {
    string name;
    string theme;
    string color;       // ANSI color for this tower
    int floors;         // number of floors
    int tier;           // difficulty tier (1-4)
    bool cleared;
};

extern TowerInfo towers[4];

// Tower functions
void enterTower(Player &hero, int towerNumber);
bool battle(Player &hero, Enemy &enemy);
Enemy generateMonster(int towerTier, int floor);
Enemy generateBoss(int towerTier);
void displayBattleScene(Player &hero, Enemy &enemy);
void towerSelectMenu(Player &hero);

#endif
