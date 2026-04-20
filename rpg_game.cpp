#include <iostream>
#include <fstream>   
#include <cstdlib>   
#include <ctime>     
#include <string>   
using namespace std;

// ============================================================
//   KONSTANTA GLOBAL
// ============================================================
const int MAX_INVENTORY = 5;
const int MAX_ITEMS     = 10;  // Ditambah jadi 10 item
const int MAX_FLOORS    = 4;

// ============================================================
//   STRUCT ITEM
// ============================================================
struct Item {
    char name[30];
    int  price;
    int  atkBonus;
    int  hpBonus;
};

// ============================================================
//   DATABASE ITEM (Toko)
// ============================================================
Item shopItems[MAX_ITEMS] = {
    {"Pedang Kayu",     50,  5,  0},
    {"Perisai Kayu",    40,  0, 20},
    {"Pedang Besi",    120, 15,  0},
    {"Baju Besi",      100,  0, 40},
    {"Pedang Mistik",  300, 35,  0},
    {"Jubah Naga",     250,  0, 80},
    {"Kapak Orc",      180, 25,  0},
    {"Tameng Baja",    200,  0, 60},
    {"Pedang Cahaya",  500, 60,  0},
    {"Mahkota Raja",   600, 20, 150}
};

// ============================================================
//   STRUCT PLAYER
// ============================================================
struct Player {
    string name;
    int  hp;
    int  maxHp;
    int  attack;
    int  coin;
    int  exp;
    int  level;
    int  currentFloor;
    int  inventory[MAX_INVENTORY]; 
};

// ============================================================
//   FUNGSI UTILITAS
// ============================================================
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\n[Tekan Enter untuk melanjutkan...]";
    cin.ignore();
    cin.get();
}

int getTotalAtkBonus(const Player& p) {
    int total = 0; // Kesalahan konyol manéh (0;2;) geus dibenerkeun
    for (int i = 0; i < MAX_INVENTORY; i++) {
        if (p.inventory[i] != -1) {
            total += shopItems[p.inventory[i]].atkBonus;
        }
    }
    return total;
}

int getEffectiveAtk(const Player& p) {
    return p.attack + getTotalAtkBonus(p);
}

// ============================================================
//   ASCII ART
// ============================================================
void printTower() {
    cout << "        /\\\n";
    cout << "       /  \\\n";
    cout << "      / 4F \\\n";
    cout << "     /------\\\n";
    cout << "    /  3F    \\\n";
    cout << "   /----------\\\n";
    cout << "  /    2F      \\\n";
    cout << " /--------------\\\n";
    cout << "/      1F        \\\n";
    cout << "|================|\n";
    cout << "|   TOWER ASCII  |\n";
    cout << "|________________|\n";
}

void printPlayer() {
    cout << "   O   \n";
    cout << "  /|\\ \n";
    cout << "  / \\ \n";
}

void printMonster() {
    cout << "   (\\__/)  \n";
    cout << "   (o .o)  \n";
    cout << "   (>  <)  \n";
}

void printBoss() {
    cout << "    \\|/    \n";
    cout << "   (@_@)   \n";
    cout << "  /| V |\\  \n";
    cout << "   |   |   \n";
    cout << "  /     \\  \n";
}

void printBanner() {
    cout << "        ASCII RPG TOWER GAME          \n";
}

// ============================================================
//   INISIALISASI & LEVEL UP
// ============================================================
void initPlayer(Player& p, const string& playerName) {
    p.name = playerName;
    p.hp = 100;
    p.maxHp = 100;
    p.attack = 10;
    p.coin = 100;
    p.exp = 0;
    p.level = 1;
    p.currentFloor = 1;

    for (int i = 0; i < MAX_INVENTORY; i++) {
        p.inventory[i] = -1;
    }
}

void checkLevelUp(Player& p) {
    int threshold = p.level * 50;
    while (p.exp >= threshold) {
        p.exp    -= threshold;
        p.level  += 1;
        p.attack += 5;
        p.maxHp  += 20;
        p.hp      = p.maxHp;

        cout << "\n*** LEVEL UP! ***\n";
        cout << "Selamat " << p.name << "! Kamu naik ke Level " << p.level << "!\n";
        cout << "ATK +5 | Max HP +20 | HP pulih penuh!\n";
        threshold = p.level * 50; 
    }
}

// ============================================================
//   TAMPILKAN STATUS
// ============================================================
void showStatus(const Player& p) {
    clearScreen();
    printBanner();
    cout << "\n--- STATUS PLAYER ---\n";
    printPlayer();
    cout << "Nama       : " << p.name << "\n";
    cout << "Level      : " << p.level << "\n";
    cout << "HP         : " << p.hp << " / " << p.maxHp << "\n";
    cout << "ATK        : " << p.attack << " (+" << getTotalAtkBonus(p) << " dari item) = " << getEffectiveAtk(p) << "\n";
    cout << "EXP        : " << p.exp << " / " << (p.level * 50) << "\n";
    cout << "Coin       : " << p.coin << "\n";
    cout << "Lantai     : " << p.currentFloor << " / " << MAX_FLOORS << "\n";

    cout << "\n--- INVENTORY (" << MAX_INVENTORY << " slot) ---\n";
    for (int i = 0; i < MAX_INVENTORY; i++) {
        cout << "[Slot " << (i + 1) << "] ";
        if (p.inventory[i] == -1) {
            cout << "(kosong)\n";
        } else {
            cout << shopItems[p.inventory[i]].name << " | ATK+" << shopItems[p.inventory[i]].atkBonus << " | HP+" << shopItems[p.inventory[i]].hpBonus << "\n";
        }
    }
}

// ============================================================
//   SISTEM BATTLE TERPUSAT
//   Ngatur pertarungan jeung monster atawa bos
// ============================================================
bool startBattle(Player& p, bool isBoss, int floor) {
    clearScreen();
    
    int enemyHp, enemyAtk;
    string enemyName;

    if (isBoss) {
        cout << "=== PERTARUNGAN MELAWAN BOS LANTAI " << floor << " ===\n\n";
        printBoss();
        enemyName = "GUARDIAN BOS";
        enemyHp  = 100 + (floor * 50) + rand() % 30;
        enemyAtk = 15 + (floor * 8) + rand() % 5;
    } else {
        cout << "=== PERTARUNGAN MONSTER ===\n\n";
        printMonster();
        enemyName = "Monster Liar";
        enemyHp  = 30 + (floor * 20) + rand() % 20;
        enemyAtk = 5  + (floor * 3)  + rand() % 5;
    }

    cout << "\n" << enemyName << " menghadangmu!\n";
    cout << "HP  : " << enemyHp  << "\n";
    cout << "ATK : " << enemyAtk << "\n";
    cout << "\n1. Serang!\n2. Kabur\n> ";

    int choice;
    cin >> choice;

    if (choice == 2) {
        if (isBoss) {
            cout << "Kamu tidak bisa lari dari pertarungan bos!\n";
            pause();
        } else {
            cout << "Kamu berhasil kabur!\n";
            pause();
            return false;
        }
    }

    int playerHp = p.hp;
    cout << "\n--- MULAI PERTARUNGAN ---\n";
    
    while (playerHp > 0 && enemyHp > 0) {
        int dmgToEnemy = getEffectiveAtk(p) + rand() % 5;
        enemyHp -= dmgToEnemy;
        cout << "Kamu menyerang! Damage: " << dmgToEnemy;

        if (enemyHp <= 0) {
            cout << " -> " << enemyName << " KALAH!\n";
            break;
        }

        int dmgToPlayer = enemyAtk + rand() % 3;
        playerHp -= dmgToPlayer;
        cout << " | " << enemyName << " membalas! Kena " << dmgToPlayer << " damage. HP: " << playerHp << "\n";
    }

    p.hp = (playerHp > 0) ? playerHp : 0;

    if (playerHp > 0) {
        int expGain  = (isBoss ? 50 : 20) + (floor * 10) + rand() % 15;
        int coinGain = (isBoss ? 100 : 10) + (floor * 5)  + rand() % 10;

        p.exp  += expGain;
        p.coin += coinGain;

        cout << "\n[MENANG!] EXP +" << expGain << " | Coin +" << coinGain << "\n";
        checkLevelUp(p); 
        return true;
    } else {
        p.hp = 1;
        cout << "\n[KALAH!] Kamu dihajar habis-habisan! HP tersisa: 1\n";
        cout << "Segera beli ramuan di toko!\n";
        return false;
    }
}

// ============================================================
//   SISTEM GRINDING
// ============================================================
void grindMenu(Player& p) {
    startBattle(p, false, p.currentFloor);
}

// ============================================================
//   SISTEM SHOP
// ============================================================
void shopMenu(Player& p) {
    int choice;
    do {
        clearScreen();
        cout << "=== TOKO ITEM ===\n";
        cout << "Coin kamu: " << p.coin << "\n\n";
        cout << "--- DAFTAR ITEM ---\n";

        for (int i = 0; i < MAX_ITEMS; i++) {
            cout << (i + 1) << ". " << shopItems[i].name
                 << " | Harga: " << shopItems[i].price
                 << " | ATK+" << shopItems[i].atkBonus
                 << " | HP+" << shopItems[i].hpBonus << "\n";
        }

        cout << "\n11. Jual Item\n12. Beli Ramuan HP (+30 HP, 20 coin)\n0. Kembali\n> ";
        cin >> choice;

        if (choice >= 1 && choice <= MAX_ITEMS) {
            int idx = choice - 1; 

            if (p.coin < shopItems[idx].price) {
                cout << "Coin tidak cukup!\n";
                pause();
                continue;
            }

            int emptySlot = -1;
            for (int i = 0; i < MAX_INVENTORY; i++) {
                if (p.inventory[i] == -1) {
                    emptySlot = i; 
                    break;         
                }
            }

            if (emptySlot == -1) {
                cout << "Inventory penuh! Jual item dulu.\n";
                pause();
                continue;
            }

            p.coin -= shopItems[idx].price;
            p.inventory[emptySlot] = idx; 

            p.maxHp += shopItems[idx].hpBonus;
            p.hp    += shopItems[idx].hpBonus;

            cout << shopItems[idx].name << " berhasil dibeli dan masuk ke slot " << (emptySlot + 1) << "!\n";
            pause();

        } else if (choice == 11) {
            cout << "\n--- INVENTORY KAMU ---\n";

            bool hasItem = false;
            for (int i = 0; i < MAX_INVENTORY; i++) {
                if (p.inventory[i] != -1) {
                    hasItem = true;
                    cout << (i + 1) << ". [Slot " << (i+1) << "] "
                         << shopItems[p.inventory[i]].name
                         << " (jual: " << shopItems[p.inventory[i]].price / 2 << " coin)\n";
                }
            }

            if (!hasItem) {
                cout << "Inventory kosong!\n";
                pause();
                continue;
            }

            cout << "0. Batal\nPilih slot yang dijual: ";
            int sellSlot;
            cin >> sellSlot;

            if (sellSlot >= 1 && sellSlot <= MAX_INVENTORY) {
                int slotIdx = sellSlot - 1;
                if (p.inventory[slotIdx] == -1) {
                    cout << "Slot kosong!\n";
                } else {
                    int itemIdx   = p.inventory[slotIdx];
                    int sellPrice = shopItems[itemIdx].price / 2;

                    p.maxHp -= shopItems[itemIdx].hpBonus;
                    if (p.hp > p.maxHp) p.hp = p.maxHp; 

                    p.coin += sellPrice;
                    p.inventory[slotIdx] = -1; 

                    cout << shopItems[itemIdx].name << " dijual seharga " << sellPrice << " coin!\n";
                }
                pause();
            }

        } else if (choice == 12) {
            if (p.coin < 20) {
                cout << "Coin tidak cukup! (butuh 20 coin)\n";
            } else {
                p.coin -= 20;
                int heal = 30;
                p.hp = (p.hp + heal > p.maxHp) ? p.maxHp : p.hp + heal;
                cout << "HP pulih +30! HP sekarang: " << p.hp << "\n";
            }
            pause();
        }

    } while (choice != 0);
}

// ============================================================
//   SISTEM TOWER
// ============================================================
void towerMenu(Player& p) {
    int syaratLevel[MAX_FLOORS - 1] = {3, 6, 10};

    clearScreen();
    cout << "=== TOWER MENU ===\n\n";
    printTower();
    cout << "\nKamu sekarang di lantai: " << p.currentFloor << "\n";
    cout << "Level kamu            : " << p.level << "\n\n";

    cout << "--- SYARAT NAIK LANTAI ---\n";
    for (int i = 0; i < MAX_FLOORS - 1; i++) {
        cout << "Lantai " << (i + 2) << " : Level >= " << syaratLevel[i];
        if (p.level >= syaratLevel[i]) cout << " [TERPENUHI]";
        else cout << " [BELUM]";
        cout << "\n";
    }

    cout << "\n1. Naik Lantai (Lawan Bos)\n2. Turun Lantai\n0. Kembali\n> ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        if (p.currentFloor >= MAX_FLOORS) {
            cout << "Kamu sudah di lantai tertinggi!\n";
            pause();
        } else {
            int nextFloor   = p.currentFloor; 
            int levelNeeded = syaratLevel[nextFloor - 1];

            if (p.level >= levelNeeded) {
                cout << "\nBOS LANTAI MENGHALANGI JALANMU!\n";
                pause();
                
                // Manggil fungsi battle khusus boss
                bool win = startBattle(p, true, p.currentFloor);
                
                if (win) {
                    p.currentFloor++;
                    cout << "\nBOS DIKALAHKAN! Kamu berhasil naik ke lantai " << p.currentFloor << "!\n";
                } else {
                    cout << "\nKamu gagal mengalahkan bos. Latihan lagi sana!\n";
                }
                pause();
            } else {
                cout << "Level belum cukup! Butuh Level " << levelNeeded << ".\n";
                pause();
            }
        }
    } else if (choice == 2) {
        if (p.currentFloor <= 1) {
            cout << "Sudah di lantai dasar!\n";
        } else {
            p.currentFloor--;
            cout << "Turun ke lantai " << p.currentFloor << ".\n";
        }
        pause();
    }
}

// ============================================================
//   SAVE & LOAD
// ============================================================
void saveGame(const Player& p) {
    ofstream file("savegame.txt"); 
    if (!file.is_open()) {
        cout << "Gagal menyimpan file!\n";
        pause();
        return;
    }
    file << p.name         << "\n";
    file << p.hp           << "\n";
    file << p.maxHp        << "\n";
    file << p.attack       << "\n";
    file << p.coin         << "\n";
    file << p.exp          << "\n";
    file << p.level        << "\n";
    file << p.currentFloor << "\n";

    for (int i = 0; i < MAX_INVENTORY; i++) {
        file << p.inventory[i] << "\n";
    }
    file.close(); 
    cout << "Game berhasil disimpan ke 'savegame.txt'!\n";
    pause();
}

bool loadGame(Player& p) {
    ifstream file("savegame.txt"); 
    if (!file.is_open()) return false; 
    
    file >> p.name;
    file >> p.hp;
    file >> p.maxHp;
    file >> p.attack;
    file >> p.coin;
    file >> p.exp;
    file >> p.level;
    file >> p.currentFloor;

    for (int i = 0; i < MAX_INVENTORY; i++) {
        file >> p.inventory[i];
    }
    file.close();
    return true; 
}

// ============================================================
//   MAIN MENU & MAIN FUNGSI
// ============================================================
void mainMenu(Player& p) {
    int choice;
    do {
        showStatus(p);
        cout << "\n=== MENU UTAMA ===\n";
        cout << "1. Grinding (Cari Monster)\n";
        cout << "2. Toko Item\n";
        cout << "3. Menu Tower (Lawan Bos)\n";
        cout << "4. Simpan Game\n";
        cout << "0. Keluar\n";
        cout << "> ";
        cin >> choice;

        switch (choice) {
            case 1: grindMenu(p); break;
            case 2: shopMenu(p);  break;
            case 3: towerMenu(p); break;
            case 4: saveGame(p);  break;
            case 0: cout << "Sampai jumpa, " << p.name << "!\n"; break;
            default: cout << "Pilihan tidak valid!\n"; pause(); break;
        }
    } while (choice != 0);
}

int main() {
    srand(time(0)); 
    int startChoice;
    string name; 
    clearScreen();
    printBanner();
    
    Player player;
    cout << "\n1. New Game\n2. Load Game\n> "; cin >> startChoice;

    if (startChoice == 2) {
        if (loadGame(player)) {
            cout << "\nGame berhasil diload! Selamat datang kembali, " << player.name << "!\n";
            pause();
        } else {
            cout << "File save tidak ditemukan. Memulai game baru...\n";
            pause();
            startChoice = 1; 
        }
    }

    if (startChoice == 1) {
        cout << "\nMasukkan nama karaktermu: "; cin >> name;
        initPlayer(player, name);
        cout << "\nSelamat datang, " << player.name << "! Petualanganmu dimulai!\n";
        pause();
    }

    mainMenu(player);
    return 0;
}