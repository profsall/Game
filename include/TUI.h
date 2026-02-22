#ifndef TUI_H
#define TUI_H

#include <string>
#include <vector>

using std::string;
using std::vector;

// ===========================================
//  ANSI Color Codes
// ===========================================

namespace Color {
    const string RESET   = "";
    const string BOLD    = "";
    const string DIM     = "";
    const string ITALIC  = "";
    const string ULINE   = "";
    const string BLINK   = "";
    const string REVERSE = "";

    const string BLACK   = "";
    const string RED     = "";
    const string GREEN   = "";
    const string YELLOW  = "";
    const string BLUE    = "";
    const string MAGENTA = "";
    const string CYAN    = "";
    const string WHITE   = "";

    const string BRIGHT_BLACK   = "";
    const string BRIGHT_RED     = "";
    const string BRIGHT_GREEN   = "";
    const string BRIGHT_YELLOW  = "";
    const string BRIGHT_BLUE    = "";
    const string BRIGHT_MAGENTA = "";
    const string BRIGHT_CYAN    = "";
    const string BRIGHT_WHITE   = "";

    const string BG_BLACK   = "";
    const string BG_RED     = "";
    const string BG_GREEN   = "";
    const string BG_YELLOW  = "";
    const string BG_BLUE    = "";
    const string BG_MAGENTA = "";
    const string BG_CYAN    = "";
    const string BG_WHITE   = "";

    const string BG_BRIGHT_BLACK = "";
}

// ===========================================
//  Box-Drawing Characters (plain ASCII)
// ===========================================

namespace Box {
    // Double-line box (ASCII)
    const string TL = "+";
    const string TR = "+";
    const string BL = "+";
    const string BR = "+";
    const string H  = "-";
    const string V  = "|";
    const string LT = "+";
    const string RT = "+";
    const string TT = "+";
    const string BT = "+";
    const string CR = "+";

    // Single-line box (ASCII)
    const string STL = "+";
    const string STR = "+";
    const string SBL = "+";
    const string SBR = "+";
    const string SH  = "-";
    const string SV  = "|";
    const string SLT = "+";
    const string SRT = "+";

    // Special (all empty - no icons)
    const string BLOCK_FULL  = "#";
    const string BLOCK_LIGHT = ".";
    const string BLOCK_MED   = ":";
    const string BLOCK_DARK  = "#";
    const string DIAMOND     = "";
    const string ARROW_R     = ">";
    const string ARROW_L     = "<";
    const string STAR        = "";
    const string HEART       = "";
    const string SWORD       = "";
    const string SHIELD      = "";
    const string SKULL       = "";
    const string SPARKLE     = "";
}

// ===========================================
//  TUI Functions
// ===========================================

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

