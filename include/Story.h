#ifndef STORY_H
#define STORY_H

#include <string>
#include <vector>

using std::string;
using std::vector;

struct DialogLine {
    string speaker;
    string text;
    string color;
};

struct Player;

// Dialog engine
void showDialog(const vector<DialogLine>& lines);
void showNarration(const vector<string>& lines, int delayMs = 35);
int showDialogChoice(const string& prompt, const vector<string>& choices);

// Story beats
void showPrologue();
void showChapterIntro(int chapter);
void showChapterOutro(int chapter);

// Dialog choices (affects gameplay)
void showChapterIntroWithChoices(int chapter, Player &hero);

// Boss pre-battle dialog
void showBossDialog(int towerNumber);

// Companion NPC
void companionEncounter(int chapter, Player &hero);

// Mid-tower story beats
void showMidTowerBeat(int towerNumber, int floor);

// Flashback / Memory fragments
void showFlashback();

// Endings
void showEnding(Player &hero);

#endif
