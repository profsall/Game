#ifndef TUI_H
#define TUI_H

#include <string>
#include <vector>

using std::string;
using std::vector;

// ═══════════════════════════════════════════
//  ANSI Color Codes
// ═══════════════════════════════════════════

namespace Color {
    // Reset
    const string RESET   = "\033[0m";
    const string BOLD    = "\033[1m";
    const string DIM     = "\033[2m";
    const string ITALIC  = "\033[3m";
    const string ULINE   = "\033[4m";
    const string BLINK   = "\033[5m";
    const string REVERSE = "\033[7m";

    // Foreground
    const string BLACK   = "\033[30m";
    const string RED     = "\033[31m";
    const string GREEN   = "\033[32m";
    const string YELLOW  = "\033[33m";
    const string BLUE    = "\033[34m";
    const string MAGENTA = "\033[35m";
    const string CYAN    = "\033[36m";
    const string WHITE   = "\033[37m";

    // Bright Foreground
    const string BRIGHT_BLACK   = "\033[90m";
    const string BRIGHT_RED     = "\033[91m";
    const string BRIGHT_GREEN   = "\033[92m";
    const string BRIGHT_YELLOW  = "\033[93m";
    const string BRIGHT_BLUE    = "\033[94m";
    const string BRIGHT_MAGENTA = "\033[95m";
    const string BRIGHT_CYAN    = "\033[96m";
    const string BRIGHT_WHITE   = "\033[97m";

    // Background
    const string BG_BLACK   = "\033[40m";
    const string BG_RED     = "\033[41m";
    const string BG_GREEN   = "\033[42m";
    const string BG_YELLOW  = "\033[43m";
    const string BG_BLUE    = "\033[44m";
    const string BG_MAGENTA = "\033[45m";
    const string BG_CYAN    = "\033[46m";
    const string BG_WHITE   = "\033[47m";

    // Bright Background
    const string BG_BRIGHT_BLACK = "\033[100m";
}

// ═══════════════════════════════════════════
//  Unicode Box-Drawing Characters
// ═══════════════════════════════════════════

namespace Box {
    // Double-line box
    const string TL = "\u2554"; // ╔
    const string TR = "\u2557"; // ╗
    const string BL = "\u255A"; // ╚
    const string BR = "\u255D"; // ╝
    const string H  = "\u2550"; // ═
    const string V  = "\u2551"; // ║
    const string LT = "\u2560"; // ╠
    const string RT = "\u2563"; // ╣
    const string TT = "\u2566"; // ╦
    const string BT = "\u2569"; // ╩
    const string CR = "\u256C"; // ╬

    // Single-line box
    const string STL = "\u250C"; // ┌
    const string STR = "\u2510"; // ┐
    const string SBL = "\u2514"; // └
    const string SBR = "\u2518"; // ┘
    const string SH  = "\u2500"; // ─
    const string SV  = "\u2502"; // │
    const string SLT = "\u251C"; // ├
    const string SRT = "\u2524"; // ┤

    // Special
    const string BLOCK_FULL  = "\u2588"; // █
    const string BLOCK_LIGHT = "\u2591"; // ░
    const string BLOCK_MED   = "\u2592"; // ▒
    const string BLOCK_DARK  = "\u2593"; // ▓
    const string DIAMOND     = "\u25C6"; // ◆
    const string ARROW_R     = "\u25B6"; // ▶
    const string ARROW_L     = "\u25C0"; // ◀
    const string STAR        = "\u2605"; // ★
    const string HEART       = "\u2665"; // ♥
    const string SWORD       = "\u2694"; // ⚔
    const string SHIELD      = "\u2764"; // ❤ (used as shield icon)
    const string SKULL       = "\u2620"; // ☠
    const string SPARKLE     = "\u2728"; // ✨
}

// ═══════════════════════════════════════════
//  TUI Functions
// ═══════════════════════════════════════════

// Setup & Basics
void initTUI();
void clearScreen();
void hideCursor();
void showCursor();
void moveCursor(int row, int col);
void sleepMs(int ms);

// Text helpers
string centerText(const string& text, int width);
string padRight(const string& text, int width);
int visibleLength(const string& text);

// Rendering
void drawDoubleBox(int width, const string& title = "", const string& borderColor = Color::CYAN);
void drawDoubleBoxEnd(int width, const string& borderColor = Color::CYAN);
void drawBoxLine(int width, const string& content, const string& borderColor = Color::CYAN);
void drawSeparator(int width, const string& borderColor = Color::CYAN);
void drawEmptyLine(int width, const string& borderColor = Color::CYAN);

// Panel — draws a complete box with multiple lines
void drawPanel(const string& title, const vector<string>& lines, int width, const string& borderColor = Color::CYAN);

// Animated text
void typewriter(const string& text, int delayMs = 30, const string& color = Color::WHITE);
void typewriterLine(const string& text, int delayMs = 30, const string& color = Color::WHITE);
void dramaticPause(int ms = 800);

// Health & Progress bars
void drawHealthBar(const string& label, int current, int max, int barWidth = 20);
void drawExpBar(const string& label, int current, int max, int barWidth = 20);
void drawManaBar(const string& label, int current, int max, int barWidth = 20);

// Menu system
int drawMenu(const string& title, const vector<string>& items, const string& borderColor = Color::CYAN, int width = 50);

// Prompt
void pausePrompt(const string& msg = "Tekan ENTER untuk melanjutkan...");
int getChoice(int min, int max);

// Fancy displays
void drawTitleArt();
void drawBattleHeader();
void drawVictoryBanner();
void drawDefeatBanner();
void drawLevelUpBanner(int newLevel);
void drawChapterTitle(int chapter, const string& title, const string& subtitle);

// Sound effects (uses Windows Beep)
void sfxAttack();
void sfxCritical();
void sfxDodge();
void sfxHeal();
void sfxLevelUp();
void sfxVictory();
void sfxDefeat();
void sfxItem();
void sfxMenuSelect();

// Battle animations
void animateAttack(const string& attackerName, const string& color);
void animateCritical();
void animateDodge();
void animateSkillUse(const string& skillName, const string& color);

// Minimap
void drawTowerMinimap(int currentFloor, int totalFloors, const string& towerName);

#endif

