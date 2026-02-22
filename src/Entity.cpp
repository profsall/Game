#include "Entity.h"
#include "Inventory.h"
#include "Skills.h"
#include "TUI.h"
#include <cstdlib>
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
    p.mp = 30;
    p.maxMp = 30;
    p.attack = 10;
    p.defense = 5;
    p.coin = 0;
    p.swordLevel = 1;
    p.towerCleared = 0;
    p.potionCount = 1;
    p.chapter = 0;
    p.critRate = 5;
    p.dodgeRate = 3;
    p.killCount = 0;
    p.questsCompleted = 0;
    p.companionMet = false;
    p.companionSaved = false;
    p.companionHelped = false;
    return p;
}

int getSwordBonus(int swordLevel) {
    switch (swordLevel) {
    case 1: return 0;
    case 2: return 5;
    case 3: return 12;
    case 4: return 22;
    case 5: return 35;
    default: return 0;
    }
}

string getSwordName(int swordLevel) {
    switch (swordLevel) {
    case 1: return "Pedang Kayu";
    case 2: return "Pedang Besi";
    case 3: return "Pedang Baja";
    case 4: return "Pedang Mithril";
    case 5: return "Pedang Legenda";
    default: return "Unknown";
    }
}

int getTotalAttack(Player &p) {
    return p.attack + getSwordBonus(p.swordLevel) + getAccessoryBonus(2);
}

int getTotalDefense(Player &p) {
    return p.defense + getArmorDefenseBonus() + getAccessoryBonus(3);
}

void levelUp(Player &p) {
    while (p.exp >= p.expToNext) {
        p.exp -= p.expToNext;
        p.level++;
        p.maxHp += 15;
        p.hp = p.maxHp;
        p.maxMp += 5;
        p.mp = p.maxMp;
        p.attack += 3;
        p.defense += 2;
        p.critRate += 1;
        p.dodgeRate += 1;
        p.expToNext = p.level * 50;

        drawLevelUpBanner(p.level);

        cout << "  " << Color::BRIGHT_GREEN << "  HP Max  +" << 15
             << " " << Box::ARROW_R << " " << p.maxHp << Color::RESET << endl;
        cout << "  " << Color::BRIGHT_BLUE << "  MP Max  +" << 5
             << " " << Box::ARROW_R << " " << p.maxMp << Color::RESET << endl;
        cout << "  " << Color::BRIGHT_YELLOW << "  Attack  +" << 3
             << "  " << Box::ARROW_R << " " << p.attack << Color::RESET << endl;
        cout << "  " << Color::BRIGHT_CYAN << "  Defense +" << 2
             << "  " << Box::ARROW_R << " " << p.defense << Color::RESET << endl;
        cout << "  " << Color::BRIGHT_MAGENTA << "  Crit    +" << 1
             << "%  " << Box::ARROW_R << " " << p.critRate << "%" << Color::RESET << endl;
        cout << endl;

        // Check for new skills
        checkNewSkills(p.level);

        sleepMs(1500);
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

bool rollCritical(Player &p) {
    int totalCrit = p.critRate + getAccessoryBonus(0);
    return (rand() % 100) < totalCrit;
}

bool rollDodge(Player &p) {
    int totalDodge = p.dodgeRate + getAccessoryBonus(1);
    return (rand() % 100) < totalDodge;
}

void displayPlayerStats(Player &p) {
    int w = 52;
    cout << endl;

    drawDoubleBox(w, " STATUS KESATRIA ", Color::BRIGHT_CYAN);

    drawBoxLine(w, Color::BRIGHT_WHITE + " Nama    : " + Color::BRIGHT_YELLOW + p.name, Color::BRIGHT_CYAN);
    drawBoxLine(w, Color::BRIGHT_WHITE + " Level   : " + Color::BRIGHT_GREEN + to_string(p.level), Color::BRIGHT_CYAN);

    drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);

    drawHealthBar("HP", p.hp, p.maxHp, 22);
    drawManaBar("MP", p.mp, p.maxMp, 22);
    drawExpBar("EXP", p.exp, p.expToNext, 22);

    cout << endl;

    string armorStr = (getEquippedArmorId() >= 0)
        ? getItemTemplate(getEquippedArmorId()).name : "None";
    string accStr = (getEquippedAccessoryId() >= 0)
        ? getItemTemplate(getEquippedAccessoryId()).name : "None";

    vector<string> lines = {
        Color::BRIGHT_RED + " " + " Attack   : " + Color::WHITE + to_string(p.attack)
            + Color::DIM + " (+" + to_string(getSwordBonus(p.swordLevel)) + " sword"
            + " +" + to_string(getAccessoryBonus(2)) + " acc)" + Color::RESET,
        Color::BRIGHT_BLUE + " " + " Defense  : " + Color::WHITE + to_string(getTotalDefense(p))
            + Color::DIM + " (base:" + to_string(p.defense)
            + " +" + to_string(getArmorDefenseBonus()) + " armor)" + Color::RESET,
        Color::BRIGHT_YELLOW + " " + " Coin     : " + Color::WHITE + to_string(p.coin) + " G" + Color::RESET,
        Color::BRIGHT_MAGENTA + " " + " Pedang   : " + Color::WHITE + getSwordName(p.swordLevel) + Color::RESET,
        Color::BRIGHT_BLUE + " " + " Armor    : " + Color::WHITE + armorStr + Color::RESET,
        Color::BRIGHT_MAGENTA + " " + " Accessory: " + Color::WHITE + accStr + Color::RESET,
        Color::BRIGHT_RED + " " + " Crit Rate: " + Color::WHITE + to_string(p.critRate + getAccessoryBonus(0)) + "%" + Color::RESET,
        Color::BRIGHT_CYAN + " " + " Dodge    : " + Color::WHITE + to_string(p.dodgeRate + getAccessoryBonus(1)) + "%" + Color::RESET,
        Color::BRIGHT_GREEN + " " + " Kills    : " + Color::WHITE + to_string(p.killCount) + Color::RESET,
        Color::BRIGHT_CYAN + " " + " Tower    : " + Color::WHITE + to_string(p.towerCleared) + " / 4" + Color::RESET
    };

    drawPanel("DETAIL STATS", lines, w, Color::BRIGHT_YELLOW);
    cout << endl;
}
