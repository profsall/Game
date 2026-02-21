#include "Entity.h"
#include "Filesystem.h"
#include "Shop.h"
#include "Tower.h"
#include <conio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

void showIntroNarration() {
  ifstream file("narasi.txt");
  if (file.is_open()) {
    string line;
    bool inIntro = false;
    while (getline(file, line)) {
      if (line.find("[INTRO]") != string::npos) {
        inIntro = true;
        continue;
      }
      if (line.find("[END]") != string::npos)
        break;
      if (inIntro) {
        cout << line << endl;
        Sleep(120);
      }
    }
    file.close();
  }
  cout << endl;
  cout << "  Tekan ENTER untuk melanjutkan...";
  cin.get();
}

void restAtInn(Player &hero) {
  system("cls");
  cout << endl;
  cout << "  ======================================" << endl;
  cout << "  ||      PENGINAPAN DESA             ||" << endl;
  cout << "  ======================================" << endl;
  cout << endl;
  cout << "  HP saat ini: " << hero.hp << " / " << hero.maxHp << endl;
  cout << endl;
  cout << "  Istirahat di penginapan? (Gratis, HP full)" << endl;
  cout << "  [1] Ya  [2] Tidak" << endl;
  cout << "  > ";

  int choice;
  cin >> choice;

  if (choice == 1) {
    hero.hp = hero.maxHp;
    cout << endl;
    cout << "  Kamu beristirahat dengan nyaman..." << endl;
    cout << "  HP dipulihkan ke " << hero.maxHp << "!" << endl;
    Sleep(1500);
  }
}

void wildBattle(Player &hero) {
  system("cls");
  cout << endl;
  cout << "  ======================================" << endl;
  cout << "  ||     BERBURU MONSTER LIAR         ||" << endl;
  cout << "  ======================================" << endl;
  cout << endl;

  // Generate a random monster based on player level
  int tier;
  if (hero.level < 5)
    tier = 1;
  else if (hero.level < 10)
    tier = 2;
  else if (hero.level < 15)
    tier = 3;
  else
    tier = 4;

  int floor = (rand() % 3) + 1;
  Enemy monster = generateMonster(tier, floor);

  cout << "  Seekor " << monster.name << " liar muncul!" << endl;
  Sleep(800);

  battle(hero, monster);
}

void gameHub(Player &hero) {
  while (true) {
    system("cls");
    cout << endl;
    cout << "  ==============================================" << endl;
    cout << "  ||            DESA ARCANDIA                 ||" << endl;
    cout << "  ==============================================" << endl;
    cout << endl;
    cout << "  Kesatria: " << hero.name << " | Lv." << hero.level;
    cout << " | HP:" << hero.hp << "/" << hero.maxHp;
    cout << " | Coin:" << hero.coin << "G" << endl;
    cout << "  Pedang: " << getSwordName(hero.swordLevel);
    cout << " | Tower: " << hero.towerCleared << "/4" << endl;
    cout << endl;
    cout << "  +----------------------------------------+" << endl;
    cout << "  |  Apa yang ingin kamu lakukan?          |" << endl;
    cout << "  +----------------------------------------+" << endl;
    cout << "  |  [1] Masuk Tower                       |" << endl;
    cout << "  |  [2] Berburu Monster Liar              |" << endl;
    cout << "  |  [3] Toko Kesatria (Shop)              |" << endl;
    cout << "  |  [4] Penginapan (Istirahat)            |" << endl;
    cout << "  |  [5] Lihat Status                      |" << endl;
    cout << "  |  [6] Save Game                         |" << endl;
    cout << "  |  [0] Kembali ke Menu Utama             |" << endl;
    cout << "  +----------------------------------------+" << endl;
    cout << "  > ";

    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
      towerSelectMenu(hero);
      break;
    case 2:
      wildBattle(hero);
      break;
    case 3:
      openShop(hero);
      break;
    case 4:
      restAtInn(hero);
      break;
    case 5:
      displayPlayerStats(hero);
      cout << "  Tekan ENTER untuk kembali...";
      cin.ignore();
      cin.get();
      break;
    case 6:
      savegame(hero);
      break;
    case 0:
      cout << endl << "  Simpan game sebelum keluar? [1] Ya [2] Tidak: ";
      int saveChoice;
      cin >> saveChoice;
      if (saveChoice == 1)
        savegame(hero);
      return;
    default:
      cout << "  Pilihan tidak valid!" << endl;
      Sleep(500);
    }
  }
}

int main() {
  srand((unsigned)time(0));
  int positions = 0;
  int input;

  while (true) {
    // Main menu loop
    while (true) {
      opening(positions);
      input = getch();

      if (input == 0 || input == 224) {
        input = getch();
        if (input == 72) { // Up arrow
          positions--;
          if (positions < 0)
            positions = 3;
        } else if (input == 80) { // Down arrow
          positions++;
          if (positions > 3)
            positions = 0;
        }
      } else if (input == 13) { // Enter key
        break;
      }
    }

    system("cls");

    if (positions == 0) {
      // Continue Game
      Player hero;
      if (loadgame(hero)) {
        cout << endl;
        cout << "  >> Save data ditemukan!" << endl;
        cout << "  >> Selamat datang kembali, " << hero.name << "!" << endl;
        Sleep(1500);
        gameHub(hero);
      } else {
        cout << endl;
        cout << "  >> Tidak ada save data ditemukan." << endl;
        cout << "  >> Silakan pilih New Game." << endl;
        Sleep(2000);
      }
    } else if (positions == 1) {
      // New Game
      Player hero;
      string playerName;

      system("cls");
      cout << endl;
      cout << "  ======================================" << endl;
      cout << "  ||         NEW GAME                 ||" << endl;
      cout << "  ======================================" << endl;
      cout << endl;

      showIntroNarration();

      system("cls");
      cout << endl;
      cout << "  ======================================" << endl;
      cout << "  ||    Masukkan Nama Kesatria:        ||" << endl;
      cout << "  ======================================" << endl;
      cout << endl;
      cout << "  Nama: ";
      cin >> playerName;

      hero = createNewPlayer(playerName);

      cout << endl;
      cout << "  Selamat datang, " << hero.name << "!" << endl;
      cout << "  Petualanganmu dimulai sekarang..." << endl;
      Sleep(1500);

      displayPlayerStats(hero);
      cout << "  Tekan ENTER untuk mulai berpetualang...";
      cin.ignore();
      cin.get();

      savegame(hero);
      gameHub(hero);

    } else if (positions == 2) {
      // Settings
      system("cls");
      cout << endl;
      cout << "  ======================================" << endl;
      cout << "  ||          SETTINGS                ||" << endl;
      cout << "  ======================================" << endl;
      cout << endl;
      cout << "  Game: OWI DUNGEON - Tower Conquest" << endl;
      cout << "  Version: 1.0" << endl;
      cout << endl;
      cout << "  Kontrol:" << endl;
      cout << "  - Arrow Up/Down : Navigasi menu" << endl;
      cout << "  - Enter         : Pilih" << endl;
      cout << "  - Angka 1-9     : Pilih opsi dalam game" << endl;
      cout << endl;
      cout << "  Tekan ENTER untuk kembali...";
      cin.ignore();
      cin.get();

    } else if (positions == 3) {
      // Quit
      cout << endl;
      cout << "  Terima kasih sudah bermain!" << endl;
      cout << "  OWI DUNGEON: Tower Conquest" << endl;
      Sleep(1500);
      return 0;
    }
  }

  return 0;
}