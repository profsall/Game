#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include "Entity.h"
#include <fstream>
#include <string>


void savegame(Player &p);
bool loadgame(Player &p);
void opening(int position);

#endif
