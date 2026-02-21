#include "Inventory.h"
#include "Entity.h"
#include "TUI.h"
#include <iostream>
using namespace std;

// ═══════════════════════════════════════════
//  Global State
// ═══════════════════════════════════════════

Item inventory[MAX_INVENTORY];
int inventoryCount = 0;

Item loreItems[MAX_LORE];
int loreCount = 0;

static int equippedArmorId = -1;
static int equippedAccessoryId = -1;

// ═══════════════════════════════════════════
//  Item Database
// ═══════════════════════════════════════════

static Item itemDB[] = {
    // id, name, description, type, value, price, count, equipped
    {0,  "HP Potion",        "Restorasi 50 HP",              ITEM_CONSUMABLE, 50,   30,  0, false},
    {1,  "HP Potion+",       "Restorasi 120 HP",             ITEM_CONSUMABLE, 120,  80,  0, false},
    {2,  "MP Potion",        "Restorasi 30 MP",              ITEM_CONSUMABLE, 30,   40,  0, false},
    {3,  "Elixir",           "Full HP + MP",                 ITEM_CONSUMABLE, 999,  200, 0, false},
    {4,  "Leather Armor",    "Defense +5",                   ITEM_ARMOR,      5,    100, 0, false},
    {5,  "Chain Mail",       "Defense +12",                  ITEM_ARMOR,      12,   300, 0, false},
    {6,  "Plate Armor",      "Defense +22",                  ITEM_ARMOR,      22,   700, 0, false},
    {7,  "Dragon Scale",     "Defense +35",                  ITEM_ARMOR,      35,   1500,0, false},
    {8,  "Lucky Charm",      "Critical +10%",                ITEM_ACCESSORY,  10,   150, 0, false},
    {9,  "Shadow Cloak",     "Dodge +12%",                   ITEM_ACCESSORY,  12,   200, 0, false},
    {10, "Power Ring",       "Attack +8",                    ITEM_ACCESSORY,  8,    250, 0, false},
    {11, "Guardian Amulet",  "Defense +10",                  ITEM_ACCESSORY,  10,   300, 0, false},
    {12, "Scroll of Power",  "ATK +50% untuk 1 battle",      ITEM_SCROLL,     50,   60,  0, false},
    {13, "Scroll of Shield", "DEF +50% untuk 1 battle",      ITEM_SCROLL,     50,   60,  0, false},
    {14, "Scroll of Haste",  "2x serangan untuk 1 turn",     ITEM_SCROLL,     2,    80,  0, false},
    {15, "Antidote",         "Sembuhkan poison",              ITEM_CONSUMABLE, 0,    25,  0, false},
    {16, "Bomb",             "Damage 80 ke musuh",            ITEM_CONSUMABLE, 80,   50,  0, false},
    {17, "Smoke Bomb",       "Kabur dari battle",             ITEM_CONSUMABLE, 0,    40,  0, false},
    {18, "Golden Apple",     "Max HP +20 permanen",           ITEM_CONSUMABLE, 20,   500, 0, false},
    {19, "Mana Crystal",     "Max MP +10 permanen",           ITEM_CONSUMABLE, 10,   400, 0, false},
};

// Lore item database
static Item loreDB[] = {
    {0, "Diary Kesatria Kuno",     "Catatan tentang sejarah menara...",  ITEM_LORE, 0, 0, 0, false},
    {1, "Peta Kuno Arcandia",      "Menunjukkan lokasi rahasia...",     ITEM_LORE, 0, 0, 0, false},
    {2, "Kristal Memori",          "Memori tentang raja terakhir...",   ITEM_LORE, 0, 0, 0, false},
    {3, "Gulungan Ramalan",        "Ramalan tentang sang penyelamat..", ITEM_LORE, 0, 0, 0, false},
    {4, "Lencana Kerajaan",        "Simbol kerajaan yang hilang...",    ITEM_LORE, 0, 0, 0, false},
    {5, "Surat Terakhir",          "Pesan dari kesatria yang gugur..",  ITEM_LORE, 0, 0, 0, false},
    {6, "Batu Rune Kuno",          "Huruf kuno yang menyimpan kekuatan..", ITEM_LORE, 0, 0, 0, false},
    {7, "Liontin Dark Lord",       "Sebuah liontin hitam berukir...",   ITEM_LORE, 0, 0, 0, false},
};

// ═══════════════════════════════════════════
//  Functions
// ═══════════════════════════════════════════

void initInventory() {
    inventoryCount = 0;
    loreCount = 0;
    equippedArmorId = -1;
    equippedAccessoryId = -1;
    for (int i = 0; i < MAX_INVENTORY; i++) inventory[i] = {-1, "", "", ITEM_CONSUMABLE, 0, 0, 0, false};
    for (int i = 0; i < MAX_LORE; i++) loreItems[i] = {-1, "", "", ITEM_LORE, 0, 0, 0, false};
}

Item getItemTemplate(int itemId) {
    if (itemId >= 0 && itemId < TOTAL_ITEMS) return itemDB[itemId];
    return {-1, "Unknown", "", ITEM_CONSUMABLE, 0, 0, 0, false};
}

bool addItem(int itemId, int count) {
    // Check if already in inventory (stack)
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i].id == itemId) {
            inventory[i].count += count;
            return true;
        }
    }
    // Add new slot
    if (inventoryCount >= MAX_INVENTORY) return false;
    inventory[inventoryCount] = getItemTemplate(itemId);
    inventory[inventoryCount].count = count;
    inventoryCount++;
    return true;
}

bool removeItem(int itemId, int count) {
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i].id == itemId) {
            inventory[i].count -= count;
            if (inventory[i].count <= 0) {
                // Remove slot by shifting
                for (int j = i; j < inventoryCount - 1; j++)
                    inventory[j] = inventory[j+1];
                inventoryCount--;
            }
            return true;
        }
    }
    return false;
}

bool hasItem(int itemId) {
    for (int i = 0; i < inventoryCount; i++)
        if (inventory[i].id == itemId && inventory[i].count > 0) return true;
    return false;
}

int getItemCount(int itemId) {
    for (int i = 0; i < inventoryCount; i++)
        if (inventory[i].id == itemId) return inventory[i].count;
    return 0;
}

void equipArmor(int itemId) {
    equippedArmorId = itemId;
}

void equipAccessory(int itemId) {
    equippedAccessoryId = itemId;
}

void unequipArmor() { equippedArmorId = -1; }
void unequipAccessory() { equippedAccessoryId = -1; }
int getEquippedArmorId() { return equippedArmorId; }
int getEquippedAccessoryId() { return equippedAccessoryId; }

int getArmorDefenseBonus() {
    if (equippedArmorId < 0) return 0;
    return getItemTemplate(equippedArmorId).value;
}

int getAccessoryBonus(int statType) {
    if (equippedAccessoryId < 0) return 0;
    Item acc = getItemTemplate(equippedAccessoryId);
    // statType: 0=crit, 1=dodge, 2=atk, 3=def
    switch (equippedAccessoryId) {
        case 8: return (statType == 0) ? acc.value : 0;  // Lucky Charm = crit
        case 9: return (statType == 1) ? acc.value : 0;  // Shadow Cloak = dodge
        case 10: return (statType == 2) ? acc.value : 0; // Power Ring = atk
        case 11: return (statType == 3) ? acc.value : 0; // Guardian Amulet = def
        default: return 0;
    }
}

void addLoreItem(int loreId) {
    if (loreId < 0 || loreId >= TOTAL_LORE_ITEMS) return;
    for (int i = 0; i < loreCount; i++)
        if (loreItems[i].id == loreId) return; // already have
    if (loreCount >= MAX_LORE) return;
    loreItems[loreCount] = loreDB[loreId];
    loreItems[loreCount].count = 1;
    loreCount++;
}

bool hasLoreItem(int loreId) {
    for (int i = 0; i < loreCount; i++)
        if (loreItems[i].id == loreId) return true;
    return false;
}

// ═══════════════════════════════════════════
//  Inventory UI
// ═══════════════════════════════════════════

void showInventoryMenu(Player &hero) {
    while (true) {
        clearScreen();
        cout << endl;

        int w = 56;
        drawDoubleBox(w, Box::STAR + " INVENTORY " + Box::STAR, Color::BRIGHT_YELLOW);

        // Equipped items
        string armorStr = (equippedArmorId >= 0) ? getItemTemplate(equippedArmorId).name : "None";
        string accStr = (equippedAccessoryId >= 0) ? getItemTemplate(equippedAccessoryId).name : "None";
        drawBoxLine(w, Color::BRIGHT_BLUE + " Armor     : " + Color::WHITE + armorStr + Color::DIM + " (+" + to_string(getArmorDefenseBonus()) + " DEF)" + Color::RESET, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::BRIGHT_MAGENTA + " Accessory : " + Color::WHITE + accStr + Color::RESET, Color::BRIGHT_YELLOW);

        drawSeparator(w, Color::BRIGHT_YELLOW);

        if (inventoryCount == 0) {
            drawBoxLine(w, Color::DIM + " Inventory kosong." + Color::RESET, Color::BRIGHT_YELLOW);
        } else {
            for (int i = 0; i < inventoryCount; i++) {
                Item &it = inventory[i];
                string typeColor;
                switch (it.type) {
                    case ITEM_CONSUMABLE: typeColor = Color::BRIGHT_GREEN; break;
                    case ITEM_ARMOR: typeColor = Color::BRIGHT_BLUE; break;
                    case ITEM_ACCESSORY: typeColor = Color::BRIGHT_MAGENTA; break;
                    case ITEM_SCROLL: typeColor = Color::BRIGHT_CYAN; break;
                    default: typeColor = Color::WHITE;
                }
                string equipped = "";
                if (it.type == ITEM_ARMOR && equippedArmorId == it.id)
                    equipped = Color::BRIGHT_GREEN + " [E]" + Color::RESET;
                if (it.type == ITEM_ACCESSORY && equippedAccessoryId == it.id)
                    equipped = Color::BRIGHT_GREEN + " [E]" + Color::RESET;

                string line = Color::BRIGHT_WHITE + " [" + to_string(i+1) + "] " + typeColor
                    + padRight(it.name, 20) + Color::DIM + " x" + to_string(it.count) + equipped + Color::RESET;
                drawBoxLine(w, line, Color::BRIGHT_YELLOW);
            }
        }

        drawSeparator(w, Color::BRIGHT_YELLOW);
        drawBoxLine(w, Color::DIM + " [0] Kembali" + Color::RESET, Color::BRIGHT_YELLOW);
        drawDoubleBoxEnd(w, Color::BRIGHT_YELLOW);

        int choice = getChoice(0, inventoryCount);

        if (choice == 0) return;

        // Item selected - show details
        int idx = choice - 1;
        Item &sel = inventory[idx];

        clearScreen();
        cout << endl;
        drawDoubleBox(w, sel.name, Color::BRIGHT_CYAN);
        drawBoxLine(w, Color::DIM + " " + sel.description + Color::RESET, Color::BRIGHT_CYAN);
        drawSeparator(w, Color::BRIGHT_CYAN);

        if (sel.type == ITEM_ARMOR) {
            drawBoxLine(w, Color::BRIGHT_WHITE + " [1] Equip  [2] Unequip  [0] Batal" + Color::RESET, Color::BRIGHT_CYAN);
            drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);
            int act = getChoice(0, 2);
            if (act == 1) equipArmor(sel.id);
            else if (act == 2 && equippedArmorId == sel.id) unequipArmor();
        } else if (sel.type == ITEM_ACCESSORY) {
            drawBoxLine(w, Color::BRIGHT_WHITE + " [1] Equip  [2] Unequip  [0] Batal" + Color::RESET, Color::BRIGHT_CYAN);
            drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);
            int act = getChoice(0, 2);
            if (act == 1) equipAccessory(sel.id);
            else if (act == 2 && equippedAccessoryId == sel.id) unequipAccessory();
        } else if (sel.type == ITEM_CONSUMABLE && sel.id == 0) {
            // HP Potion
            drawBoxLine(w, Color::BRIGHT_WHITE + " [1] Gunakan  [0] Batal" + Color::RESET, Color::BRIGHT_CYAN);
            drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);
            int act = getChoice(0, 1);
            if (act == 1) {
                hero.hp += sel.value;
                if (hero.hp > hero.maxHp) hero.hp = hero.maxHp;
                removeItem(sel.id, 1);
                cout << "  " << Color::BRIGHT_GREEN << "HP +" << sel.value << "!" << Color::RESET << endl;
                sleepMs(800);
            }
        } else if (sel.type == ITEM_CONSUMABLE && sel.id == 1) {
            drawBoxLine(w, Color::BRIGHT_WHITE + " [1] Gunakan  [0] Batal" + Color::RESET, Color::BRIGHT_CYAN);
            drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);
            int act = getChoice(0, 1);
            if (act == 1) {
                hero.hp += sel.value;
                if (hero.hp > hero.maxHp) hero.hp = hero.maxHp;
                removeItem(sel.id, 1);
                cout << "  " << Color::BRIGHT_GREEN << "HP +" << sel.value << "!" << Color::RESET << endl;
                sleepMs(800);
            }
        } else if (sel.type == ITEM_CONSUMABLE && sel.id == 2) {
            drawBoxLine(w, Color::BRIGHT_WHITE + " [1] Gunakan  [0] Batal" + Color::RESET, Color::BRIGHT_CYAN);
            drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);
            int act = getChoice(0, 1);
            if (act == 1) {
                hero.mp += sel.value;
                if (hero.mp > hero.maxMp) hero.mp = hero.maxMp;
                removeItem(sel.id, 1);
                cout << "  " << Color::BRIGHT_BLUE << "MP +" << sel.value << "!" << Color::RESET << endl;
                sleepMs(800);
            }
        } else if (sel.type == ITEM_CONSUMABLE && sel.id == 3) {
            drawBoxLine(w, Color::BRIGHT_WHITE + " [1] Gunakan  [0] Batal" + Color::RESET, Color::BRIGHT_CYAN);
            drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);
            int act = getChoice(0, 1);
            if (act == 1) {
                hero.hp = hero.maxHp;
                hero.mp = hero.maxMp;
                removeItem(sel.id, 1);
                cout << "  " << Color::BRIGHT_YELLOW << "Full HP + MP!" << Color::RESET << endl;
                sleepMs(800);
            }
        } else {
            drawBoxLine(w, Color::DIM + " Tidak bisa digunakan di sini." + Color::RESET, Color::BRIGHT_CYAN);
            drawDoubleBoxEnd(w, Color::BRIGHT_CYAN);
            pausePrompt();
        }
    }
}

void showLoreCollection() {
    clearScreen();
    cout << endl;

    int w = 56;
    drawDoubleBox(w, Box::DIAMOND + " KOLEKSI LORE " + Box::DIAMOND, Color::BRIGHT_MAGENTA);

    if (loreCount == 0) {
        drawBoxLine(w, Color::DIM + " Belum ada lore yang ditemukan." + Color::RESET, Color::BRIGHT_MAGENTA);
        drawBoxLine(w, Color::DIM + " Eksplorasi tower untuk menemukan artefak!" + Color::RESET, Color::BRIGHT_MAGENTA);
    } else {
        drawBoxLine(w, Color::BRIGHT_WHITE + " Ditemukan: " + Color::BRIGHT_YELLOW + to_string(loreCount) + "/" + to_string(TOTAL_LORE_ITEMS) + Color::RESET, Color::BRIGHT_MAGENTA);
        drawSeparator(w, Color::BRIGHT_MAGENTA);
        for (int i = 0; i < loreCount; i++) {
            drawBoxLine(w, Color::BRIGHT_YELLOW + " " + Box::STAR + " " + Color::BRIGHT_WHITE + loreItems[i].name + Color::RESET, Color::BRIGHT_MAGENTA);
            drawBoxLine(w, Color::DIM + "   " + loreItems[i].description + Color::RESET, Color::BRIGHT_MAGENTA);
        }
    }

    drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);
    pausePrompt();
}
