#include "Skills.h"
#include "TUI.h"
#include <iostream>
#include <cstdlib>
using namespace std;

Skill playerSkills[MAX_SKILLS];
int playerSkillCount = 0;

// ═══════════════════════════════════════════
//  Skill Database
// ═══════════════════════════════════════════

static Skill skillDB[] = {
    // id, name, desc, type, manaCost, power, reqLevel
    {0, "Heavy Strike",   "Serangan kuat 1.5x damage",           SKILL_ATTACK, 8,  150, 2},
    {1, "Fire Slash",     "Tebasan api, 2x damage",              SKILL_ATTACK, 15, 200, 5},
    {2, "Thunder Blade",  "Pedang petir, 2.5x damage",           SKILL_ATTACK, 22, 250, 8},
    {3, "Shadow Assault", "Serangan bayangan, 3x damage",        SKILL_ATTACK, 30, 300, 12},
    {4, "Divine Fury",    "Serangan suci, 4x damage",            SKILL_ATTACK, 45, 400, 16},
    {5, "Healing Light",  "Pulihkan 60 HP",                      SKILL_HEAL,   10, 60,  3},
    {6, "Greater Heal",   "Pulihkan 150 HP",                     SKILL_HEAL,   25, 150, 9},
    {7, "Full Restore",   "Pulihkan semua HP",                   SKILL_HEAL,   40, 999, 14},
    {8, "War Cry",        "ATK +30% untuk 3 turn",               SKILL_BUFF,   12, 30,  4},
    {9, "Iron Fortress",  "DEF +50% untuk 3 turn",               SKILL_BUFF,   15, 50,  7},
};

// ═══════════════════════════════════════════
//  Functions
// ═══════════════════════════════════════════

void initSkills() {
    playerSkillCount = 0;
    for (int i = 0; i < MAX_SKILLS; i++)
        playerSkills[i] = {-1, "", "", SKILL_ATTACK, 0, 0, 0};
}

Skill getSkillTemplate(int skillId) {
    if (skillId >= 0 && skillId < TOTAL_SKILLS) return skillDB[skillId];
    return {-1, "Unknown", "", SKILL_ATTACK, 0, 0, 0};
}

void checkNewSkills(int playerLevel) {
    for (int s = 0; s < TOTAL_SKILLS; s++) {
        if (skillDB[s].requiredLevel == playerLevel) {
            // Check if already learned
            bool known = false;
            for (int i = 0; i < playerSkillCount; i++) {
                if (playerSkills[i].id == s) { known = true; break; }
            }
            if (!known && playerSkillCount < MAX_SKILLS) {
                playerSkills[playerSkillCount] = skillDB[s];
                playerSkillCount++;

                cout << endl;
                drawDoubleBox(48, Box::SPARKLE + " SKILL BARU! " + Box::SPARKLE, Color::BRIGHT_MAGENTA);
                drawBoxLine(48, Color::BRIGHT_WHITE + " " + skillDB[s].name + Color::RESET, Color::BRIGHT_MAGENTA);
                drawBoxLine(48, Color::DIM + " " + skillDB[s].description + Color::RESET, Color::BRIGHT_MAGENTA);
                drawBoxLine(48, Color::BRIGHT_BLUE + " MP Cost: " + to_string(skillDB[s].manaCost) + Color::RESET, Color::BRIGHT_MAGENTA);
                drawDoubleBoxEnd(48, Color::BRIGHT_MAGENTA);
                sleepMs(2000);
            }
        }
    }
}

int useAttackSkill(int skillId, int userAtk, int targetDef) {
    Skill sk = getSkillTemplate(skillId);
    // Power is percentage: 150 = 1.5x
    int baseDmg = userAtk - targetDef;
    if (baseDmg < 1) baseDmg = 1;
    int totalDmg = (baseDmg * sk.power) / 100;
    // Add some variance
    int variance = rand() % (totalDmg / 4 + 1);
    if (rand() % 2 == 0) totalDmg += variance;
    else totalDmg -= variance / 2;
    if (totalDmg < 1) totalDmg = 1;
    return totalDmg;
}

int useHealSkill(int skillId) {
    Skill sk = getSkillTemplate(skillId);
    return sk.power; // heal amount directly
}

// ═══════════════════════════════════════════
//  UI
// ═══════════════════════════════════════════

void showSkillsMenu() {
    clearScreen();
    cout << endl;

    int w = 52;
    drawDoubleBox(w, Box::STAR + " SKILL LIST " + Box::STAR, Color::BRIGHT_MAGENTA);

    if (playerSkillCount == 0) {
        drawBoxLine(w, Color::DIM + " Belum ada skill. Level up untuk unlock!" + Color::RESET, Color::BRIGHT_MAGENTA);
    } else {
        for (int i = 0; i < playerSkillCount; i++) {
            Skill &sk = playerSkills[i];
            string typeColor;
            string typeStr;
            switch (sk.type) {
                case SKILL_ATTACK: typeColor = Color::BRIGHT_RED; typeStr = "ATK"; break;
                case SKILL_HEAL: typeColor = Color::BRIGHT_GREEN; typeStr = "HEAL"; break;
                case SKILL_BUFF: typeColor = Color::BRIGHT_CYAN; typeStr = "BUFF"; break;
            }
            drawBoxLine(w, typeColor + " [" + typeStr + "] " + Color::BRIGHT_WHITE + padRight(sk.name, 18)
                + Color::BRIGHT_BLUE + "MP:" + to_string(sk.manaCost)
                + Color::DIM + " Pwr:" + to_string(sk.power) + Color::RESET, Color::BRIGHT_MAGENTA);
            drawBoxLine(w, Color::DIM + "       " + sk.description + Color::RESET, Color::BRIGHT_MAGENTA);
        }
    }

    drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);
    pausePrompt();
}

int showBattleSkills(int playerMp) {
    if (playerSkillCount == 0) {
        cout << "  " << Color::DIM << "Belum ada skill." << Color::RESET << endl;
        sleepMs(800);
        return -1;
    }

    int w = 52;
    drawDoubleBox(w, "SKILLS (MP: " + to_string(playerMp) + ")", Color::BRIGHT_MAGENTA);

    for (int i = 0; i < playerSkillCount; i++) {
        Skill &sk = playerSkills[i];
        string canUse = (playerMp >= sk.manaCost) ? Color::BRIGHT_WHITE : Color::DIM + Color::BRIGHT_BLACK;
        string typeChar;
        switch (sk.type) {
            case SKILL_ATTACK: typeChar = Box::SWORD; break;
            case SKILL_HEAL: typeChar = Box::HEART; break;
            case SKILL_BUFF: typeChar = Box::STAR; break;
        }
        drawBoxLine(w, canUse + " [" + to_string(i+1) + "] " + typeChar + " "
            + padRight(sk.name, 16) + Color::BRIGHT_BLUE + "MP:" + to_string(sk.manaCost)
            + Color::RESET, Color::BRIGHT_MAGENTA);
    }
    drawBoxLine(w, Color::DIM + " [0] Batal" + Color::RESET, Color::BRIGHT_MAGENTA);
    drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);

    int choice = getChoice(0, playerSkillCount);
    if (choice == 0) return -1;
    int idx = choice - 1;
    if (playerMp < playerSkills[idx].manaCost) {
        cout << "  " << Color::BRIGHT_RED << "MP tidak cukup!" << Color::RESET << endl;
        sleepMs(800);
        return -1;
    }
    return playerSkills[idx].id;
}
