#ifndef SKILLS_H
#define SKILLS_H

#include <string>

enum SkillType {
    SKILL_ATTACK = 0,   // damage skill
    SKILL_HEAL = 1,     // heal HP
    SKILL_BUFF = 2      // temporary stat boost
};

struct Skill {
    int id;
    std::string name;
    std::string description;
    SkillType type;
    int manaCost;
    int power;           // damage, heal amount, or buff %
    int requiredLevel;
};

// Constants
const int MAX_SKILLS = 8;
const int TOTAL_SKILLS = 10;

// Global skill data
extern Skill playerSkills[MAX_SKILLS];
extern int playerSkillCount;

// Functions
void initSkills();
void checkNewSkills(int playerLevel); // learn skills on level up
Skill getSkillTemplate(int skillId);
int useAttackSkill(int skillId, int userAtk, int targetDef);
int useHealSkill(int skillId);

// UI
void showSkillsMenu();
int showBattleSkills(int playerMp); // returns selected skill ID or -1

#endif
