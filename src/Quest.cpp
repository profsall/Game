#include "Quest.h"
#include "Entity.h"
#include "Inventory.h"
#include "TUI.h"
#include <iostream>
using namespace std;

Quest activeQuests[MAX_QUESTS];
int activeQuestCount = 0;

// ===========================================
//  Quest Database
// ===========================================

static Quest questDB[] = {
    // id, name, desc, giver, target, targetCount, current, expR, coinR, itemR, active, completed, claimed
    {0, "Pembersihan Slime",     "Kalahkan 3 Slime",                          "Tetua Desa",     "Slime",       3, 0, 40,  30,  -1, false, false, false},
    {1, "Ancaman Goblin",        "Kalahkan 5 Goblin",                         "Penjaga Gerbang","Goblin",      5, 0, 80,  60,  0,  false, false, false},
    {2, "Berburu Wolf",          "Kalahkan 3 Wolf",                            "Pemburu Desa",   "Wolf",        3, 0, 60,  50,  -1, false, false, false},
    {3, "Teror Kelelawar",       "Kalahkan 4 Bat di dalam Gua",               "Penjaga Gua",    "Bat",         4, 0, 100, 80,  2,  false, false, false},
    {4, "Golem di Gua Kristal",  "Kalahkan 3 Golem",                          "Penambang",      "Golem",       3, 0, 150, 120, 8,  false, false, false},
    {5, "Urusan Laba-Laba",      "Kalahkan 5 Spider",                         "Penduduk Desa",  "Spider",      5, 0, 130, 100, -1, false, false, false},
    {6, "Pemusnahan Iblis",      "Kalahkan 3 Fire Imp",                       "Alkemis Tua",    "Fire Imp",    3, 0, 200, 150, 12, false, false, false},
    {7, "Ular Lava",             "Kalahkan 4 Lava Snake",                     "Pandai Besi",    "Lava Snake",  4, 0, 250, 180, -1, false, false, false},
    {8, "Sang Bayangan",         "Kalahkan 5 Shadow",                         "Arwah Kesatria", "Shadow",      5, 0, 350, 250, 9,  false, false, false},
    {9, "Ksatria Kegelapan",     "Kalahkan 3 Dark Knight",                    "Arwah Kesatria", "Dark Knight", 3, 0, 400, 300, 13, false, false, false},
};

// ===========================================
//  Functions
// ===========================================

void initQuests() {
    activeQuestCount = 0;
    for (int i = 0; i < MAX_QUESTS; i++)
        activeQuests[i] = {-1, "", "", "", "", 0, 0, 0, 0, -1, false, false, false};
}

Quest getQuestTemplate(int questId) {
    if (questId >= 0 && questId < TOTAL_QUESTS) return questDB[questId];
    return {-1, "Unknown", "", "", "", 0, 0, 0, 0, -1, false, false, false};
}

bool acceptQuest(int questId) {
    if (activeQuestCount >= MAX_QUESTS) return false;
    // Check if already active
    for (int i = 0; i < activeQuestCount; i++)
        if (activeQuests[i].id == questId) return false;
    activeQuests[activeQuestCount] = getQuestTemplate(questId);
    activeQuests[activeQuestCount].active = true;
    activeQuestCount++;
    return true;
}

void updateQuestProgress(const string& monsterName) {
    for (int i = 0; i < activeQuestCount; i++) {
        if (activeQuests[i].active && !activeQuests[i].completed) {
            if (activeQuests[i].targetMonster == monsterName ||
                activeQuests[i].targetMonster.empty()) {
                activeQuests[i].currentCount++;
                if (activeQuests[i].currentCount >= activeQuests[i].targetCount) {
                    activeQuests[i].completed = true;
                }
            }
        }
    }
}

void checkQuestCompletion() {
    for (int i = 0; i < activeQuestCount; i++) {
        if (activeQuests[i].active && activeQuests[i].completed && !activeQuests[i].claimed) {
            cout << endl;
            cout << "  " << Color::BRIGHT_YELLOW << " Quest selesai: "
                 << Color::BRIGHT_WHITE << activeQuests[i].name << "!" << Color::RESET << endl;
            sleepMs(1000);
        }
    }
}

bool claimQuestReward(int questIndex, Player &hero) {
    if (questIndex < 0 || questIndex >= activeQuestCount) return false;
    Quest &q = activeQuests[questIndex];
    if (!q.completed || q.claimed) return false;

    hero.exp += q.rewardExp;
    hero.coin += q.rewardCoin;
    if (q.rewardItemId >= 0) {
        addItem(q.rewardItemId, 1);
    }
    q.claimed = true;
    return true;
}

int getAvailableQuestCount(int chapter) {
    int count = 0;
    for (int i = 0; i < TOTAL_QUESTS; i++) {
        // Simple chapter gating: quests 0-2 = ch1, 3-5 = ch2, 6-7 = ch3, 8-9 = ch4
        int questChapter;
        if (i <= 2) questChapter = 0;
        else if (i <= 5) questChapter = 1;
        else if (i <= 7) questChapter = 2;
        else questChapter = 3;

        if (questChapter <= chapter) {
            // Check not already active or completed
            bool activeOrDone = false;
            for (int j = 0; j < activeQuestCount; j++) {
                if (activeQuests[j].id == i) { activeOrDone = true; break; }
            }
            if (!activeOrDone) count++;
        }
    }
    return count;
}

// ===========================================
//  UI
// ===========================================

void showQuestBoard(Player &hero) {
    while (true) {
        clearScreen();
        cout << endl;

        int w = 56;
        drawDoubleBox(w, " PAPAN QUEST ", Color::BRIGHT_CYAN);

        // Show available quests
        drawBoxLine(w, Color::BRIGHT_WHITE + " Quest Tersedia:" + Color::RESET, Color::BRIGHT_CYAN);
        drawSeparator(w, Color::BRIGHT_CYAN);

        int displayIdx = 1;
        int questMap[TOTAL_QUESTS];
        int mapCount = 0;

        for (int i = 0; i < TOTAL_QUESTS; i++) {
            int questChapter;
            if (i <= 2) questChapter = 0;
            else if (i <= 5) questChapter = 1;
            else if (i <= 7) questChapter = 2;
            else questChapter = 3;

            if (questChapter > hero.chapter) continue;

            bool activeOrDone = false;
            for (int j = 0; j < activeQuestCount; j++)
                if (activeQuests[j].id == i) { activeOrDone = true; break; }
            if (activeOrDone) continue;

            Quest q = questDB[i];
            drawBoxLine(w, Color::BRIGHT_WHITE + " [" + to_string(displayIdx) + "] "
                + Color::BRIGHT_YELLOW + q.name + Color::RESET, Color::BRIGHT_CYAN);
            drawBoxLine(w, Color::DIM + "     " + q.description
                + " | " + Color::BRIGHT_GREEN + "EXP:" + to_string(q.rewardExp)
                + " Coin:" + to_string(q.rewardCoin) + "G" + Color::RESET, Color::BRIGHT_CYAN);
            drawBoxLine(w, Color::DIM + "     Dari: " + q.giver + Color::RESET, Color::BRIGHT_CYAN);

            questMap[mapCount++] = i;
            displayIdx++;
        }

        if (mapCount == 0) {
            drawBoxLine(w, Color::DIM + " Tidak ada quest baru saat ini." + Color::RESET, Color::BRIGHT_CYAN);
        }

        drawSeparator(w, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::DIM + " [0] Kembali" + Color::RESET, Color::BRIGHT_CYAN);
        drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);

        int choice = getChoice(0, mapCount);
        if (choice == 0) return;

        int questId = questMap[choice - 1];
        if (acceptQuest(questId)) {
            cout << "  " << Color::BRIGHT_GREEN << " Quest diterima: "
                 << questDB[questId].name << "!" << Color::RESET << endl;
            sleepMs(1000);
        } else {
            cout << "  " << Color::RED << "Tidak bisa menerima quest." << Color::RESET << endl;
            sleepMs(800);
        }
    }
}

void showActiveQuests() {
    clearScreen();
    cout << endl;

    int w = 56;
    drawDoubleBox(w, " QUEST AKTIF ", Color::BRIGHT_GREEN);

    if (activeQuestCount == 0) {
        drawBoxLine(w, Color::DIM + " Tidak ada quest aktif." + Color::RESET, Color::BRIGHT_GREEN);
    } else {
        for (int i = 0; i < activeQuestCount; i++) {
            Quest &q = activeQuests[i];
            string statusColor, statusStr;
            if (q.claimed) {
                statusColor = Color::DIM;
                statusStr = "[CLAIMED]";
            } else if (q.completed) {
                statusColor = Color::BRIGHT_YELLOW;
                statusStr = "[SELESAI!]";
            } else {
                statusColor = Color::BRIGHT_WHITE;
                statusStr = "[" + to_string(q.currentCount) + "/" + to_string(q.targetCount) + "]";
            }

            drawBoxLine(w, statusColor + " " + " " + q.name + " " + statusStr + Color::RESET, Color::BRIGHT_GREEN);
            drawBoxLine(w, Color::DIM + "   " + q.description + Color::RESET, Color::BRIGHT_GREEN);
        }
    }

    drawDoubleBoxEnd(w, Color::BRIGHT_GREEN);
    pausePrompt();
}
