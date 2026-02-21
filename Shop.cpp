#include "Shop.h"
#include <iostream>
#include <windows.h>
using namespace std;

int getSwordUpgradeCost(int nextLevel) {
  switch (nextLevel) {
  case 2:
    return 100;
  case 3:
    return 300;
  case 4:
    return 700;
  case 5:
    return 1500;
  default:
    return 0;
  }
}

void showSwordUpgrades(Player &hero) {
  system("cls");
  cout << endl;
  cout << "  ======================================" << endl;
  cout << "  ||      UPGRADE PEDANG              ||" << endl;
  cout << "  ======================================" << endl;
  cout << endl;
  cout << "  Pedang saat ini: " << getSwordName(hero.swordLevel) << " (Lv."
       << hero.swordLevel << ")" << endl;
  cout << "  Total ATK: " << getTotalAttack(hero) << " (base:" << hero.attack
       << " + sword:" << getSwordBonus(hero.swordLevel) << ")" << endl;
  cout << "  Coin: " << hero.coin << " G" << endl;
  cout << endl;

  if (hero.swordLevel >= 5) {
    cout << "  Pedangmu sudah level maksimal!" << endl;
    cout << endl << "  Tekan ENTER untuk kembali...";
    cin.ignore();
    cin.get();
    return;
  }

  cout << "  Daftar upgrade:" << endl;
  cout << "  +------+------------------+--------+--------+" << endl;
  cout << "  | Lv.  | Nama             | ATK    | Harga  |" << endl;
  cout << "  +------+------------------+--------+--------+" << endl;

  for (int i = 1; i <= 5; i++) {
    string mark = "";
    if (i == hero.swordLevel)
      mark = " <-- NOW";
    else if (i < hero.swordLevel)
      mark = " [OWNED]";

    cout << "  |  " << i << "   | " << getSwordName(i);
    // padding
    int nameLen = getSwordName(i).length();
    for (int j = nameLen; j < 16; j++)
      cout << " ";
    cout << " | +" << getSwordBonus(i);
    if (getSwordBonus(i) < 10)
      cout << "    ";
    else
      cout << "   ";
    cout << "| ";
    if (i == 1)
      cout << "  --  ";
    else {
      cout << getSwordUpgradeCost(i);
      int costLen = to_string(getSwordUpgradeCost(i)).length();
      for (int j = costLen; j < 5; j++)
        cout << " ";
      cout << " ";
    }
    cout << "|" << mark << endl;
  }
  cout << "  +------+------------------+--------+--------+" << endl;
  cout << endl;

  int nextLevel = hero.swordLevel + 1;
  int cost = getSwordUpgradeCost(nextLevel);

  cout << "  Upgrade ke " << getSwordName(nextLevel) << "? (Harga: " << cost
       << " G)" << endl;
  cout << "  [1] Ya  [2] Tidak" << endl;
  cout << "  > ";

  int choice;
  cin >> choice;

  if (choice == 1) {
    if (hero.coin >= cost) {
      hero.coin -= cost;
      hero.swordLevel = nextLevel;
      cout << endl;
      cout << "  ************************************" << endl;
      cout << "  *  PEDANG DI-UPGRADE!              *" << endl;
      cout << "  *  " << getSwordName(hero.swordLevel) << " diperoleh!" << endl;
      cout << "  *  ATK +" << getSwordBonus(hero.swordLevel) << " dari pedang"
           << endl;
      cout << "  ************************************" << endl;
      Sleep(1500);
    } else {
      cout << endl
           << "  Coin tidak cukup! Butuh " << cost - hero.coin << " G lagi."
           << endl;
      Sleep(1500);
    }
  }
}

void buyPotion(Player &hero) {
  system("cls");
  cout << endl;
  cout << "  ======================================" << endl;
  cout << "  ||         BELI POTION               ||" << endl;
  cout << "  ======================================" << endl;
  cout << endl;
  cout << "  Potion saat ini: " << hero.potionCount << " / 5" << endl;
  cout << "  Coin: " << hero.coin << " G" << endl;
  cout << endl;
  cout << "  HP Potion - Restorasi 50 HP" << endl;
  cout << "  Harga: 30 G per potion" << endl;
  cout << endl;

  if (hero.potionCount >= 5) {
    cout << "  Inventory potion sudah penuh!" << endl;
    cout << endl << "  Tekan ENTER untuk kembali...";
    cin.ignore();
    cin.get();
    return;
  }

  int maxBuy = 5 - hero.potionCount;
  int maxAfford = hero.coin / 30;
  int canBuy = (maxBuy < maxAfford) ? maxBuy : maxAfford;

  cout << "  Bisa beli maksimal: " << canBuy << " potion" << endl;
  cout << "  Berapa yang mau dibeli? (0 untuk batal): ";

  int amount;
  cin >> amount;

  if (amount > 0 && amount <= canBuy) {
    int totalCost = amount * 30;
    hero.coin -= totalCost;
    hero.potionCount += amount;
    cout << endl
         << "  Membeli " << amount << " potion! (-" << totalCost << " G)"
         << endl;
    cout << "  Potion: " << hero.potionCount << " / 5" << endl;
    Sleep(1000);
  } else if (amount > canBuy) {
    cout << endl << "  Tidak bisa beli sebanyak itu!" << endl;
    Sleep(1000);
  }
}

void openShop(Player &hero) {
  while (true) {
    system("cls");
    cout << endl;
    cout << "  ======================================" << endl;
    cout << "  ||      TOKO KESATRIA               ||" << endl;
    cout << "  ||  \"Selamat datang, petualang!\"     ||" << endl;
    cout << "  ======================================" << endl;
    cout << endl;
    cout << "  Coin: " << hero.coin << " G" << endl;
    cout << "  Pedang: " << getSwordName(hero.swordLevel)
         << " | Potion: " << hero.potionCount << "/5" << endl;
    cout << endl;
    cout << "  [1] Upgrade Pedang" << endl;
    cout << "  [2] Beli Potion (30 G)" << endl;
    cout << "  [0] Keluar Toko" << endl;
    cout << "  > ";

    int choice;
    cin >> choice;

    switch (choice) {
    case 1:
      showSwordUpgrades(hero);
      break;
    case 2:
      buyPotion(hero);
      break;
    case 0:
      return;
    }
  }
}
