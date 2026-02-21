#ifndef TOWER_H
#define TOWER_H
#include "Entity.h"

struct TowerInfo {
  string name;
  string theme;
  int requiredTower; // tower sebelumnya yang harus cleared (-1 = none)
  int minLevel;
};

// Tower functions
void enterTower(Player &hero, int towerNumber);
bool battle(Player &hero, Enemy &enemy);
Enemy generateMonster(int towerNumber, int floor);
Enemy generateBoss(int towerNumber);
void displayBattleScene(Player &hero, Enemy &enemy);
void displayHealthBar(string label, int current, int max, int barLen = 20);
TowerInfo getTowerInfo(int towerNumber);
void towerSelectMenu(Player &hero);

#endif
