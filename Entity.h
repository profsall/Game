#ifndef ENTITY_H
#define ENTITY_H
#include <string>
using namespace std;

struct Player {
    string name;
    int level;
    int exp;
    int expToNext;
    int hp;
    int maxHp;
    int attack;
    int defense;
    int coin;
    int swordLevel;      // 1=Kayu, 2=Besi, 3=Baja, 4=Mithril, 5=Legenda
    int towerCleared;    // 0-4
    int potionCount;     // max 5
};

struct Enemy {
    string name;
    int hp;
    int maxHp;
    int attack;
    int defense;
    int expReward;
    int coinReward;
};

// Player functions
Player createNewPlayer(string name);
void levelUp(Player &p);
void displayPlayerStats(Player &p);
int getSwordBonus(int swordLevel);
string getSwordName(int swordLevel);
int getTotalAttack(Player &p);

// Enemy functions
Enemy createEnemy(string name, int hp, int atk, int def, int expR, int coinR);

#endif