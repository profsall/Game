#include "Tower.h"
#include "Bestiary.h"
#include "Config.h"
#include "Inventory.h"
#include "Quest.h"
#include "Skills.h"
#include "Story.h"
#include "TUI.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <windows.h>
using namespace std;

// ===========================================
//  Tower Data
// ===========================================

TowerInfo towers[4] = {
    {"Menara Hutan Gelap",  "Hutan",   Color::BRIGHT_GREEN,   5, 1, false},
    {"Gua Kristal",         "Gua",     Color::BRIGHT_BLUE,    5, 2, false},
    {"Menara Api Neraka",   "Api",     Color::BRIGHT_RED,     5, 3, false},
    {"Menara Kegelapan",    "Gelap",   Color::BRIGHT_MAGENTA, 5, 4, false}
};

// ===========================================
//  Monster Generation
// ===========================================

Enemy generateMonster(int towerTier, int floor) {
    float dm = getDifficultyMultiplier(gameConfig.difficulty);
    int tierBase = (towerTier - 1) * 30;
    int floorBonus = floor * 5;

    string names[4][5] = {
        {"Slime", "Goblin", "Wolf", "Spider", "Tree Golem"},
        {"Bat", "Golem", "Mushroom", "Cave Spider", "Ice Elemental"},
        {"Fire Imp", "Lava Snake", "Flame Golem", "Phoenix", "Magma Elemental"},
        {"Shadow", "Dark Knight", "Wraith", "Demon", "Void Walker"}
    };

    int idx = rand() % 5;
    int t = towerTier - 1;
    if (t < 0) t = 0;
    if (t > 3) t = 3;

    string name = names[t][idx];
    int hp  = (int)((tierBase + 40 + floorBonus + rand() % 20) * dm);
    int atk = (int)((tierBase / 3 + 8 + floor * 2 + rand() % 5) * dm);
    int def = (int)((tierBase / 4 + 3 + floor + rand() % 3) * dm);
    int expR = (int)((tierBase / 2 + 15 + floor * 5) * getDifficultyRewardMultiplier(gameConfig.difficulty));
    int coinR = (int)((tierBase / 3 + 10 + floor * 3) * getDifficultyRewardMultiplier(gameConfig.difficulty));

    return createEnemy(name, hp, atk, def, expR, coinR);
}

Enemy generateBoss(int towerTier) {
    float dm = getDifficultyMultiplier(gameConfig.difficulty);
    string bossNames[] = {
        "Raja Goblin",
        "Golem Kristal Raksasa",
        "Naga Api Inferno",
        "Dark Lord Umbra"
    };
    int t = towerTier - 1;
    if (t < 0) t = 0;
    if (t > 3) t = 3;

    int hp  = (int)((t * 80 + 200 + rand() % 50) * dm);
    int atk = (int)((t * 15 + 25 + rand() % 10) * dm);
    int def = (int)((t * 8 + 10 + rand() % 5) * dm);
    int expR = (int)((t * 100 + 200) * getDifficultyRewardMultiplier(gameConfig.difficulty));
    int coinR = (int)((t * 80 + 150) * getDifficultyRewardMultiplier(gameConfig.difficulty));

    return createEnemy(bossNames[t], hp, atk, def, expR, coinR);
}

// ===========================================
//  Display Battle Scene
// ===========================================

void displayBattleScene(Player &hero, Enemy &enemy) {
    clearScreen();
    drawBattleHeader();
    cout << endl;

    int w = 52;

    // Enemy
    drawDoubleBox(w, "" + enemy.name + "", Color::BRIGHT_RED);
    drawHealthBar("HP", enemy.hp, enemy.maxHp, 22);
    drawDoubleBoxEnd(w, Color::BRIGHT_RED);
    cout << endl;

    // Player
    drawDoubleBox(w, "" + hero.name + " (Lv." + to_string(hero.level) + ")", Color::BRIGHT_CYAN);
    drawHealthBar("HP", hero.hp, hero.maxHp, 22);
    drawManaBar("MP", hero.mp, hero.maxMp, 22);
    drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);
}

// ===========================================
//  Random Tower Events
// ===========================================

void randomTowerEvent(Player &hero, int towerTier) {
    int roll = rand() % 100;

    if (roll < 15) {
        // Treasure Chest (15%)
        clearScreen();
        cout << endl;

        int w = 50;
        drawDoubleBox(w, "PETI HARTA", Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::WHITE + " Kamu menemukan peti harta karun!" + Color::RESET, Color::BRIGHT_YELLOW);

        int coinFound = 20 + rand() % (towerTier * 30);
        hero.coin += coinFound;
        drawBoxLine(w, Color::BRIGHT_YELLOW + " +" + to_string(coinFound) + " Gold!" + Color::RESET, Color::BRIGHT_YELLOW);

        // Chance for item
        if (rand() % 100 < 40) {
            int possibleItems[] = {0, 1, 2, 12, 13, 14, 16, 17};
            int itemId = possibleItems[rand() % 8];
            addItem(itemId, 1);
            drawBoxLine(w, Color::BRIGHT_CYAN + " Item: " + getItemTemplate(itemId).name + "!" + Color::RESET, Color::BRIGHT_YELLOW);
            sfxItem();
        }

        drawDoubleBoxEnd(w, Color::BRIGHT_YELLOW);
        pausePrompt();

    } else if (roll < 25) {
        // Trap (10%)
        clearScreen();
        cout << endl;

        int w = 50;
        drawDoubleBox(w, "JEBAKAN!", Color::BRIGHT_RED);

        int dmg = 10 + rand() % (towerTier * 15);
        hero.hp -= dmg;
        if (hero.hp < 1) hero.hp = 1;

        drawBoxLine(w, Color::BRIGHT_RED + " Kamu terkena jebakan!" + Color::RESET, Color::BRIGHT_RED);
        drawBoxLine(w, Color::RED + " -" + to_string(dmg) + " HP!" + Color::RESET, Color::BRIGHT_RED);
        drawDoubleBoxEnd(w, Color::BRIGHT_RED);
        sfxDefeat();
        pausePrompt();

    } else if (roll < 35) {
        // Rest Spot (10%)
        clearScreen();
        cout << endl;

        int w = 50;
        drawDoubleBox(w, "TEMPAT ISTIRAHAT", Color::BRIGHT_GREEN);

        int heal = 20 + rand() % (towerTier * 10);
        hero.hp += heal;
        if (hero.hp > hero.maxHp) hero.hp = hero.maxHp;
        int mpHeal = 5 + rand() % 10;
        hero.mp += mpHeal;
        if (hero.mp > hero.maxMp) hero.mp = hero.maxMp;

        drawBoxLine(w, Color::WHITE + " Kamu menemukan tempat istirahat!" + Color::RESET, Color::BRIGHT_GREEN);
        drawBoxLine(w, Color::BRIGHT_GREEN + " +" + to_string(heal) + " HP!" + Color::RESET, Color::BRIGHT_GREEN);
        drawBoxLine(w, Color::BRIGHT_BLUE + " +" + to_string(mpHeal) + " MP!" + Color::RESET, Color::BRIGHT_GREEN);
        drawDoubleBoxEnd(w, Color::BRIGHT_GREEN);
        sfxHeal();
        pausePrompt();

    } else if (roll < 42) {
        // Wandering Merchant (7%)
        clearScreen();
        cout << endl;

        int w = 50;
        drawDoubleBox(w, "PEDAGANG KELILING", Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::WHITE + " \"Halo, mau beli sesuatu?\"" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawSeparator(w, Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [1] HP Potion   - 25G" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [2] MP Potion   - 35G" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [3] Bomb        - 45G" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::DIM + " [0] Tidak, terima kasih" + Color::RESET, Color::BRIGHT_MAGENTA);
        drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);

        int ch = getChoice(0, 3);
        int buyId = -1, cost = 0;
        if (ch == 1) { buyId = 0; cost = 25; }
        if (ch == 2) { buyId = 2; cost = 35; }
        if (ch == 3) { buyId = 16; cost = 45; }

        if (buyId >= 0 && hero.coin >= cost) {
            hero.coin -= cost;
            addItem(buyId, 1);
            sfxItem();
            cout << "  " << Color::BRIGHT_GREEN << "Berhasil membeli!" << Color::RESET << endl;
            sleepMs(800);
        } else if (buyId >= 0) {
            cout << "  " << Color::BRIGHT_RED << "Gold tidak cukup!" << Color::RESET << endl;
            sleepMs(800);
        }

    } else if (roll < 48) {
        // Lore Item (6%)
        int loreId = min(towerTier - 1, 3) * 2 + rand() % 2;
        if (!hasLoreItem(loreId)) {
            addLoreItem(loreId);
            clearScreen();
            cout << endl;

            int w = 50;
            drawDoubleBox(w, "ARTEFAK DITEMUKAN", Color::BRIGHT_MAGENTA);
            drawBoxLine(w, Color::BRIGHT_YELLOW + " " + loreItems[loreCount-1].name + Color::RESET, Color::BRIGHT_MAGENTA);
            drawBoxLine(w, Color::DIM + " " + loreItems[loreCount-1].description + Color::RESET, Color::BRIGHT_MAGENTA);
            drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);
            sfxItem();
            pausePrompt();
        }
    } else if (roll < 55) {
        // Flashback / Memory Fragment (7%)
        showFlashback();
    }
    // else nothing happens (45% of the time)
}

// ===========================================
//  Battle System
// ===========================================

bool battle(Player &hero, Enemy &enemy) {
    // Log encounter in bestiary
    logEncounter(enemy.name, enemy.maxHp, enemy.attack, enemy.defense);

    int buffAtkTurns = 0;
    int buffDefTurns = 0;
    float buffAtkMult = 1.0f;
    float buffDefMult = 1.0f;

    while (hero.hp > 0 && enemy.hp > 0) {
        displayBattleScene(hero, enemy);

        cout << endl;
        int w = 52;
        drawDoubleBox(w, "AKSI", Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [1] " + "Attack" + Color::RESET, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [2] " + " Defend" + Color::RESET, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [3] " + "Skill" + Color::RESET, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [4] " + "Item" + Color::RESET, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::DIM + " [5] " + "Kabur" + Color::RESET, Color::BRIGHT_YELLOW);
        drawDoubleBoxEnd(w, Color::BRIGHT_YELLOW);

        int choice = getChoice(1, 5);
        bool defending = false;
        bool playerActed = false;

        // Buff timer
        if (buffAtkTurns > 0) buffAtkTurns--;
        else buffAtkMult = 1.0f;
        if (buffDefTurns > 0) buffDefTurns--;
        else buffDefMult = 1.0f;

        switch (choice) {
        case 1: { // Attack
            animateAttack(hero.name, Color::BRIGHT_CYAN);
            sfxAttack();

            int totalAtk = (int)(getTotalAttack(hero) * buffAtkMult);
            int baseDmg = totalAtk - enemy.defense;
            if (baseDmg < 1) baseDmg = 1;
            int variance = rand() % (baseDmg / 4 + 1);
            int dmg = baseDmg + variance;

            bool isCrit = rollCritical(hero);
            if (isCrit) {
                dmg = (int)(dmg * 2.0);
                animateCritical();
                sfxCritical();
            }

            enemy.hp -= dmg;
            if (enemy.hp < 0) enemy.hp = 0;

            cout << "  " << Color::BRIGHT_RED << " " << hero.name
                 << " menyerang! -" << dmg << " HP";
            if (isCrit) cout << " (CRITICAL!)";
            cout << Color::RESET << endl;

            playerActed = true;
            break;
        }
        case 2: // Defend
            defending = true;
            playerActed = true;
            cout << "  " << Color::BRIGHT_BLUE << " " << hero.name
                 << " bertahan! DEF naik untuk turn ini." << Color::RESET << endl;
            break;

        case 3: { // Skill
            int skillId = showBattleSkills(hero.mp);
            if (skillId < 0) continue;

            Skill sk = getSkillTemplate(skillId);
            hero.mp -= sk.manaCost;

            animateSkillUse(sk.name, Color::BRIGHT_MAGENTA);

            if (sk.type == SKILL_ATTACK) {
                int totalAtk = (int)(getTotalAttack(hero) * buffAtkMult);
                int dmg = useAttackSkill(skillId, totalAtk, enemy.defense);

                bool isCrit = rollCritical(hero);
                if (isCrit) {
                    dmg = (int)(dmg * 1.5);
                    animateCritical();
                    sfxCritical();
                }

                enemy.hp -= dmg;
                if (enemy.hp < 0) enemy.hp = 0;

                cout << "  " << Color::BRIGHT_MAGENTA << " " << sk.name
                     << "! -" << dmg << " HP" << Color::RESET << endl;
            } else if (sk.type == SKILL_HEAL) {
                int heal = useHealSkill(skillId);
                if (heal >= 999) {
                    hero.hp = hero.maxHp;
                    cout << "  " << Color::BRIGHT_GREEN << " Full Restore! HP penuh!"
                         << Color::RESET << endl;
                } else {
                    hero.hp += heal;
                    if (hero.hp > hero.maxHp) hero.hp = hero.maxHp;
                    cout << "  " << Color::BRIGHT_GREEN << " " << sk.name
                         << "! +" << heal << " HP" << Color::RESET << endl;
                }
                sfxHeal();
            } else if (sk.type == SKILL_BUFF) {
                if (skillId == 8) { // War Cry - ATK buff
                    buffAtkMult = 1.0f + sk.power / 100.0f;
                    buffAtkTurns = 3;
                    cout << "  " << Color::BRIGHT_YELLOW << " ATK +"
                         << sk.power << "% untuk 3 turn!" << Color::RESET << endl;
                } else { // Iron Fortress - DEF buff
                    buffDefMult = 1.0f + sk.power / 100.0f;
                    buffDefTurns = 3;
                    cout << "  " << Color::BRIGHT_CYAN << " DEF +"
                         << sk.power << "% untuk 3 turn!" << Color::RESET << endl;
                }
            }

            playerActed = true;
            break;
        }
        case 4: { // Item
            int w2 = 50;
            drawDoubleBox(w2, "ITEM", Color::BRIGHT_GREEN);

            int battleItems[10];
            int battleItemCount = 0;
            for (int i = 0; i < inventoryCount && battleItemCount < 10; i++) {
                if (inventory[i].type == ITEM_CONSUMABLE || inventory[i].type == ITEM_SCROLL) {
                    drawBoxLine(w2, Color::BRIGHT_WHITE + " [" + to_string(battleItemCount + 1) + "] "
                        + inventory[i].name + " x" + to_string(inventory[i].count) + Color::RESET, Color::BRIGHT_GREEN);
                    battleItems[battleItemCount++] = inventory[i].id;
                }
            }
            drawBoxLine(w2, Color::DIM + " [0] Batal" + Color::RESET, Color::BRIGHT_GREEN);
            drawDoubleBoxEnd(w2, Color::BRIGHT_GREEN);

            if (battleItemCount == 0) {
                cout << "  " << Color::DIM << "Tidak ada item." << Color::RESET << endl;
                sleepMs(600);
                continue;
            }

            int itemChoice = getChoice(0, battleItemCount);
            if (itemChoice == 0) continue;

            int itemId = battleItems[itemChoice - 1];
            sfxItem();

            if (itemId == 0 || itemId == 1) { // HP Potions
                int heal = getItemTemplate(itemId).value;
                hero.hp += heal;
                if (hero.hp > hero.maxHp) hero.hp = hero.maxHp;
                removeItem(itemId, 1);
                cout << "  " << Color::BRIGHT_GREEN << " HP +" << heal << "!" << Color::RESET << endl;
                sfxHeal();
            } else if (itemId == 2) { // MP Potion
                hero.mp += 30;
                if (hero.mp > hero.maxMp) hero.mp = hero.maxMp;
                removeItem(itemId, 1);
                cout << "  " << Color::BRIGHT_BLUE << " MP +30!" << Color::RESET << endl;
            } else if (itemId == 3) { // Elixir
                hero.hp = hero.maxHp;
                hero.mp = hero.maxMp;
                removeItem(itemId, 1);
                cout << "  " << Color::BRIGHT_YELLOW << " Full HP + MP!" << Color::RESET << endl;
            } else if (itemId == 12) { // Scroll of Power
                buffAtkMult = 1.5f;
                buffAtkTurns = 99;
                removeItem(itemId, 1);
                cout << "  " << Color::BRIGHT_RED << " ATK +50% untuk battle ini!" << Color::RESET << endl;
            } else if (itemId == 13) { // Scroll of Shield
                buffDefMult = 1.5f;
                buffDefTurns = 99;
                removeItem(itemId, 1);
                cout << "  " << Color::BRIGHT_BLUE << " DEF +50% untuk battle ini!" << Color::RESET << endl;
            } else if (itemId == 16) { // Bomb
                enemy.hp -= 80;
                if (enemy.hp < 0) enemy.hp = 0;
                removeItem(itemId, 1);
                cout << "  " << Color::BRIGHT_RED << " BOOM! -80 HP!" << Color::RESET << endl;
            } else if (itemId == 17) { // Smoke Bomb - escape
                removeItem(itemId, 1);
                cout << "  " << Color::DIM << "Kamu menggunakan Smoke Bomb dan kabur!" << Color::RESET << endl;
                sleepMs(1000);
                return false;
            }

            playerActed = true;
            break;
        }
        case 5: { // Run
            int escapeChance = 40 + hero.dodgeRate;
            if (rand() % 100 < escapeChance) {
                cout << "  " << Color::DIM << "Kamu berhasil kabur!" << Color::RESET << endl;
                sleepMs(800);
                return false;
            } else {
                cout << "  " << Color::BRIGHT_RED << "Gagal kabur!" << Color::RESET << endl;
                playerActed = true;
            }
            break;
        }
        }

        sleepMs(600);

        if (!playerActed) continue;

        // Enemy turn
        if (enemy.hp > 0) {
            // Check player dodge
            if (rollDodge(hero)) {
                animateDodge();
                sfxDodge();
                cout << "  " << Color::BRIGHT_CYAN << " " << hero.name
                     << " menghindari serangan!" << Color::RESET << endl;
            } else {
                animateAttack(enemy.name, Color::BRIGHT_RED);
                sfxAttack();

                int totalDef = (int)(getTotalDefense(hero) * buffDefMult);
                if (defending) totalDef = (int)(totalDef * 1.5);

                int dmg = enemy.attack - totalDef;
                if (dmg < 1) dmg = 1;
                int variance = rand() % (dmg / 3 + 1);
                dmg += variance;

                hero.hp -= dmg;
                if (hero.hp < 0) hero.hp = 0;

                cout << "  " << Color::BRIGHT_RED << " " << enemy.name
                     << " menyerang! -" << dmg << " HP" << Color::RESET << endl;
            }
            sleepMs(600);
        }
    }

    // Battle result
    if (hero.hp > 0) {
        drawVictoryBanner();
        sfxVictory();

        // Rewards
        hero.exp += enemy.expReward;
        hero.coin += enemy.coinReward;
        hero.killCount++;

        cout << "  " << Color::BRIGHT_GREEN << "EXP +" << enemy.expReward << Color::RESET << endl;
        cout << "  " << Color::BRIGHT_YELLOW << "Gold +" << enemy.coinReward << "G" << Color::RESET << endl;
        cout << endl;

        // Bestiary
        logKill(enemy.name);

        // Quest progress
        updateQuestProgress(enemy.name);
        checkQuestCompletion();

        // Level up?
        levelUp(hero);

        pausePrompt();
        return true;
    } else {
        drawDefeatBanner();
        sfxDefeat();

        hero.hp = hero.maxHp / 4;
        hero.mp = hero.maxMp / 4;
        int lost = hero.coin / 4;
        hero.coin -= lost;

        cout << "  " << Color::BRIGHT_RED << "Kamu kalah..." << Color::RESET << endl;
        cout << "  " << Color::DIM << "HP dipulihkan 25%. Kehilangan " << lost << "G." << Color::RESET << endl;

        pausePrompt();
        return false;
    }
}

// ===========================================
//  Tower Select Menu
// ===========================================

void towerSelectMenu(Player &hero) {
    clearScreen();
    cout << endl;

    int w = 56;
    drawDoubleBox(w, "PILIH TOWER", Color::BRIGHT_RED);

    for (int i = 0; i < 4; i++) {
        string statusStr;
        if (hero.towerCleared > i) {
            statusStr = Color::BRIGHT_GREEN + "[CLEARED]" + Color::RESET;
        } else if (hero.towerCleared == i) {
            statusStr = Color::BRIGHT_YELLOW + "[AVAILABLE]" + Color::RESET;
        } else {
            statusStr = Color::BRIGHT_RED + "[LOCKED]" + Color::RESET;
        }

        drawBoxLine(w, Color::BRIGHT_WHITE + " [" + to_string(i+1) + "] "
            + towers[i].color + padRight(towers[i].name, 25)
            + statusStr, Color::BRIGHT_RED);
    }

    drawSeparator(w, Color::BRIGHT_RED);
    drawBoxLine(w, Color::DIM + " [0] Kembali" + Color::RESET, Color::BRIGHT_RED);
    drawDoubleBoxEnd(w, Color::BRIGHT_RED);

    int choice = getChoice(0, 4);
    if (choice == 0) return;

    int towerIndex = choice - 1;
    if (towerIndex > hero.towerCleared) {
        cout << "  " << Color::BRIGHT_RED << "Tower ini belum terbuka!" << Color::RESET << endl;
        sleepMs(1000);
        return;
    }

    enterTower(hero, choice);
}

// ===========================================
//  Enter Tower
// ===========================================

void enterTower(Player &hero, int towerNumber) {
    int t = towerNumber - 1;
    bool firstTime = (hero.towerCleared == t);

    // Chapter intro with dialog choices (Feature #2)
    if (firstTime)
        showChapterIntroWithChoices(towerNumber, hero);

    int totalFloors = towers[t].floors;

    for (int floor = 1; floor <= totalFloors; floor++) {
        clearScreen();
        cout << endl;

        // Minimap
        drawTowerMinimap(floor, totalFloors, towers[t].name);
        cout << endl;

        int w = 50;
        drawDoubleBox(w, towers[t].color + towers[t].name + " - Lantai " + to_string(floor) + Color::RESET, towers[t].color);
        drawDoubleBoxEnd(w, towers[t].color);

        sleepMs(600);

        // Companion encounter on floor 2 (Feature #3)
        if (floor == 2 && firstTime) {
            companionEncounter(towerNumber, hero);
        }

        // Mid-tower story beats on floors 3-4 (Feature #6)
        if ((floor == 3 || floor == 4) && firstTime) {
            showMidTowerBeat(towerNumber, floor);
        }

        // Random event before battle (not on boss floor)
        if (floor < totalFloors) {
            randomTowerEvent(hero, towerNumber);
        }

        // Generate enemy
        Enemy enemy;
        if (floor == totalFloors) {
            enemy = generateBoss(towerNumber);

            // Boss pre-battle dialog (Feature #1)
            showBossDialog(towerNumber);

            clearScreen();
            cout << endl;
            drawDoubleBox(w, "BOSS BATTLE!", Color::BRIGHT_RED);
            drawBoxLine(w, Color::BRIGHT_RED + " " + enemy.name + " muncul!" + Color::RESET, Color::BRIGHT_RED);
            drawDoubleBoxEnd(w, Color::BRIGHT_RED);
            sleepMs(1500);
        } else {
            enemy = generateMonster(towerNumber, floor);
        }

        bool won = battle(hero, enemy);

        if (!won) {
            cout << "  " << Color::DIM << "Kamu dikeluarkan dari tower..." << Color::RESET << endl;
            sleepMs(1000);
            return;
        }

        // Brief rest between floors
        if (floor < totalFloors) {
            int mpRegen = 3 + rand() % 5;
            hero.mp += mpRegen;
            if (hero.mp > hero.maxMp) hero.mp = hero.maxMp;
        }
    }

    // Tower cleared!
    if (firstTime) {
        hero.towerCleared = towerNumber;
        hero.chapter = towerNumber;
        towers[t].cleared = true;

        showChapterOutro(towerNumber);

        // Bonus reward
        int bonusCoin = towerNumber * 100;
        hero.coin += bonusCoin;
        cout << "  " << Color::BRIGHT_YELLOW << " Bonus Tower Clear: +"
             << bonusCoin << "G!" << Color::RESET << endl;

        // Special item drop from boss
        if (towerNumber <= 4) {
            int specialItems[] = {4, 5, 6, 7}; // armor progression
            addItem(specialItems[towerNumber - 1], 1);
            cout << "  " << Color::BRIGHT_CYAN << " Item langka: "
                 << getItemTemplate(specialItems[towerNumber - 1]).name << "!" << Color::RESET << endl;
            sfxItem();
        }

        sleepMs(1500);

        // Multiple endings (Feature #4)
        if (hero.towerCleared >= 4) {
            showEnding(hero);
        }
    } else {
        cout << "  " << Color::BRIGHT_GREEN << "Tower diselesaikan lagi! Bagus!" << Color::RESET << endl;
        sleepMs(1000);
    }
}
