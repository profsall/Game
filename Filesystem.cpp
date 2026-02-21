#include "Filesystem.h"
#include <iostream>
#include <windows.h>
using namespace std;

void savegame(Player &p) {
  ofstream savefile("savegame.txt");
  if (savefile.is_open()) {
    savefile << p.name << endl;
    savefile << p.level << endl;
    savefile << p.exp << endl;
    savefile << p.expToNext << endl;
    savefile << p.hp << endl;
    savefile << p.maxHp << endl;
    savefile << p.attack << endl;
    savefile << p.defense << endl;
    savefile << p.coin << endl;
    savefile << p.swordLevel << endl;
    savefile << p.towerCleared << endl;
    savefile << p.potionCount << endl;
    savefile.close();
    cout << endl << "  >> Game berhasil disimpan!" << endl;
    Sleep(1000);
  } else {
    cout << "  >> Gagal menyimpan game!" << endl;
    Sleep(1000);
  }
}

bool loadgame(Player &p) {
  ifstream loadfile("savegame.txt");
  if (loadfile.is_open()) {
    string testLine;
    // Check if file is empty
    if (!getline(loadfile, testLine) || testLine.empty()) {
      loadfile.close();
      return false;
    }
    p.name = testLine;
    loadfile >> p.level;
    loadfile >> p.exp;
    loadfile >> p.expToNext;
    loadfile >> p.hp;
    loadfile >> p.maxHp;
    loadfile >> p.attack;
    loadfile >> p.defense;
    loadfile >> p.coin;
    loadfile >> p.swordLevel;
    loadfile >> p.towerCleared;
    loadfile >> p.potionCount;
    loadfile.close();
    return true;
  }
  return false;
}

void opening(int position) {
  system("cls");
  cout << endl;
  cout << "  ============================================" << endl;
  cout << "  ||                                        ||" << endl;
  cout << "  ||     OWI DUNGEON: TOWER CONQUEST        ||" << endl;
  cout << "  ||                                        ||" << endl;
  cout << "  ============================================" << endl;
  cout << endl;
  cout << "           [o]                               " << endl;
  cout << "           /|\\         /\\                    " << endl;
  cout << "           / \\        /  \\                   " << endl;
  cout << "                     /    \\                  " << endl;
  cout << "                    / TOWER\\                 " << endl;
  cout << "                   /________\\                " << endl;
  cout << endl;

  string menu[] = {"Continue Game", "New Game", "Settings", "Quit Game"};
  for (int i = 0; i < 4; i++) {
    if (i == position) {
      cout << "         >>  " << menu[i] << "  <<" << endl;
    } else {
      cout << "             " << menu[i] << endl;
    }
  }
  cout << endl;
  cout << "  Gunakan Arrow Up/Down dan Enter" << endl;
}