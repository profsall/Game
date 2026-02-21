#ifndef SHOP_H
#define SHOP_H
#include "Entity.h"

void openShop(Player &hero);
void showSwordUpgrades(Player &hero);
void buyPotion(Player &hero);
int getSwordUpgradeCost(int nextLevel);

#endif
