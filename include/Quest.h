#ifndef QUEST_H
#define QUEST_H

#include <string>

struct Quest {
    int id;
    std::string name;
    std::string description;
    std::string giver;        // NPC name
    std::string targetMonster; // monster name to kill (empty = any)
    int targetCount;
    int currentCount;
    int rewardExp;
    int rewardCoin;
    int rewardItemId;         // -1 = no item reward
    bool active;
    bool completed;
    bool claimed;             // reward claimed?
};

const int MAX_QUESTS = 6;
const int TOTAL_QUESTS = 10;

extern Quest activeQuests[MAX_QUESTS];
extern int activeQuestCount;

// Functions
void initQuests();
Quest getQuestTemplate(int questId);
bool acceptQuest(int questId);
void updateQuestProgress(const std::string& monsterName);
void checkQuestCompletion();
bool claimQuestReward(int questIndex, struct Player &hero);
int getAvailableQuestCount(int chapter);

// UI
void showQuestBoard(struct Player &hero);
void showActiveQuests();

#endif
