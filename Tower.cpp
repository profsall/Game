#include "Tower.h"
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <windows.h>

using namespace std;

TowerInfo getTowerInfo(int towerNumber) {
  TowerInfo info;
  switch (towerNumber) {
  case 1:
    info.name = "Hutan Gelap";
    info.theme = "Hutan yang dipenuhi kegelapan dan makhluk liar...";
    info.requiredTower = 0;
    info.minLevel = 1;
    break;
  case 2:
    info.name = "Gua Kristal";
    info.theme = "Gua dalam yang berkilau penuh kristal misterius...";
    info.requiredTower = 1;
    info.minLevel = 5;
    break;
  case 3:
    info.name = "Menara Api";
    info.theme = "Menara yang terbakar api abadi dari dalam bumi...";
    info.requiredTower = 2;
    info.minLevel = 10;
    break;
  case 4:
    info.name = "Kastil Kegelapan";
    info.theme = "Kastil terakhir tempat Dark Lord bersemayam...";
    info.requiredTower = 3;
    info.minLevel = 15;
    break;
  default:
    info.name = "Unknown";
    info.theme = "";
    info.requiredTower = -1;
    info.minLevel = 1;
  }
  return info;
}

Enemy generateMonster(int towerNumber, int floor) {
  srand((unsigned)time(0) + floor * 100 + rand());
  int variation = rand() % 3;

  switch (towerNumber) {
  case 1: {
    string names[] = {"Slime", "Goblin", "Wolf"};
    int baseHp[] = {30, 40, 50};
    int baseAtk[] = {6, 8, 10};
    int baseDef[] = {2, 3, 4};
    int baseExp[] = {15, 20, 25};
    int baseCoin[] = {8, 12, 15};
    int scale = floor;
    return createEnemy(
        names[variation], baseHp[variation] + scale * 5,
        baseAtk[variation] + scale * 1, baseDef[variation] + scale,
        baseExp[variation] + scale * 3, baseCoin[variation] + scale * 2);
  }
  case 2: {
    string names[] = {"Bat", "Golem", "Spider"};
    int baseHp[] = {50, 70, 55};
    int baseAtk[] = {12, 15, 13};
    int baseDef[] = {5, 8, 6};
    int baseExp[] = {30, 40, 35};
    int baseCoin[] = {18, 25, 20};
    int scale = floor;
    return createEnemy(
        names[variation], baseHp[variation] + scale * 8,
        baseAtk[variation] + scale * 2, baseDef[variation] + scale,
        baseExp[variation] + scale * 5, baseCoin[variation] + scale * 3);
  }
  case 3: {
    string names[] = {"Fire Imp", "Lava Snake", "Demon"};
    int baseHp[] = {80, 90, 100};
    int baseAtk[] = {20, 22, 25};
    int baseDef[] = {10, 11, 13};
    int baseExp[] = {50, 55, 65};
    int baseCoin[] = {30, 35, 40};
    int scale = floor;
    return createEnemy(
        names[variation], baseHp[variation] + scale * 10,
        baseAtk[variation] + scale * 3, baseDef[variation] + scale * 2,
        baseExp[variation] + scale * 7, baseCoin[variation] + scale * 5);
  }
  case 4: {
    string names[] = {"Shadow", "Dark Knight", "Wraith"};
    int baseHp[] = {120, 150, 130};
    int baseAtk[] = {30, 35, 32};
    int baseDef[] = {15, 18, 16};
    int baseExp[] = {80, 100, 90};
    int baseCoin[] = {50, 65, 55};
    int scale = floor;
    return createEnemy(
        names[variation], baseHp[variation] + scale * 12,
        baseAtk[variation] + scale * 4, baseDef[variation] + scale * 2,
        baseExp[variation] + scale * 10, baseCoin[variation] + scale * 7);
  }
  default:
    return createEnemy("Unknown", 10, 5, 1, 5, 5);
  }
}

Enemy generateBoss(int towerNumber) {
  switch (towerNumber) {
  case 1:
    return createEnemy(">> GOBLIN KING <<", 120, 18, 8, 100, 80);
  case 2:
    return createEnemy(">> CRYSTAL GOLEM <<", 200, 28, 15, 180, 150);
  case 3:
    return createEnemy(">> FIRE DRAGON <<", 320, 40, 22, 300, 250);
  case 4:
    return createEnemy(">> DARK LORD <<", 500, 55, 30, 500, 500);
  default:
    return createEnemy("Unknown Boss", 100, 20, 10, 50, 50);
  }
}

void displayHealthBar(string label, int current, int max, int barLen) {
  int filled = (current * barLen) / max;
  if (filled < 0)
    filled = 0;
  if (filled > barLen)
    filled = barLen;

  cout << "  " << label << " [";
  for (int i = 0; i < barLen; i++) {
    if (i < filled)
      cout << "#";
    else
      cout << "-";
  }
  cout << "] " << current << "/" << max << endl;
}

void displayBattleScene(Player &hero, Enemy &enemy) {
  system("cls");
  cout << endl;
  cout << "  ============== BATTLE! ==============" << endl;
  cout << endl;

  // Enemy side
  cout << "    " << enemy.name << endl;
  displayHealthBar("Enemy HP", enemy.hp, enemy.maxHp);
  cout << "    ATK:" << enemy.attack << " DEF:" << enemy.defense << endl;

  cout << endl;
  cout << "          --- VS ---" << endl;
  cout << endl;

  // Hero side
  cout << "    " << hero.name << " (Lv." << hero.level << ")" << endl;
  displayHealthBar("Hero  HP", hero.hp, hero.maxHp);
  cout << "    ATK:" << getTotalAttack(hero) << " DEF:" << hero.defense
       << " | Potion:" << hero.potionCount << endl;

  cout << endl;
  cout << "  ======================================" << endl;
}

bool battle(Player &hero, Enemy &enemy) {
  while (hero.hp > 0 && enemy.hp > 0) {
    displayBattleScene(hero, enemy);

    cout << endl;
    cout << "  Pilih aksi:" << endl;
    cout << "  [1] Attack" << endl;
    cout << "  [2] Defend (kurangi damage 50%)" << endl;
    cout << "  [3] Use Potion (+" << 50 << " HP)" << endl;
    cout << "  > ";

    int choice;
    cin >> choice;

    bool defending = false;
    int damageDealt = 0;

    switch (choice) {
    case 1: {
      // Hero attacks
      damageDealt = getTotalAttack(hero) - enemy.defense;
      if (damageDealt < 1)
        damageDealt = 1;
      // random variance +-20%
      int variance = rand() % (damageDealt / 3 + 1);
      if (rand() % 2 == 0)
        damageDealt += variance;
      else
        damageDealt -= variance / 2;
      if (damageDealt < 1)
        damageDealt = 1;
      enemy.hp -= damageDealt;
      cout << endl
           << "  >> " << hero.name << " menyerang! Damage: " << damageDealt
           << endl;
      break;
    }
    case 2:
      defending = true;
      cout << endl << "  >> " << hero.name << " bertahan!" << endl;
      break;
    case 3:
      if (hero.potionCount > 0) {
        hero.potionCount--;
        int healAmount = 50;
        hero.hp += healAmount;
        if (hero.hp > hero.maxHp)
          hero.hp = hero.maxHp;
        cout << endl << "  >> Menggunakan Potion! HP +" << healAmount << endl;
      } else {
        cout << endl << "  >> Tidak ada Potion tersisa!" << endl;
      }
      break;
    default:
      cout << endl << "  >> Aksi tidak valid! Giliran terbuang!" << endl;
      break;
    }

    // Enemy turn (if still alive)
    if (enemy.hp > 0) {
      int enemyDamage = enemy.attack - hero.defense;
      if (defending)
        enemyDamage /= 2;
      if (enemyDamage < 1)
        enemyDamage = 1;
      // enemy variance
      int eVar = rand() % (enemyDamage / 4 + 1);
      if (rand() % 2 == 0)
        enemyDamage += eVar;
      else
        enemyDamage -= eVar / 2;
      if (enemyDamage < 1)
        enemyDamage = 1;

      hero.hp -= enemyDamage;
      cout << "  >> " << enemy.name << " menyerang! Damage: " << enemyDamage
           << endl;
    }

    Sleep(800);
  }

  displayBattleScene(hero, enemy);

  if (hero.hp <= 0) {
    hero.hp = 0;
    cout << endl;
    cout << "  ************************************" << endl;
    cout << "  *        KAMU KALAH!               *" << endl;
    cout << "  *  Kamu pingsan dan kembali ke     *" << endl;
    cout << "  *  desa dengan sisa nyawa...       *" << endl;
    cout << "  ************************************" << endl;
    hero.hp = hero.maxHp / 4; // revive with 25% HP
    cout << endl << "  Tekan ENTER untuk melanjutkan...";
    cin.ignore();
    cin.get();
    return false;
  } else {
    cout << endl;
    cout << "  ************************************" << endl;
    cout << "  *         KAMU MENANG!             *" << endl;
    cout << "  *  EXP  +" << enemy.expReward << endl;
    cout << "  *  Coin +" << enemy.coinReward << " G" << endl;
    cout << "  ************************************" << endl;

    hero.exp += enemy.expReward;
    hero.coin += enemy.coinReward;
    levelUp(hero);

    Sleep(1000);
    return true;
  }
}

void enterTower(Player &hero, int towerNumber) {
  TowerInfo info = getTowerInfo(towerNumber);

  system("cls");
  cout << endl;
  cout << "  ===========================================" << endl;
  cout << "  ||  TOWER " << towerNumber << ": " << info.name << endl;
  cout << "  ||  " << info.theme << endl;
  cout << "  ===========================================" << endl;
  cout << endl;
  cout << "  Bersiaplah, kesatria!" << endl;
  cout << endl;
  cout << "  Tekan ENTER untuk memasuki tower...";
  cin.ignore();
  cin.get();

  // 4 floors of monsters
  for (int floor = 1; floor <= 4; floor++) {
    Enemy monster = generateMonster(towerNumber, floor);

    system("cls");
    cout << endl;
    cout << "  --- Tower " << towerNumber << " | Lantai " << floor << " / 5 ---"
         << endl;
    cout << "  Seekor " << monster.name << " muncul!" << endl;
    Sleep(1000);

    bool won = battle(hero, monster);
    if (!won) {
      cout << endl << "  Kamu gagal menaklukkan tower ini..." << endl;
      cout << "  Tekan ENTER untuk kembali...";
      cin.get();
      return;
    }
  }

  // Floor 5: Boss fight
  Enemy boss = generateBoss(towerNumber);

  system("cls");
  cout << endl;
  cout << "  ===========================================" << endl;
  cout << "  ||       BOSS FLOOR!                    ||" << endl;
  cout << "  ||  " << boss.name << " muncul!" << endl;
  cout << "  ===========================================" << endl;
  cout << endl;
  cout << "  Tekan ENTER untuk melawan boss...";
  cin.get();
  Sleep(500);

  bool bossWon = battle(hero, boss);
  if (bossWon) {
    hero.towerCleared = towerNumber; // mark as cleared

    system("cls");
    cout << endl;
    cout << "  =============================================" << endl;
    cout << "  ||                                         ||" << endl;
    cout << "  ||    TOWER " << towerNumber << " DITAKLUKKAN!               ||"
         << endl;
    cout << "  ||    " << info.name << " telah dibersihkan!   ||" << endl;
    cout << "  ||                                         ||" << endl;
    cout << "  =============================================" << endl;

    if (towerNumber == 4) {
      cout << endl;
      cout << "  =============================================" << endl;
      cout << "  ||                                         ||" << endl;
      cout << "  ||   SELAMAT! SEMUA TOWER DITAKLUKKAN!     ||" << endl;
      cout << "  ||   Kamu adalah kesatria sejati!           ||" << endl;
      cout << "  ||   Kerajaan telah diselamatkan!           ||" << endl;
      cout << "  ||                                         ||" << endl;
      cout << "  =============================================" << endl;
    }

    cout << endl << "  Tekan ENTER untuk kembali...";
    cin.get();
  } else {
    cout << endl << "  Boss terlalu kuat! Latih dirimu lagi..." << endl;
    cout << "  Tekan ENTER untuk kembali...";
    cin.get();
  }
}

void towerSelectMenu(Player &hero) {
  while (true) {
    system("cls");
    cout << endl;
    cout << "  ======================================" << endl;
    cout << "  ||       PILIH TOWER               ||" << endl;
    cout << "  ======================================" << endl;
    cout << endl;

    for (int i = 1; i <= 4; i++) {
      TowerInfo info = getTowerInfo(i);
      string status;
      if (hero.towerCleared >= i) {
        status = "[CLEARED]";
      } else if (hero.towerCleared >= info.requiredTower) {
        status = "[UNLOCKED]";
      } else {
        status = "[LOCKED]";
      }

      cout << "  [" << i << "] Tower " << i << ": " << info.name;
      cout << " (Lv." << info.minLevel << "+) " << status << endl;
    }

    cout << endl;
    cout << "  [0] Kembali" << endl;
    cout << "  > ";

    int choice;
    cin >> choice;

    if (choice == 0)
      return;

    if (choice >= 1 && choice <= 4) {
      TowerInfo info = getTowerInfo(choice);

      if (hero.towerCleared >= choice) {
        cout << endl << "  Tower ini sudah ditaklukkan!" << endl;
        Sleep(1000);
        continue;
      }

      if (hero.towerCleared < info.requiredTower) {
        cout << endl
             << "  Tower ini masih terkunci! Taklukkan tower sebelumnya dulu."
             << endl;
        Sleep(1000);
        continue;
      }

      if (hero.level < info.minLevel) {
        cout << endl
             << "  Level kamu masih kurang! (Butuh Lv." << info.minLevel << ")"
             << endl;
        cout << "  Tetap masuk? [1] Ya [2] Tidak: ";
        int confirm;
        cin >> confirm;
        if (confirm != 1)
          continue;
      }

      enterTower(hero, choice);
    }
  }
}
