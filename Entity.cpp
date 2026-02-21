#include "Entity.h"
#include <iostream>
using namespace std;

Player createNewPlayer(string name) {
  Player p;
  p.name = name;
  p.level = 1;
  p.exp = 0;
  p.expToNext = 50;
  p.hp = 100;
  p.maxHp = 100;
  p.attack = 10;
  p.defense = 5;
  p.coin = 0;
  p.swordLevel = 1;
  p.towerCleared = 0;
  p.potionCount = 1;
  return p;
}

int getSwordBonus(int swordLevel) {
  switch (swordLevel) {
  case 1:
    return 0; // Pedang Kayu
  case 2:
    return 5; // Pedang Besi
  case 3:
    return 12; // Pedang Baja
  case 4:
    return 22; // Pedang Mithril
  case 5:
    return 35; // Pedang Legenda
  default:
    return 0;
  }
}

string getSwordName(int swordLevel) {
  switch (swordLevel) {
  case 1:
    return "Pedang Kayu";
  case 2:
    return "Pedang Besi";
  case 3:
    return "Pedang Baja";
  case 4:
    return "Pedang Mithril";
  case 5:
    return "Pedang Legenda";
  default:
    return "Unknown";
  }
}

int getTotalAttack(Player &p) { return p.attack + getSwordBonus(p.swordLevel); }

void levelUp(Player &p) {
  while (p.exp >= p.expToNext) {
    p.exp -= p.expToNext;
    p.level++;
    p.maxHp += 15;
    p.hp = p.maxHp; // full heal on level up
    p.attack += 3;
    p.defense += 2;
    p.expToNext = p.level * 50;

    cout << endl;
    cout << "  ========================================" << endl;
    cout << "  ||     ** LEVEL UP! **                ||" << endl;
    cout << "  ||     Level " << p.level << " tercapai!             ||" << endl;
    cout << "  ||     HP Max  +" << 15 << " -> " << p.maxHp << "            ||"
         << endl;
    cout << "  ||     Attack  +" << 3 << "  -> " << p.attack
         << "             ||" << endl;
    cout << "  ||     Defense +" << 2 << "  -> " << p.defense
         << "             ||" << endl;
    cout << "  ========================================" << endl;
    cout << endl;
  }
}

Enemy createEnemy(string name, int hp, int atk, int def, int expR, int coinR) {
  Enemy e;
  e.name = name;
  e.hp = hp;
  e.maxHp = hp;
  e.attack = atk;
  e.defense = def;
  e.expReward = expR;
  e.coinReward = coinR;
  return e;
}

void displayPlayerStats(Player &p) {
  cout << endl;
  cout << "  +------------------------------------+" << endl;
  cout << "  |         STATUS KESATRIA             |" << endl;
  cout << "  +------------------------------------+" << endl;
  cout << "  | Nama    : " << p.name << endl;
  cout << "  | Level   : " << p.level << endl;
  cout << "  | EXP     : " << p.exp << " / " << p.expToNext << endl;
  cout << "  | HP      : " << p.hp << " / " << p.maxHp << endl;
  cout << "  | Attack  : " << p.attack << " (+" << getSwordBonus(p.swordLevel)
       << " sword)" << endl;
  cout << "  | Defense : " << p.defense << endl;
  cout << "  | Coin    : " << p.coin << " G" << endl;
  cout << "  | Pedang  : " << getSwordName(p.swordLevel) << " (Lv."
       << p.swordLevel << ")" << endl;
  cout << "  | Potion  : " << p.potionCount << " / 5" << endl;
  cout << "  | Tower   : " << p.towerCleared << " / 4 cleared" << endl;
  cout << "  +------------------------------------+" << endl;
  cout << endl;
}
