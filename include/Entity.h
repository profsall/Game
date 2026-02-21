#ifndef ENTITY_H
#define ENTITY_H
#include <string>

struct Player {
    std::string name;
    int level;
    int exp;
    int expToNext;
    int hp;
    int maxHp;
    int mp;
    int maxMp;
    int attack;
    int defense;
    int coin;
    int swordLevel;      // 1=Kayu, 2=Besi, 3=Baja, 4=Mithril, 5=Legenda
    int towerCleared;    // 0-4
    int potionCount;     // max 5 (legacy, now use inventory)
    int chapter;         // 0=prologue done, 1-4=chapter progress
    int critRate;        // % chance (base 5)
    int dodgeRate;       // % chance (base 3)
    int killCount;       // total monsters killed
    int questsCompleted; // total quests completed
    // Companion NPC state
    bool companionMet;   // met Lyra in Ch1?
    bool companionSaved; // saved Lyra in Ch3?
    bool companionHelped;// Lyra helped in Ch4?
};

struct Enemy {
    std::string name;
    int hp;
    int maxHp;
    int attack;
    int defense;
    int expReward;
    int coinReward;
};

// Player functions
Player createNewPlayer(std::string name);
void levelUp(Player &p);
void displayPlayerStats(Player &p);
int getSwordBonus(int swordLevel);
std::string getSwordName(int swordLevel);
int getTotalAttack(Player &p);
int getTotalDefense(Player &p);

// Enemy functions
Enemy createEnemy(std::string name, int hp, int atk, int def, int expR, int coinR);

// Combat helpers
bool rollCritical(Player &p);
bool rollDodge(Player &p);

#endif
