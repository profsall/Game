#include "TUI.h"
#include <conio.h>
#include <iostream>
#include <windows.h>

using namespace std;

// ===========================================
//  Setup & Basics
// ===========================================

void initTUI() {
    // Enable Virtual Terminal Processing for ANSI on Windows
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);

    // Set console to UTF-8
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // Set console window size
    HWND console = GetConsoleWindow();
    if (console) {
        // Set a nice size
        SMALL_RECT rect = {0, 0, 79, 34};
        SetConsoleWindowInfo(hOut, TRUE, &rect);
    }

    // Set console title
    SetConsoleTitleW(L"Arcandia: Shadows of the Four Towers");
}

void clearScreen() {
    cout << "\033[2J\033[H";
    cout.flush();
}

void hideCursor() {
    cout << "\033[?25l";
}

void showCursor() {
    cout << "\033[?25h";
}

void moveCursor(int row, int col) {
    cout << "\033[" << row << ";" << col << "H";
}

void sleepMs(int ms) {
    Sleep(ms);
}

// ===========================================
//  Text Helpers
// ===========================================

// Calculate visible length (ignoring ANSI codes)
int visibleLength(const string& text) {
    int len = 0;
    bool inEscape = false;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '\033') {
            inEscape = true;
            continue;
        }
        if (inEscape) {
            if (text[i] == 'm') inEscape = false;
            continue;
        }
        // Handle UTF-8 multi-byte sequences
        unsigned char c = (unsigned char)text[i];
        if (c < 0x80) {
            len++;
        } else if ((c & 0xE0) == 0xC0) {
            len++; // 2-byte
            i += 1;
        } else if ((c & 0xF0) == 0xE0) {
            len += 2; // 3-byte (most Unicode symbols are double-width)
            i += 2;
        } else if ((c & 0xF8) == 0xF0) {
            len += 2; // 4-byte
            i += 3;
        }
    }
    return len;
}

string centerText(const string& text, int width) {
    int vLen = visibleLength(text);
    if (vLen >= width) return text;
    int leftPad = (width - vLen) / 2;
    int rightPad = width - vLen - leftPad;
    return string(leftPad, ' ') + text + string(rightPad, ' ');
}

string padRight(const string& text, int width) {
    int vLen = visibleLength(text);
    if (vLen >= width) return text;
    return text + string(width - vLen, ' ');
}

// ===========================================
//  Rendering — Box Drawing
// ===========================================

void drawDoubleBox(int width, const string& title, const string& borderColor) {
    cout << "  " << borderColor << Box::TL;
    for (int i = 0; i < width - 2; i++) cout << Box::H;
    cout << Box::TR << Color::RESET << endl;

    if (!title.empty()) {
        string centered = centerText(title, width - 4);
        cout << "  " << borderColor << Box::V << Color::RESET
             << " " << Color::BOLD << Color::BRIGHT_WHITE << centered << Color::RESET
             << " " << borderColor << Box::V << Color::RESET << endl;

        // Separator after title
        cout << "  " << borderColor << Box::LT;
        for (int i = 0; i < width - 2; i++) cout << Box::H;
        cout << Box::RT << Color::RESET << endl;
    }
}

void drawDoubleBoxEnd(int width, const string& borderColor) {
    cout << "  " << borderColor << Box::BL;
    for (int i = 0; i < width - 2; i++) cout << Box::H;
    cout << Box::BR << Color::RESET << endl;
}

void drawBoxLine(int width, const string& content, const string& borderColor) {
    string padded = padRight(content, width - 4);
    cout << "  " << borderColor << Box::V << Color::RESET
         << " " << padded << " "
         << borderColor << Box::V << Color::RESET << endl;
}

void drawSeparator(int width, const string& borderColor) {
    cout << "  " << borderColor << Box::LT;
    for (int i = 0; i < width - 2; i++) cout << Box::H;
    cout << Box::RT << Color::RESET << endl;
}

void drawEmptyLine(int width, const string& borderColor) {
    cout << "  " << borderColor << Box::V << Color::RESET;
    for (int i = 0; i < width - 2; i++) cout << " ";
    cout << borderColor << Box::V << Color::RESET << endl;
}

void drawPanel(const string& title, const vector<string>& lines, int width, const string& borderColor) {
    drawDoubleBox(width, title, borderColor);
    for (const auto& line : lines) {
        drawBoxLine(width, line, borderColor);
    }
    drawDoubleBoxEnd(width, borderColor);
}

// ===========================================
//  Animated Text
// ===========================================

void typewriter(const string& text, int delayMs, const string& color) {
    cout << color;
    for (size_t i = 0; i < text.size(); i++) {
        unsigned char c = (unsigned char)text[i];
        if (c < 0x80) {
            cout << text[i];
        } else if ((c & 0xE0) == 0xC0 && i + 1 < text.size()) {
            cout << text[i] << text[i+1];
            i += 1;
        } else if ((c & 0xF0) == 0xE0 && i + 2 < text.size()) {
            cout << text[i] << text[i+1] << text[i+2];
            i += 2;
        } else if ((c & 0xF8) == 0xF0 && i + 3 < text.size()) {
            cout << text[i] << text[i+1] << text[i+2] << text[i+3];
            i += 3;
        } else {
            cout << text[i];
        }
        cout.flush();
        Sleep(delayMs);
    }
    cout << Color::RESET;
}

void typewriterLine(const string& text, int delayMs, const string& color) {
    typewriter(text, delayMs, color);
    cout << endl;
}

void dramaticPause(int ms) {
    Sleep(ms);
}

// ===========================================
//  Health & Progress Bars
// ===========================================

void drawHealthBar(const string& label, int current, int max, int barWidth) {
    if (current < 0) current = 0;
    if (current > max) current = max;
    int filled = (max > 0) ? (current * barWidth) / max : 0;
    float ratio = (max > 0) ? (float)current / max : 0;

    string barColor;
    if (ratio > 0.6f) barColor = Color::BRIGHT_GREEN;
    else if (ratio > 0.3f) barColor = Color::BRIGHT_YELLOW;
    else barColor = Color::BRIGHT_RED;

    string bar = "  " + Color::BOLD + Color::WHITE + padRight(label, 10) + Color::RESET;
    bar += Color::DIM + "[" + Color::RESET;
    for (int i = 0; i < barWidth; i++) {
        if (i < filled)
            bar += barColor + Box::BLOCK_FULL + Color::RESET;
        else
            bar += Color::BRIGHT_BLACK + Box::BLOCK_LIGHT + Color::RESET;
    }
    bar += Color::DIM + "]" + Color::RESET;
    bar += " " + barColor + to_string(current) + "/" + to_string(max) + Color::RESET;
    cout << bar << endl;
}

void drawExpBar(const string& label, int current, int max, int barWidth) {
    if (current < 0) current = 0;
    if (current > max) current = max;
    int filled = (max > 0) ? (current * barWidth) / max : 0;

    string bar = "  " + Color::BOLD + Color::WHITE + padRight(label, 10) + Color::RESET;
    bar += Color::DIM + "[" + Color::RESET;
    for (int i = 0; i < barWidth; i++) {
        if (i < filled)
            bar += Color::BRIGHT_CYAN + Box::BLOCK_FULL + Color::RESET;
        else
            bar += Color::BRIGHT_BLACK + Box::BLOCK_LIGHT + Color::RESET;
    }
    bar += Color::DIM + "]" + Color::RESET;
    bar += " " + Color::BRIGHT_CYAN + to_string(current) + "/" + to_string(max) + Color::RESET;
    cout << bar << endl;
}

void drawManaBar(const string& label, int current, int max, int barWidth) {
    if (current < 0) current = 0;
    int filled = (max > 0) ? (current * barWidth) / max : 0;

    string bar = "  " + Color::BOLD + Color::WHITE + padRight(label, 10) + Color::RESET;
    bar += Color::DIM + "[" + Color::RESET;
    for (int i = 0; i < barWidth; i++) {
        if (i < filled)
            bar += Color::BRIGHT_BLUE + Box::BLOCK_FULL + Color::RESET;
        else
            bar += Color::BRIGHT_BLACK + Box::BLOCK_LIGHT + Color::RESET;
    }
    bar += Color::DIM + "]" + Color::RESET;
    bar += " " + Color::BRIGHT_BLUE + to_string(current) + "/" + to_string(max) + Color::RESET;
    cout << bar << endl;
}

// ===========================================
//  Menu System
// ===========================================

int drawMenu(const string& title, const vector<string>& items, const string& borderColor, int width) {
    int selected = 0;
    int itemCount = (int)items.size();

    while (true) {
        clearScreen();
        cout << endl;

        drawDoubleBox(width, title, borderColor);
        drawEmptyLine(width, borderColor);

        for (int i = 0; i < itemCount; i++) {
            string line;
            if (i == selected) {
                line = Color::BOLD + borderColor + "  " + Box::ARROW_R + " " + Color::BRIGHT_WHITE + items[i] + Color::RESET;
            } else {
                line = Color::DIM + "    " + items[i] + Color::RESET;
            }
            drawBoxLine(width, line, borderColor);
        }

        drawEmptyLine(width, borderColor);
        drawBoxLine(width,
            Color::DIM + Color::BRIGHT_BLACK + "  Arrow Up/Down = Navigasi  |  Enter = Pilih" + Color::RESET,
            borderColor);
        drawDoubleBoxEnd(width, borderColor);

        int input = _getch();
        if (input == 0 || input == 224) {
            input = _getch();
            if (input == 72) { // Up
                selected--;
                if (selected < 0) selected = itemCount - 1;
            } else if (input == 80) { // Down
                selected++;
                if (selected >= itemCount) selected = 0;
            }
        } else if (input == 13) { // Enter
            return selected;
        }
    }
}

// ===========================================
//  Prompt & Input
// ===========================================

void pausePrompt(const string& msg) {
    cout << endl;
    cout << "  " << Color::DIM << Color::BRIGHT_BLACK << msg << Color::RESET;
    cout.flush();
    _getch();
    cout << endl;
}

int getChoice(int min, int max) {
    int choice;
    while (true) {
        cout << "  " << Color::BRIGHT_CYAN << Box::ARROW_R << " " << Color::RESET;
        if (cin >> choice) {
            if (choice >= min && choice <= max) return choice;
        } else {
            cin.clear();
            cin.ignore(10000, '\n');
        }
        cout << "  " << Color::RED << "Pilihan tidak valid! (" << min << "-" << max << ")" << Color::RESET << endl;
    }
}

// ===========================================
//  Fancy Displays
// ===========================================

void drawTitleArt() {
    clearScreen();
    cout << endl;

    string c1 = Color::BRIGHT_CYAN;
    string c2 = Color::CYAN;
    string c3 = Color::BRIGHT_MAGENTA;
    string c4 = Color::BRIGHT_YELLOW;
    string r  = Color::RESET;

    cout << c1 << "       ___                                _ _       " << r << endl;
    cout << c1 << "      / _ \\                              | (_)      " << r << endl;
    cout << c2 << "     / /_\\ \\_ __ ___ __ _ _ __   __| |_  __ _ " << r << endl;
    cout << c2 << "     |  _  | '__/ __/ _` | '_ \\ / _` | |/ _` |" << r << endl;
    cout << c3 << "     | | | | | | (_| (_| | | | | (_| | | (_| |" << r << endl;
    cout << c3 << "     \\_| |_/_|  \\___\\__,_|_| |_|\\__,_|_|\\__,_|" << r << endl;
    cout << endl;

    string subtitle = "S H A D O W S   O F   T H E   F O U R   T O W E R S";
    cout << "  " << c4 << Color::BOLD << centerText(subtitle, 56) << r << endl;
    cout << endl;

    // Tower ASCII art
    cout << c2 << "                    " << Box::BLOCK_DARK << Box::BLOCK_DARK << Box::BLOCK_DARK << r << endl;
    cout << c2 << "                   " << Box::BLOCK_DARK << Box::BLOCK_MED << Box::BLOCK_MED << Box::BLOCK_MED << Box::BLOCK_DARK << r << endl;
    cout << c1 << "                  " << Box::BLOCK_DARK << Box::BLOCK_MED << Box::BLOCK_LIGHT << Box::BLOCK_LIGHT << Box::BLOCK_LIGHT << Box::BLOCK_MED << Box::BLOCK_DARK << r << endl;
    cout << c1 << "                 " << Box::BLOCK_DARK << Box::BLOCK_MED << Box::BLOCK_LIGHT << "   " << Box::BLOCK_LIGHT << Box::BLOCK_MED << Box::BLOCK_DARK << r << endl;
    cout << c1 << "                 " << Box::BLOCK_DARK << Box::BLOCK_MED << Box::BLOCK_MED << Box::BLOCK_MED << Box::BLOCK_MED << Box::BLOCK_MED << Box::BLOCK_MED << Box::BLOCK_MED << Box::BLOCK_DARK << r << endl;
    cout << c2 << "                 " << Box::BLOCK_DARK << Box::BLOCK_DARK << Box::BLOCK_DARK << Box::BLOCK_DARK << Box::BLOCK_DARK << Box::BLOCK_DARK << Box::BLOCK_DARK << Box::BLOCK_DARK << Box::BLOCK_DARK << r << endl;
    cout << endl;

    // Decorative line
    cout << "  " << c2;
    for (int i = 0; i < 56; i++) cout << Box::H;
    cout << r << endl;
    cout << endl;
}

void drawBattleHeader() {
    string c = Color::BRIGHT_RED;
    string r = Color::RESET;
    int w = 52;

    cout << endl;
    cout << "  " << c << Box::TL;
    for (int i = 0; i < w - 2; i++) cout << Box::H;
    cout << Box::TR << r << endl;

    string title = " B A T T L E ";
    cout << "  " << c << Box::V << r
         << Color::BOLD << Color::BRIGHT_RED << centerText(title, w - 2) << r
         << c << Box::V << r << endl;

    cout << "  " << c << Box::BL;
    for (int i = 0; i < w - 2; i++) cout << Box::H;
    cout << Box::BR << r << endl;
}

void drawVictoryBanner() {
    string c = Color::BRIGHT_YELLOW;
    string r = Color::RESET;
    int w = 50;

    cout << endl;
    cout << "  " << c << Color::BOLD;
    cout << "  " << " " << " " << "";
    cout << "  V I C T O R Y  ";
    cout << " " << " " << "";
    cout << r << endl;
    cout << endl;
}

void drawDefeatBanner() {
    string c = Color::BRIGHT_RED;
    string r = Color::RESET;

    cout << endl;
    cout << "  " << c << Color::BOLD;
    cout << "  " << "  D E F E A T  " << "";
    cout << r << endl;
    cout << endl;
}

void drawLevelUpBanner(int newLevel) {
    string c1 = Color::BRIGHT_YELLOW;
    string c2 = Color::BRIGHT_GREEN;
    string r  = Color::RESET;
    int w = 44;

    cout << endl;
    cout << "  " << c1 << Box::TL;
    for (int i = 0; i < w - 2; i++) cout << Box::H;
    cout << Box::TR << r << endl;

    string title = " L E V E L   U P ! ";
    cout << "  " << c1 << Box::V << r
         << c1 << Color::BOLD << centerText(title, w - 2) << r
         << c1 << Box::V << r << endl;

    string lvText = "Level " + to_string(newLevel) + " tercapai!";
    cout << "  " << c1 << Box::V << r
         << c2 << centerText(lvText, w - 2) << r
         << c1 << Box::V << r << endl;

    cout << "  " << c1 << Box::BL;
    for (int i = 0; i < w - 2; i++) cout << Box::H;
    cout << Box::BR << r << endl;
    cout << endl;
}

void drawChapterTitle(int chapter, const string& title, const string& subtitle) {
    clearScreen();
    string c1 = Color::BRIGHT_YELLOW;
    string c2 = Color::BRIGHT_CYAN;
    string c3 = Color::WHITE;
    string r  = Color::RESET;
    int w = 56;

    cout << endl << endl << endl;

    cout << "  " << c1 << Color::DIM;
    for (int i = 0; i < w; i++) cout << Box::H;
    cout << r << endl;
    cout << endl;

    string chapStr = "C H A P T E R   " + to_string(chapter);
    cout << "  " << c1 << Color::BOLD << centerText(chapStr, w) << r << endl;
    cout << endl;

    cout << "  " << c2 << Color::BOLD << centerText(title, w) << r << endl;
    cout << endl;

    if (!subtitle.empty()) {
        cout << "  " << c3 << Color::DIM << centerText(subtitle, w) << r << endl;
    }

    cout << endl;

    cout << "  " << c1 << Color::DIM;
    for (int i = 0; i < w; i++) cout << Box::H;
    cout << r << endl;

    cout << endl << endl;
    sleepMs(1500);
}

// ===========================================
//  Sound Effects (Windows Beep)
// ===========================================

// Forward declare config check
extern bool isSoundEnabled();

void sfxAttack() {
    if (!isSoundEnabled()) return;
    Beep(400, 80);
    Beep(300, 60);
}

void sfxCritical() {
    if (!isSoundEnabled()) return;
    Beep(600, 60);
    Beep(800, 80);
    Beep(1000, 100);
}

void sfxDodge() {
    if (!isSoundEnabled()) return;
    Beep(500, 40);
    Beep(700, 40);
}

void sfxHeal() {
    if (!isSoundEnabled()) return;
    Beep(523, 100);
    Beep(659, 100);
    Beep(784, 150);
}

void sfxLevelUp() {
    if (!isSoundEnabled()) return;
    Beep(523, 80);
    Beep(659, 80);
    Beep(784, 80);
    Beep(1047, 200);
}

void sfxVictory() {
    if (!isSoundEnabled()) return;
    Beep(523, 100);
    Beep(659, 100);
    Beep(784, 100);
    Beep(1047, 200);
    Beep(784, 100);
    Beep(1047, 300);
}

void sfxDefeat() {
    if (!isSoundEnabled()) return;
    Beep(400, 200);
    Beep(350, 200);
    Beep(300, 200);
    Beep(250, 400);
}

void sfxItem() {
    if (!isSoundEnabled()) return;
    Beep(600, 60);
    Beep(800, 80);
}

void sfxMenuSelect() {
    if (!isSoundEnabled()) return;
    Beep(600, 30);
}

// ===========================================
//  Battle Animations
// ===========================================

void animateAttack(const string& attackerName, const string& color) {
    string frames[] = {
        "     " + color + attackerName + Color::RESET + "  " + Color::BRIGHT_RED + "-->" + Color::RESET,
        "     " + color + attackerName + Color::RESET + "  " + Color::BRIGHT_RED + "--->" + Color::RESET,
        "     " + color + attackerName + Color::RESET + "  " + Color::BRIGHT_RED + "---->" + Color::RESET,
        "     " + color + attackerName + Color::RESET + "  " + Color::BRIGHT_RED + "----->" + Color::RESET + " " + Color::BRIGHT_YELLOW + Color::RESET,
    };
    for (int i = 0; i < 4; i++) {
        cout << "\r  " << padRight(frames[i], 55);
        cout.flush();
        Sleep(100);
    }
    cout << endl;
}

void animateCritical() {
    string c = Color::BRIGHT_YELLOW;
    string r = Color::RESET;
    cout << endl;
    cout << "  " << c << Color::BOLD << "  " << ""
         << " C R I T I C A L ! " << r << endl;
    cout << endl;
}

void animateDodge() {
    string c = Color::BRIGHT_CYAN;
    string r = Color::RESET;
    cout << endl;
    cout << "  " << c << Color::BOLD << "  ~ D O D G E ! ~" << r << endl;
    cout << endl;
}

void animateSkillUse(const string& skillName, const string& color) {
    string r = Color::RESET;
    cout << endl;
    cout << "  " << color << Color::BOLD << "  " << " "
         << skillName << " " << r << endl;
    Sleep(200);
    cout << "  " << color;
    for (int i = 0; i < 40; i++) {
        cout << "";
        cout.flush();
        Sleep(15);
    }
    cout << r << endl;
    Sleep(200);
}

// ===========================================
//  Tower Minimap
// ===========================================

void drawTowerMinimap(int currentFloor, int totalFloors, const string& towerName) {
    int w = 30;
    string bc = Color::BRIGHT_YELLOW;
    string r = Color::RESET;

    cout << "  " << bc << Box::TL;
    for (int i = 0; i < w - 2; i++) cout << Box::H;
    cout << Box::TR << r << endl;

    string header = "" + towerName;
    cout << "  " << bc << Box::V << r << Color::BRIGHT_WHITE << " " << padRight(header, w - 3)
         << bc << Box::V << r << endl;

    cout << "  " << bc << Box::LT;
    for (int i = 0; i < w - 2; i++) cout << Box::H;
    cout << Box::RT << r << endl;

    // Draw floors from top to bottom
    for (int f = totalFloors; f >= 1; f--) {
        string floorLabel;
        string floorColor;
        if (f == currentFloor) {
            floorColor = Color::BRIGHT_GREEN;
            floorLabel = floorColor + " " + Box::ARROW_R + " F" + to_string(f) + " " + "SINI";
        } else if (f < currentFloor) {
            floorColor = Color::DIM;
            floorLabel = floorColor + "   F" + to_string(f) + " " + Color::BRIGHT_GREEN + "Cleared";
        } else {
            floorColor = Color::BRIGHT_BLACK;
            floorLabel = floorColor + "   F" + to_string(f) + " ???";
        }
        floorLabel += r;

        cout << "  " << bc << Box::V << r << padRight(floorLabel, w - 3)
             << bc << Box::V << r << endl;
    }

    cout << "  " << bc << Box::BL;
    for (int i = 0; i < w - 2; i++) cout << Box::H;
    cout << Box::BR << r << endl;
}

