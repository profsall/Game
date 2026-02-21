#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>

// Item types
enum ItemType {
    ITEM_CONSUMABLE = 0,  // potions, food
    ITEM_ARMOR = 1,       // defense boost
    ITEM_ACCESSORY = 2,   // crit/dodge/stat boost
    ITEM_SCROLL = 3,      // single-use battle buff
    ITEM_LORE = 4         // collectible lore items
};

struct Item {
    int id;
    std::string name;
    std::string description;
    ItemType type;
    int value;         // effect amount
    int price;
    int count;         // how many owned (stackable for consumables)
    bool equipped;     // for armor/accessory
};

// Constants
const int MAX_INVENTORY = 20;
const int MAX_LORE = 10;

// Global inventory
extern Item inventory[MAX_INVENTORY];
extern int inventoryCount;

// Lore collection (separate)
extern Item loreItems[MAX_LORE];
extern int loreCount;

// Inventory functions
void initInventory();
bool addItem(int itemId, int count = 1);
bool removeItem(int itemId, int count = 1);
bool hasItem(int itemId);
int getItemCount(int itemId);
Item getItemTemplate(int itemId);

// Equipment
void equipArmor(int itemId);
void equipAccessory(int itemId);
void unequipArmor();
void unequipAccessory();
int getEquippedArmorId();
int getEquippedAccessoryId();
int getArmorDefenseBonus();
int getAccessoryBonus(int statType); // 0=crit, 1=dodge, 2=atk, 3=def

// Lore
void addLoreItem(int loreId);
bool hasLoreItem(int loreId);

// UI
void showInventoryMenu(struct Player &hero);
void showLoreCollection();

// Item database (all possible items)
const int TOTAL_ITEMS = 20;
const int TOTAL_LORE_ITEMS = 8;

#endif
