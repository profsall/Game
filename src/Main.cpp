#include "Bestiary.h"
#include "Config.h"
#include "Entity.h"
#include "Filesystem.h"
#include "Inventory.h"
#include "Quest.h"
#include "Shop.h"
#include "Skills.h"
#include "Story.h"
#include "TUI.h"
#include "Tower.h"
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

// ===========================================
//  Rest at Inn
// ===========================================

void restAtInn(Player &hero) {
    clearScreen();
    cout << endl;

    int w = 50;
    drawDoubleBox(w, " PENGINAPAN DESA ", Color::BRIGHT_BLUE);
    drawEmptyLine(w, Color::BRIGHT_BLUE);

    drawBoxLine(w, Color::BRIGHT_GREEN + " " + " Pemilik Penginapan:" + Color::RESET, Color::BRIGHT_BLUE);
    drawBoxLine(w, Color::WHITE + "   \"Istirahatlah, kesatria...\"" + Color::RESET, Color::BRIGHT_BLUE);

    drawSeparator(w, Color::BRIGHT_BLUE);
    drawHealthBar("HP", hero.hp, hero.maxHp, 22);
    drawManaBar("MP", hero.mp, hero.maxMp, 22);
    drawDoubleBoxEnd(w, Color::BRIGHT_BLUE);

    cout << endl;
    cout << "  " << Color::WHITE << "Istirahat? (Gratis, HP + MP full)" << Color::RESET << endl;
    cout << "  " << Color::BRIGHT_WHITE << "[1] Ya  [2] Tidak" << Color::RESET << endl;

    int choice = getChoice(1, 2);

    if (choice == 1) {
        hero.hp = hero.maxHp;
        hero.mp = hero.maxMp;
        sfxHeal();
        cout << endl;
        cout << "  " << Color::BRIGHT_GREEN << ""
             << " Kamu beristirahat dengan nyaman..." << Color::RESET << endl;
        cout << "  " << Color::BRIGHT_CYAN
             << "HP dan MP dipulihkan!" << Color::RESET << endl;
        sleepMs(1500);
    }
}

// ===========================================
//  Wild Battle (Hunting)
// ===========================================

void wildBattle(Player &hero) {
    clearScreen();
    cout << endl;

    int w = 50;
    drawDoubleBox(w, " BERBURU MONSTER LIAR ", Color::BRIGHT_RED);
    drawEmptyLine(w, Color::BRIGHT_RED);
    drawDoubleBoxEnd(w, Color::BRIGHT_RED);

    int tier;
    if (hero.level < 5) tier = 1;
    else if (hero.level < 10) tier = 2;
    else if (hero.level < 15) tier = 3;
    else tier = 4;

    int floor = (rand() % 3) + 1;
    Enemy monster = generateMonster(tier, floor);

    cout << endl;
    cout << "  " << Color::BRIGHT_RED << " Seekor "
         << Color::BOLD << monster.name << Color::RESET
         << Color::BRIGHT_RED << " liar muncul!" << Color::RESET << endl;
    sleepMs(800);

    battle(hero, monster);
}

// ===========================================
//  Game Hub (Village)
// ===========================================

static int currentSaveSlot = 1;

void gameHub(Player &hero) {
    while (true) {
        clearScreen();
        cout << endl;

        int w = 58;

        // Village title
        string diffColor;
        switch (gameConfig.difficulty) {
            case 0: diffColor = Color::BRIGHT_GREEN; break;
            case 1: diffColor = Color::BRIGHT_YELLOW; break;
            case 2: diffColor = Color::BRIGHT_RED; break;
            default: diffColor = Color::WHITE;
        }
        drawDoubleBox(w, " DESA ARCANDIA " + Color::RESET + Color::DIM + " [" + getDifficultyName(gameConfig.difficulty) + "]" + Color::RESET, Color::BRIGHT_CYAN);

        // Player status bar
        string statusLine1 = Color::BRIGHT_WHITE + " " + " " + hero.name
            + Color::DIM + " | Lv." + to_string(hero.level)
            + " | " + Color::BRIGHT_GREEN + "HP:" + to_string(hero.hp) + "/" + to_string(hero.maxHp)
            + " " + Color::BRIGHT_BLUE + "MP:" + to_string(hero.mp) + "/" + to_string(hero.maxMp)
            + Color::DIM + " | " + Color::BRIGHT_YELLOW + to_string(hero.coin) + "G"
            + Color::RESET;
        drawBoxLine(w, statusLine1, Color::BRIGHT_CYAN);

        string statusLine2 = Color::DIM + " " + " " + Color::BRIGHT_MAGENTA + getSwordName(hero.swordLevel)
            + Color::DIM + " | Ch:" + Color::BRIGHT_CYAN + to_string(hero.chapter) + "/4"
            + Color::DIM + " | Tower:" + Color::BRIGHT_GREEN + to_string(hero.towerCleared) + "/4"
            + Color::DIM + " | Kills:" + Color::BRIGHT_RED + to_string(hero.killCount)
            + Color::RESET;
        drawBoxLine(w, statusLine2, Color::BRIGHT_CYAN);

        drawSeparator(w, Color::BRIGHT_CYAN);

        drawBoxLine(w, Color::BOLD + Color::BRIGHT_WHITE + " Apa yang ingin kamu lakukan?" + Color::RESET, Color::BRIGHT_CYAN);
        drawSeparator(w, Color::BRIGHT_CYAN);

        drawBoxLine(w, Color::BRIGHT_WHITE + " [1] " + Color::BRIGHT_RED + " Masuk Tower" + Color::RESET, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [2] " + Color::BRIGHT_YELLOW + " Berburu Monster Liar" + Color::RESET, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [3] " + Color::BRIGHT_MAGENTA + " Toko Kesatria (Shop)" + Color::RESET, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [4] " + Color::BRIGHT_BLUE + " Penginapan (Istirahat)" + Color::RESET, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [5] " + Color::BRIGHT_CYAN + " Status & Inventory" + Color::RESET, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [6] " + Color::BRIGHT_GREEN + " Quest Board" + Color::RESET, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [7] " + Color::BRIGHT_RED + " Bestiary" + Color::RESET, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [8] " + Color::BRIGHT_MAGENTA + " Lore Collection" + Color::RESET, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::BRIGHT_WHITE + " [9] " + Color::BRIGHT_GREEN + " Save Game" + Color::RESET, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::DIM + " [0] Kembali ke Menu Utama" + Color::RESET, Color::BRIGHT_CYAN);

        drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);

        int choice = getChoice(0, 9);

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
        case 5: {
            clearScreen();
            displayPlayerStats(hero);

            cout << "  " << Color::BRIGHT_WHITE << "[1] Inventory  [2] Skills  [0] Kembali" << Color::RESET << endl;
            int sub = getChoice(0, 2);
            if (sub == 1) showInventoryMenu(hero);
            else if (sub == 2) showSkillsMenu();
            break;
        }
        case 6:
            showQuestBoard(hero);
            // Check for completed quests to claim
            for (int i = 0; i < activeQuestCount; i++) {
                if (activeQuests[i].completed && !activeQuests[i].claimed) {
                    cout << "  " << Color::BRIGHT_YELLOW << "Claim reward untuk: "
                         << activeQuests[i].name << "?" << Color::RESET << endl;
                    cout << "  " << Color::BRIGHT_WHITE << "[1] Ya  [2] Nanti" << Color::RESET << endl;
                    int cl = getChoice(1, 2);
                    if (cl == 1) {
                        claimQuestReward(i, hero);
                        cout << "  " << Color::BRIGHT_GREEN << " Reward diterima!"
                             << Color::RESET << endl;
                        sfxItem();
                        sleepMs(800);
                    }
                }
            }
            break;
        case 7:
            showBestiary();
            break;
        case 8:
            showLoreCollection();
            break;
        case 9: {
            int slot = showSaveSlotMenu("SAVE");
            if (slot > 0) {
                currentSaveSlot = slot;
                savegame(hero, slot);
            }
            break;
        }
        case 0:
            cout << endl;
            cout << "  " << Color::WHITE << "Simpan game sebelum keluar?"
                 << Color::RESET << endl;
            cout << "  " << Color::BRIGHT_WHITE << "[1] Ya [2] Tidak" << Color::RESET << endl;
            {
                int saveChoice = getChoice(1, 2);
                if (saveChoice == 1) {
                    int slot = showSaveSlotMenu("SAVE");
                    if (slot > 0) savegame(hero, slot);
                }
            }
            return;
        }
    }
}

// ===========================================
//  Difficulty Selection
// ===========================================

void selectDifficulty() {
    clearScreen();
    cout << endl;

    int w = 52;
    drawDoubleBox(w, " PILIH KESULITAN ", Color::BRIGHT_YELLOW);
    drawEmptyLine(w, Color::BRIGHT_YELLOW);

    drawBoxLine(w, Color::BRIGHT_GREEN + " [1] Easy   " + Color::DIM + "- Musuh lebih lemah, reward banyak" + Color::RESET, Color::BRIGHT_YELLOW);
    drawBoxLine(w, Color::BRIGHT_YELLOW + " [2] Normal " + Color::DIM + "- Pengalaman standar" + Color::RESET, Color::BRIGHT_YELLOW);
    drawBoxLine(w, Color::BRIGHT_RED + " [3] Hard   " + Color::DIM + "- Musuh kuat, reward sedikit" + Color::RESET, Color::BRIGHT_YELLOW);

    drawEmptyLine(w, Color::BRIGHT_YELLOW);
    drawDoubleBoxEnd(w, Color::BRIGHT_YELLOW);

    int choice = getChoice(1, 3);
    gameConfig.difficulty = choice - 1;
    saveConfig();

    string diffColor;
    switch (gameConfig.difficulty) {
        case 0: diffColor = Color::BRIGHT_GREEN; break;
        case 1: diffColor = Color::BRIGHT_YELLOW; break;
        case 2: diffColor = Color::BRIGHT_RED; break;
        default: diffColor = Color::WHITE;
    }
    cout << "  " << diffColor << "Difficulty: " << getDifficultyName(gameConfig.difficulty)
         << Color::RESET << endl;
    sleepMs(800);
}

// ===========================================
//  Main
// ===========================================

int main() {
    srand((unsigned)time(0));

    // Initialize all systems
    initTUI();
    initConfig();
    loadConfig();
    initInventory();
    initSkills();
    initQuests();
    initBestiary();
    hideCursor();

    int positions = 0;
    int input;

    while (true) {
        // Main menu loop
        while (true) {
            opening(positions);
            input = _getch();

            if (input == 0 || input == 224) {
                input = _getch();
                if (input == 72) {
                    positions--;
                    if (positions < 0) positions = 3;
                    sfxMenuSelect();
                } else if (input == 80) {
                    positions++;
                    if (positions > 3) positions = 0;
                    sfxMenuSelect();
                }
            } else if (input == 13) {
                break;
            }
        }

        showCursor();
        clearScreen();

        if (positions == 0) {
            // ── Continue Game ──
            int slot = showSaveSlotMenu("LOAD");
            if (slot == 0) continue;

            Player hero;
            if (loadgame(hero, slot)) {
                currentSaveSlot = slot;
                cout << endl;
                int w = 50;
                drawDoubleBox(w, "SAVE DATA DITEMUKAN", Color::BRIGHT_GREEN);
                drawBoxLine(w, Color::BRIGHT_GREEN + " Selamat datang kembali, " + Color::BOLD + Color::BRIGHT_YELLOW + hero.name + "!" + Color::RESET, Color::BRIGHT_GREEN);
                drawBoxLine(w, Color::DIM + " Level: " + to_string(hero.level)
                    + " | Chapter: " + to_string(hero.chapter) + "/4"
                    + " | Kills: " + to_string(hero.killCount) + Color::RESET, Color::BRIGHT_GREEN);
                drawDoubleBoxEnd(w, Color::BRIGHT_GREEN);
                sleepMs(1500);
                gameHub(hero);
            } else {
                cout << endl;
                cout << "  " << Color::BRIGHT_RED
                     << "Slot kosong! Silakan pilih New Game." << Color::RESET << endl;
                sleepMs(1500);
            }

        } else if (positions == 1) {
            // ── New Game ──
            Player hero;
            string playerName;

            // Difficulty
            selectDifficulty();

            clearScreen();
            cout << endl;

            int w = 50;
            drawDoubleBox(w, " NEW GAME ", Color::BRIGHT_GREEN);
            drawEmptyLine(w, Color::BRIGHT_GREEN);
            drawBoxLine(w, Color::WHITE + " Petualangan baru dimulai..." + Color::RESET, Color::BRIGHT_GREEN);
            drawEmptyLine(w, Color::BRIGHT_GREEN);
            drawDoubleBoxEnd(w, Color::BRIGHT_GREEN);

            sleepMs(1000);

            // Prologue
            showPrologue();

            // Name input
            clearScreen();
            cout << endl;

            drawDoubleBox(w, " IDENTITAS KESATRIA ", Color::BRIGHT_YELLOW);
            drawEmptyLine(w, Color::BRIGHT_YELLOW);
            drawBoxLine(w, Color::WHITE + " Masukkan namamu, pahlawan:" + Color::RESET, Color::BRIGHT_YELLOW);
            drawDoubleBoxEnd(w, Color::BRIGHT_YELLOW);

            cout << endl;
            cout << "  " << Color::BRIGHT_CYAN << "Nama: " << Color::BRIGHT_WHITE;
            cin >> playerName;
            cout << Color::RESET;

            hero = createNewPlayer(playerName);

            // Initialize systems for new game
            initInventory();
            initSkills();
            initQuests();
            initBestiary();

            // Give starter items
            addItem(0, 3);  // 3 HP Potions
            addItem(2, 1);  // 1 MP Potion

            clearScreen();
            cout << endl;

            drawDoubleBox(w, " SELAMAT DATANG ", Color::BRIGHT_CYAN);
            drawEmptyLine(w, Color::BRIGHT_CYAN);
            string welcomeMsg = "Kesatria " + hero.name + ", petualanganmu dimulai!";
            drawBoxLine(w, Color::BRIGHT_WHITE + " " + welcomeMsg + Color::RESET, Color::BRIGHT_CYAN);
            drawEmptyLine(w, Color::BRIGHT_CYAN);
            drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);

            sleepMs(1500);

            // Show stats
            clearScreen();
            displayPlayerStats(hero);
            pausePrompt("Tekan ENTER untuk mulai berpetualang...");

            // Chapter 1 intro
            showChapterIntro(1);

            // Save slot selection
            int slot = showSaveSlotMenu("SAVE");
            if (slot > 0) {
                currentSaveSlot = slot;
                savegame(hero, slot);
            }

            gameHub(hero);

        } else if (positions == 2) {
            // ── Settings ──
            showConfigMenu();

        } else if (positions == 3) {
            // ── Quit Game ──
            clearScreen();
            cout << endl << endl;

            string c1 = Color::BRIGHT_CYAN;
            string c2 = Color::BRIGHT_YELLOW;
            string r  = Color::RESET;

            cout << "  " << c1 << Color::BOLD
                 << centerText("Terima kasih sudah bermain!", 56) << r << endl;
            cout << endl;
            cout << "  " << c2
                 << centerText("Arcandia: Shadows of the Four Towers", 56) << r << endl;
            cout << endl;

            cout << "  " << Color::DIM;
            for (int i = 0; i < 56; i++) cout << Box::H;
            cout << r << endl;
            cout << endl;

            showCursor();
            sleepMs(2000);
            return 0;
        }

        hideCursor();
    }

    showCursor();
    return 0;
}
