#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include "Entity.h"

void savegame(Player &p);
bool loadgame(Player &p);
void savegame(Player &p, int slot);
bool loadgame(Player &p, int slot);
int showSaveSlotMenu(const std::string& action);
void opening(int position);

#endif
