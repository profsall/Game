#include "Config.h"
#include "TUI.h"
#include <fstream>
#include <iostream>
using namespace std;

GameConfig gameConfig;

bool isSoundEnabled() {
    return gameConfig.soundEnabled;
}

void initConfig() {
    gameConfig.textSpeed = 30;
    gameConfig.difficulty = 1; // Normal
    gameConfig.soundEnabled = true;
}

void saveConfig() {
    ofstream f("config.txt");
    if (f.is_open()) {
        f << gameConfig.textSpeed << endl;
        f << gameConfig.difficulty << endl;
        f << (gameConfig.soundEnabled ? 1 : 0) << endl;
        f.close();
    }
}

void loadConfig() {
    ifstream f("config.txt");
    if (f.is_open()) {
        f >> gameConfig.textSpeed;
        f >> gameConfig.difficulty;
        int snd;
        f >> snd;
        gameConfig.soundEnabled = (snd == 1);
        f.close();
    } else {
        initConfig();
    }
}

string getDifficultyName(int diff) {
    switch (diff) {
    case 0: return "Easy";
    case 1: return "Normal";
    case 2: return "Hard";
    default: return "Normal";
    }
}

float getDifficultyMultiplier(int diff) {
    switch (diff) {
    case 0: return 0.7f;  // enemies 30% weaker
    case 1: return 1.0f;
    case 2: return 1.4f;  // enemies 40% stronger
    default: return 1.0f;
    }
}

float getDifficultyRewardMultiplier(int diff) {
    switch (diff) {
    case 0: return 1.3f;  // 30% more rewards on easy
    case 1: return 1.0f;
    case 2: return 0.8f;  // 20% less rewards on hard
    default: return 1.0f;
    }
}

void showConfigMenu() {
    while (true) {
        clearScreen();
        cout << endl;

        int w = 54;
        drawDoubleBox(w, " SETTINGS ", Color::BRIGHT_YELLOW);

        drawBoxLine(w, Color::BRIGHT_CYAN + " Game    : " + Color::BRIGHT_WHITE + "Arcandia: Shadows of the Four Towers" + Color::RESET, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::BRIGHT_CYAN + " Version : " + Color::WHITE + "2.5 (TUI + Features)" + Color::RESET, Color::BRIGHT_YELLOW);

        drawSeparator(w, Color::BRIGHT_YELLOW);

        // Difficulty
        string diffColor;
        switch (gameConfig.difficulty) {
            case 0: diffColor = Color::BRIGHT_GREEN; break;
            case 1: diffColor = Color::BRIGHT_YELLOW; break;
            case 2: diffColor = Color::BRIGHT_RED; break;
            default: diffColor = Color::WHITE;
        }
        drawBoxLine(w, Color::BRIGHT_WHITE + " [1] Difficulty  : " + diffColor + getDifficultyName(gameConfig.difficulty) + Color::RESET, Color::BRIGHT_YELLOW);

        // Text Speed
        string speedLabel;
        if (gameConfig.textSpeed <= 15) speedLabel = "Fast";
        else if (gameConfig.textSpeed <= 40) speedLabel = "Normal";
        else speedLabel = "Slow";
        drawBoxLine(w, Color::BRIGHT_WHITE + " [2] Text Speed  : " + Color::BRIGHT_CYAN + speedLabel + Color::DIM + " (" + to_string(gameConfig.textSpeed) + "ms)" + Color::RESET, Color::BRIGHT_YELLOW);

        // Sound
        string sndStr = gameConfig.soundEnabled ? (Color::BRIGHT_GREEN + "ON") : (Color::BRIGHT_RED + "OFF");
        drawBoxLine(w, Color::BRIGHT_WHITE + " [3] Sound       : " + sndStr + Color::RESET, Color::BRIGHT_YELLOW);

        drawSeparator(w, Color::BRIGHT_YELLOW);

        drawBoxLine(w, Color::BRIGHT_WHITE + " Kontrol:" + Color::RESET, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::WHITE + "   Arrow Up/Down : Navigasi menu" + Color::RESET, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::WHITE + "   Enter         : Pilih" + Color::RESET, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::WHITE + "   Angka 1-9     : Pilih opsi" + Color::RESET, Color::BRIGHT_YELLOW);

        drawSeparator(w, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::DIM + " [0] Kembali (auto-save config)" + Color::RESET, Color::BRIGHT_YELLOW);
        drawDoubleBoxEnd(w, Color::BRIGHT_YELLOW);

        int choice = getChoice(0, 3);

        switch (choice) {
        case 1:
            gameConfig.difficulty = (gameConfig.difficulty + 1) % 3;
            break;
        case 2:
            if (gameConfig.textSpeed <= 15) gameConfig.textSpeed = 40;
            else if (gameConfig.textSpeed <= 40) gameConfig.textSpeed = 60;
            else gameConfig.textSpeed = 10;
            break;
        case 3:
            gameConfig.soundEnabled = !gameConfig.soundEnabled;
            break;
        case 0:
            saveConfig();
            return;
        }
    }
}
