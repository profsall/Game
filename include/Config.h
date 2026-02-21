#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct GameConfig {
    int textSpeed;       // ms per char for typewriter (10-80)
    int difficulty;      // 0=easy, 1=normal, 2=hard
    bool soundEnabled;   // Beep() effects
};

// Global config
extern GameConfig gameConfig;

// Functions
void initConfig();
void saveConfig();
void loadConfig();
void showConfigMenu();

// Difficulty helpers
std::string getDifficultyName(int diff);
float getDifficultyMultiplier(int diff);    // enemy stat multiplier
float getDifficultyRewardMultiplier(int diff); // reward multiplier

#endif
