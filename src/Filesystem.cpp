#include "Filesystem.h"
#include "Bestiary.h"
#include "Inventory.h"
#include "Quest.h"
#include "Skills.h"
#include "TUI.h"
#include <fstream>
#include <iostream>
#include <windows.h>
using namespace std;

// ===========================================
//  Save Slot System (3 slots)
// ===========================================

static string getSaveFilename(int slot) {
    return "save_" + to_string(slot) + ".txt";
}

static string getBestiaryFilename(int slot) {
    return "bestiary_" + to_string(slot) + ".txt";
}

void savegame(Player &p, int slot) {
    string filename = getSaveFilename(slot);
    ofstream f(filename);
    if (!f.is_open()) {
        cout << "  " << Color::RED << "Gagal menyimpan!" << Color::RESET << endl;
        sleepMs(800);
        return;
    }

    // Player data
    f << p.name << endl;
    f << p.level << " " << p.exp << " " << p.expToNext << endl;
    f << p.hp << " " << p.maxHp << " " << p.mp << " " << p.maxMp << endl;
    f << p.attack << " " << p.defense << endl;
    f << p.coin << " " << p.swordLevel << " " << p.towerCleared << " " << p.potionCount << endl;
    f << p.chapter << " " << p.critRate << " " << p.dodgeRate << " " << p.killCount << endl;
    f << p.questsCompleted << " " << p.companionMet << " " << p.companionSaved << " " << p.companionHelped << endl;

    // Inventory
    f << inventoryCount << endl;
    for (int i = 0; i < inventoryCount; i++) {
        f << inventory[i].id << " " << inventory[i].count << endl;
    }

    // Equipped
    f << getEquippedArmorId() << " " << getEquippedAccessoryId() << endl;

    // Skills
    f << playerSkillCount << endl;
    for (int i = 0; i < playerSkillCount; i++) {
        f << playerSkills[i].id << endl;
    }

    // Quests
    f << activeQuestCount << endl;
    for (int i = 0; i < activeQuestCount; i++) {
        f << activeQuests[i].id << " "
          << activeQuests[i].currentCount << " "
          << activeQuests[i].completed << " "
          << activeQuests[i].claimed << endl;
    }

    // Lore
    f << loreCount << endl;
    for (int i = 0; i < loreCount; i++) {
        f << loreItems[i].id << endl;
    }

    f.close();

    // Save bestiary
    saveBestiary(getBestiaryFilename(slot));

    cout << "  " << Color::BRIGHT_GREEN << " Game disimpan ke Slot "
         << slot << "!" << Color::RESET << endl;
    sleepMs(800);
}

bool loadgame(Player &p, int slot) {
    string filename = getSaveFilename(slot);
    ifstream f(filename);
    if (!f.is_open()) return false;

    getline(f, p.name);
    f >> p.level >> p.exp >> p.expToNext;
    f >> p.hp >> p.maxHp >> p.mp >> p.maxMp;
    f >> p.attack >> p.defense;
    f >> p.coin >> p.swordLevel >> p.towerCleared >> p.potionCount;
    f >> p.chapter >> p.critRate >> p.dodgeRate >> p.killCount;
    if (f >> p.questsCompleted) {
        int met, saved, helped;
        f >> met >> saved >> helped;
        p.companionMet = (met != 0);
        p.companionSaved = (saved != 0);
        p.companionHelped = (helped != 0);
    } else {
        p.questsCompleted = 0;
        p.companionMet = false;
        p.companionSaved = false;
        p.companionHelped = false;
    }

    // Inventory
    initInventory();
    int invCount;
    f >> invCount;
    for (int i = 0; i < invCount; i++) {
        int id, count;
        f >> id >> count;
        addItem(id, count);
    }

    // Equipped
    int armorId, accId;
    f >> armorId >> accId;
    if (armorId >= 0) equipArmor(armorId);
    if (accId >= 0) equipAccessory(accId);

    // Skills
    initSkills();
    int skCount;
    f >> skCount;
    for (int i = 0; i < skCount; i++) {
        int id;
        f >> id;
        if (i < MAX_SKILLS) {
            playerSkills[i] = getSkillTemplate(id);
            playerSkillCount = i + 1;
        }
    }

    // Quests
    initQuests();
    int qCount;
    f >> qCount;
    for (int i = 0; i < qCount && i < MAX_QUESTS; i++) {
        int id, current, completed, claimed;
        f >> id >> current >> completed >> claimed;
        activeQuests[i] = getQuestTemplate(id);
        activeQuests[i].active = true;
        activeQuests[i].currentCount = current;
        activeQuests[i].completed = (completed != 0);
        activeQuests[i].claimed = (claimed != 0);
        activeQuestCount = i + 1;
    }

    // Lore
    int lCount;
    f >> lCount;
    for (int i = 0; i < lCount && i < MAX_LORE; i++) {
        int id;
        f >> id;
        addLoreItem(id);
    }

    f.close();

    // Load bestiary
    loadBestiary(getBestiaryFilename(slot));

    return true;
}

// ===========================================
//  Legacy savegame (for backward compat)
// ===========================================

void savegame(Player &p) {
    savegame(p, 1);
}

bool loadgame(Player &p) {
    return loadgame(p, 1);
}

// ===========================================
//  Save Slot Selection UI
// ===========================================

int showSaveSlotMenu(const string& action) {
    clearScreen();
    cout << endl;

    int w = 52;
    drawDoubleBox(w, " " + action + " - Pilih Slot ", Color::BRIGHT_GREEN);

    for (int s = 1; s <= 3; s++) {
        string filename = getSaveFilename(s);
        ifstream f(filename);
        string slotInfo;
        if (f.is_open()) {
            string name;
            int level;
            getline(f, name);
            f >> level;
            f.close();
            slotInfo = Color::BRIGHT_WHITE + name + Color::DIM + " (Lv." + to_string(level) + ")";
        } else {
            slotInfo = Color::DIM + "--- Kosong ---";
        }
        drawBoxLine(w, Color::BRIGHT_WHITE + " [" + to_string(s) + "] Slot " + to_string(s) + ": "
            + slotInfo + Color::RESET, Color::BRIGHT_GREEN);
    }

    drawSeparator(w, Color::BRIGHT_GREEN);
    drawBoxLine(w, Color::DIM + " [0] Batal" + Color::RESET, Color::BRIGHT_GREEN);
    drawDoubleBoxEnd(w, Color::BRIGHT_GREEN);

    return getChoice(0, 3);
}

// ===========================================
//  Title Screen
// ===========================================

void opening(int position) {
    drawTitleArt();

    string menuItems[] = {"Continue Game", "New Game", "Settings", "Quit Game"};
    int menuCount = 4;

    for (int i = 0; i < menuCount; i++) {
        string prefix;
        if (i == position) {
            prefix = Color::BOLD + Color::BRIGHT_YELLOW + "  " + Box::ARROW_R + " " + menuItems[i] + Color::RESET;
        } else {
            prefix = Color::DIM + "    " + menuItems[i] + Color::RESET;
        }
        cout << "  " << prefix << endl;
    }

    cout << endl;
    cout << "  " << Color::DIM << Color::BRIGHT_BLACK
         << "  Arrow Up/Down = Navigasi  |  Enter = Pilih"
         << Color::RESET << endl;
}
