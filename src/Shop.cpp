#include "Shop.h"
#include "Config.h"
#include "Entity.h"
#include "Inventory.h"
#include "TUI.h"
#include <iostream>
using namespace std;

// ═══════════════════════════════════════════
//  Sword Upgrade Costs
// ═══════════════════════════════════════════

int getUpgradeCost(int currentLevel) {
    switch (currentLevel) {
    case 1: return 100;
    case 2: return 300;
    case 3: return 700;
    case 4: return 1500;
    default: return 0;
    }
}

// ═══════════════════════════════════════════
//  Sword Upgrades
// ═══════════════════════════════════════════

void showSwordUpgrades(Player &hero) {
    clearScreen();
    cout << endl;

    int w = 56;
    drawDoubleBox(w, Box::SWORD + " UPGRADE PEDANG " + Box::SWORD, Color::BRIGHT_RED);

    drawBoxLine(w, Color::BRIGHT_WHITE + " Pedang saat ini: " + Color::BRIGHT_CYAN
        + getSwordName(hero.swordLevel) + Color::DIM + " (+)" + to_string(getSwordBonus(hero.swordLevel))
        + " ATK)" + Color::RESET, Color::BRIGHT_RED);

    drawSeparator(w, Color::BRIGHT_RED);

    // Sword table
    drawBoxLine(w, Color::BOLD + Color::BRIGHT_WHITE + " Lv   Nama               ATK    Harga" + Color::RESET, Color::BRIGHT_RED);
    drawSeparator(w, Color::BRIGHT_RED);

    for (int i = 1; i <= 5; i++) {
        string color;
        string statusStr;
        if (i < hero.swordLevel) {
            color = Color::DIM;
            statusStr = "";
        } else if (i == hero.swordLevel) {
            color = Color::BRIGHT_GREEN;
            statusStr = Color::BRIGHT_GREEN + " [EQUIPPED]";
        } else if (i == hero.swordLevel + 1) {
            int cost = getUpgradeCost(hero.swordLevel);
            bool canAfford = hero.coin >= cost;
            color = canAfford ? Color::BRIGHT_YELLOW : Color::BRIGHT_RED;
            statusStr = color + " " + to_string(cost) + "G";
        } else {
            color = Color::BRIGHT_BLACK;
            statusStr = Color::DIM + " ???";
        }

        drawBoxLine(w, color + " [" + to_string(i) + "]  "
            + padRight(getSwordName(i), 18)
            + padRight("+" + to_string(getSwordBonus(i)), 7)
            + statusStr + Color::RESET, Color::BRIGHT_RED);
    }

    drawSeparator(w, Color::BRIGHT_RED);
    drawBoxLine(w, Color::BRIGHT_YELLOW + " Gold: " + to_string(hero.coin) + "G" + Color::RESET, Color::BRIGHT_RED);
    drawDoubleBoxEnd(w, Color::BRIGHT_RED);

    cout << endl;
    if (hero.swordLevel >= 5) {
        cout << "  " << Color::BRIGHT_GREEN << "Pedangmu sudah level maksimal!" << Color::RESET << endl;
        pausePrompt();
        return;
    }

    int cost = getUpgradeCost(hero.swordLevel);
    cout << "  " << Color::WHITE << "Upgrade ke " << getSwordName(hero.swordLevel + 1)
         << "? (" << cost << "G)" << Color::RESET << endl;
    cout << "  " << Color::BRIGHT_WHITE << "[1] Ya  [2] Tidak" << Color::RESET << endl;

    int choice = getChoice(1, 2);
    if (choice == 1 && hero.coin >= cost) {
        hero.coin -= cost;
        hero.swordLevel++;
        sfxItem();
        cout << "  " << Color::BRIGHT_GREEN << Box::SPARKLE << " Upgrade berhasil! "
             << getSwordName(hero.swordLevel) << "!" << Color::RESET << endl;
    } else if (choice == 1) {
        cout << "  " << Color::BRIGHT_RED << "Gold tidak cukup!" << Color::RESET << endl;
    }
    sleepMs(800);
}

// ═══════════════════════════════════════════
//  Item Shop
// ═══════════════════════════════════════════

void showItemShop(Player &hero) {
    while (true) {
        clearScreen();
        cout << endl;

        int w = 56;
        drawDoubleBox(w, Box::DIAMOND + " TOKO ITEM " + Box::DIAMOND, Color::BRIGHT_GREEN);

        // Consumables
        int shopItems[] = {0, 1, 2, 3, 12, 13, 14, 15, 16, 17, 18, 19};
        int shopCount = 12;

        for (int i = 0; i < shopCount; i++) {
            Item it = getItemTemplate(shopItems[i]);
            bool canAfford = hero.coin >= it.price;
            string color = canAfford ? Color::BRIGHT_WHITE : Color::DIM;

            drawBoxLine(w, color + " [" + to_string(i + 1) + "]  "
                + padRight(it.name, 18)
                + padRight(to_string(it.price) + "G", 8)
                + Color::DIM + it.description + Color::RESET, Color::BRIGHT_GREEN);
        }

        drawSeparator(w, Color::BRIGHT_GREEN);
        drawBoxLine(w, Color::BRIGHT_YELLOW + " Gold: " + to_string(hero.coin) + "G" + Color::RESET, Color::BRIGHT_GREEN);
        drawBoxLine(w, Color::DIM + " [0] Kembali" + Color::RESET, Color::BRIGHT_GREEN);
        drawDoubleBoxEnd(w, Color::BRIGHT_GREEN);

        int choice = getChoice(0, shopCount);
        if (choice == 0) return;

        int itemIdx = choice - 1;
        int itemId = shopItems[itemIdx];
        Item it = getItemTemplate(itemId);

        if (hero.coin >= it.price) {
            hero.coin -= it.price;
            if (it.id == 18) {
                // Golden Apple - permanent HP boost
                hero.maxHp += 20;
                hero.hp += 20;
                sfxItem();
                cout << "  " << Color::BRIGHT_GREEN << Box::SPARKLE << " Max HP +20!"
                     << Color::RESET << endl;
            } else if (it.id == 19) {
                // Mana Crystal - permanent MP boost
                hero.maxMp += 10;
                hero.mp += 10;
                sfxItem();
                cout << "  " << Color::BRIGHT_BLUE << Box::SPARKLE << " Max MP +10!"
                     << Color::RESET << endl;
            } else {
                addItem(itemId, 1);
                sfxItem();
                cout << "  " << Color::BRIGHT_GREEN << "Membeli " << it.name << "!"
                     << Color::RESET << endl;
            }
        } else {
            cout << "  " << Color::BRIGHT_RED << "Gold tidak cukup!" << Color::RESET << endl;
        }
        sleepMs(600);
    }
}

// ═══════════════════════════════════════════
//  Equipment Shop
// ═══════════════════════════════════════════

void showEquipShop(Player &hero) {
    while (true) {
        clearScreen();
        cout << endl;

        int w = 56;
        drawDoubleBox(w, Box::SHIELD + " TOKO PERLENGKAPAN " + Box::SHIELD, Color::BRIGHT_BLUE);

        // Armor items
        int armorIds[] = {4, 5, 6, 7};    // Leather, Chain, Plate, Dragon
        int accIds[]   = {8, 9, 10, 11};   // Charm, Cloak, Ring, Amulet

        drawBoxLine(w, Color::BOLD + Color::BRIGHT_WHITE + " ARMOR:" + Color::RESET, Color::BRIGHT_BLUE);
        for (int i = 0; i < 4; i++) {
            Item it = getItemTemplate(armorIds[i]);
            bool owned = hasItem(armorIds[i]);
            bool canAfford = hero.coin >= it.price;
            string status = owned ? Color::BRIGHT_GREEN + " [OWNED]" : (canAfford ? "" : Color::BRIGHT_RED + " [MAHAL]");
            string color = owned ? Color::DIM : (canAfford ? Color::BRIGHT_WHITE : Color::DIM);

            drawBoxLine(w, color + " [" + to_string(i + 1) + "] "
                + padRight(it.name, 16) + padRight("DEF+" + to_string(it.value), 8)
                + padRight(to_string(it.price) + "G", 7) + status + Color::RESET, Color::BRIGHT_BLUE);
        }

        drawSeparator(w, Color::BRIGHT_BLUE);
        drawBoxLine(w, Color::BOLD + Color::BRIGHT_WHITE + " ACCESSORY:" + Color::RESET, Color::BRIGHT_BLUE);
        for (int i = 0; i < 4; i++) {
            Item it = getItemTemplate(accIds[i]);
            bool owned = hasItem(accIds[i]);
            bool canAfford = hero.coin >= it.price;
            string status = owned ? Color::BRIGHT_GREEN + " [OWNED]" : (canAfford ? "" : Color::BRIGHT_RED + " [MAHAL]");
            string color = owned ? Color::DIM : (canAfford ? Color::BRIGHT_WHITE : Color::DIM);

            drawBoxLine(w, color + " [" + to_string(i + 5) + "] "
                + padRight(it.name, 16) + padRight(it.description, 16)
                + padRight(to_string(it.price) + "G", 7) + status + Color::RESET, Color::BRIGHT_BLUE);
        }

        drawSeparator(w, Color::BRIGHT_BLUE);
        drawBoxLine(w, Color::BRIGHT_YELLOW + " Gold: " + to_string(hero.coin) + "G" + Color::RESET, Color::BRIGHT_BLUE);
        drawBoxLine(w, Color::DIM + " [0] Kembali" + Color::RESET, Color::BRIGHT_BLUE);
        drawDoubleBoxEnd(w, Color::BRIGHT_BLUE);

        int choice = getChoice(0, 8);
        if (choice == 0) return;

        int buyId;
        if (choice >= 1 && choice <= 4) buyId = armorIds[choice - 1];
        else buyId = accIds[choice - 5];

        if (hasItem(buyId)) {
            cout << "  " << Color::DIM << "Sudah punya item ini." << Color::RESET << endl;
            sleepMs(600);
            continue;
        }

        Item it = getItemTemplate(buyId);
        if (hero.coin >= it.price) {
            hero.coin -= it.price;
            addItem(buyId, 1);
            sfxItem();

            // Auto-equip
            if (it.type == ITEM_ARMOR) {
                equipArmor(buyId);
                cout << "  " << Color::BRIGHT_GREEN << Box::SHIELD << " " << it.name
                     << " dibeli dan di-equip!" << Color::RESET << endl;
            } else {
                equipAccessory(buyId);
                cout << "  " << Color::BRIGHT_MAGENTA << Box::DIAMOND << " " << it.name
                     << " dibeli dan di-equip!" << Color::RESET << endl;
            }
        } else {
            cout << "  " << Color::BRIGHT_RED << "Gold tidak cukup!" << Color::RESET << endl;
        }
        sleepMs(600);
    }
}

// ═══════════════════════════════════════════
//  Main Shop Menu
// ═══════════════════════════════════════════

void openShop(Player &hero) {
    while (true) {
        clearScreen();
        cout << endl;

        int w = 56;
        drawDoubleBox(w, Box::STAR + " TOKO KESATRIA " + Box::STAR, Color::BRIGHT_MAGENTA);

        // NPC greeting
        drawBoxLine(w, Color::BRIGHT_GREEN + " " + Box::DIAMOND + " Penjaga Toko:" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::WHITE + "   \"Selamat datang! Apa yang kamu cari?\"" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawSeparator(w, Color::BRIGHT_MAGENTA);

        drawBoxLine(w, Color::BRIGHT_WHITE + " [1] " + Box::SWORD + " Upgrade Pedang" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [2] " + Box::HEART + " Beli Item / Potion" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [3] " + Box::SHIELD + " Beli Armor & Accessory" + Color::RESET, Color::BRIGHT_MAGENTA);

        drawSeparator(w, Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::BRIGHT_YELLOW + " Gold: " + to_string(hero.coin) + "G" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::DIM + " [0] Keluar toko" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);

        int choice = getChoice(0, 3);

        switch (choice) {
        case 1: showSwordUpgrades(hero); break;
        case 2: showItemShop(hero); break;
        case 3: showEquipShop(hero); break;
        case 0: return;
        }
    }
}
