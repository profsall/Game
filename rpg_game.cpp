<<<<<<< HEAD
#include <iostream>
#include <fstream>   
#include <cstdlib>   
#include <ctime>     
#include <string>   
=======
// ============================================================
//   ASCII RPG TOWER - Tugas Dasar Pemrograman
//   Dibuat untuk: Iqq
//   Deskripsi: Game RPG berbasis teks dengan sistem tower 4 lantai,
//              grinding, shop, inventory manual, save & load file.
// ============================================================

#include <iostream>
#include <fstream>   // Untuk save & load (file handling)
#include <cstdlib>   // Untuk rand() dan srand()
#include <ctime>     // Untuk seed random (time)
#include <cstring>   // Untuk strcmp dan strcpy
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
using namespace std;

// ============================================================
//   KONSTANTA GLOBAL
// ============================================================
<<<<<<< HEAD
const int MAX_INVENTORY = 5;
const int MAX_ITEMS     = 10;  // Ditambah jadi 10 item
const int MAX_FLOORS    = 4;

// ============================================================
//   STRUCT ITEM
=======
const int MAX_INVENTORY = 5;   // Batas slot inventory manual
const int MAX_ITEMS     = 6;   // Jumlah item yang tersedia di shop
const int MAX_FLOORS    = 4;   // Jumlah lantai tower

// ============================================================
//   STRUCT ITEM
//   Merepresentasikan satu item (nama, harga, bonus attack/hp)
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
// ============================================================
struct Item {
    char name[30];
    int  price;
    int  atkBonus;
    int  hpBonus;
};

// ============================================================
<<<<<<< HEAD
//   DATABASE ITEM (Toko)
=======
//   DATABASE ITEM (array statis, tanpa vector!)
//   Iqq, ini adalah "toko" item yang tersedia untuk dibeli.
//   Indeks array ini dipakai sebagai ID item di inventory.
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
// ============================================================
Item shopItems[MAX_ITEMS] = {
    {"Pedang Kayu",     50,  5,  0},
    {"Perisai Kayu",    40,  0, 20},
    {"Pedang Besi",    120, 15,  0},
    {"Baju Besi",      100,  0, 40},
    {"Pedang Mistik",  300, 35,  0},
<<<<<<< HEAD
    {"Jubah Naga",     250,  0, 80},
    {"Kapak Orc",      180, 25,  0},
    {"Tameng Baja",    200,  0, 60},
    {"Pedang Cahaya",  500, 60,  0},
    {"Mahkota Raja",   600, 20, 150}
=======
    {"Jubah Naga",     250,  0, 80}
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
};

// ============================================================
//   STRUCT PLAYER
<<<<<<< HEAD
// ============================================================
struct Player {
    string name;
=======
//   Semua data player disimpan di sini.
//   inventory[] menyimpan INDEKS item dari shopItems[].
//   Nilai -1 berarti slot kosong.
// ============================================================
struct Player {
    char name[30];
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    int  hp;
    int  maxHp;
    int  attack;
    int  coin;
    int  exp;
    int  level;
    int  currentFloor;
<<<<<<< HEAD
    int  inventory[MAX_INVENTORY]; 
=======
    int  inventory[MAX_INVENTORY]; // Array manual! Isi: indeks item atau -1
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
};

// ============================================================
//   FUNGSI UTILITAS
// ============================================================
<<<<<<< HEAD
=======

// Cross-platform clear screen (Windows vs Linux)
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

<<<<<<< HEAD
=======
// Pause dan tunggu input Enter dari user
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
void pause() {
    cout << "\n[Tekan Enter untuk melanjutkan...]";
    cin.ignore();
    cin.get();
}

<<<<<<< HEAD
int getTotalAtkBonus(const Player& p) {
    int total = 0; // Kesalahan konyol manéh (0;2;) geus dibenerkeun
    for (int i = 0; i < MAX_INVENTORY; i++) {
=======
// Hitung total bonus ATK dari semua item di inventory
int getTotalAtkBonus(const Player& p) {
    int total = 0;
    for (int i = 0; i < MAX_INVENTORY; i++) {
        // Cek apakah slot tidak kosong (-1 = kosong)
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
        if (p.inventory[i] != -1) {
            total += shopItems[p.inventory[i]].atkBonus;
        }
    }
    return total;
}

<<<<<<< HEAD
=======
// Hitung total ATK efektif player (base + bonus item)
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
int getEffectiveAtk(const Player& p) {
    return p.attack + getTotalAtkBonus(p);
}

// ============================================================
//   ASCII ART
// ============================================================
<<<<<<< HEAD
=======

>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
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

<<<<<<< HEAD
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

=======
void printBanner() {
    cout << "╔══════════════════════════════════════╗\n";
    cout << "║        ASCII RPG TOWER GAME          ║\n";
    cout << "║      Tugas Dasar Pemrograman         ║\n";
    cout << "╚══════════════════════════════════════╝\n";
}

// ============================================================
//   INISIALISASI PLAYER BARU
//   Iqq, di sini semua slot inventory diset ke -1 (kosong)
//   menggunakan loop manual, bukan fungsi otomatis.
// ============================================================
void initPlayer(Player& p, const char* playerName) {
    strcpy(p.name, playerName);
    p.hp           = 100;
    p.maxHp        = 100;
    p.attack       = 10;
    p.coin         = 100;
    p.exp          = 0;
    p.level        = 1;
    p.currentFloor = 1;

    // Inisialisasi manual semua slot inventory ke -1 (kosong)
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    for (int i = 0; i < MAX_INVENTORY; i++) {
        p.inventory[i] = -1;
    }
}

<<<<<<< HEAD
=======
// ============================================================
//   LEVEL UP
//   Dipanggil setiap kali exp memenuhi threshold.
//   Threshold: level * 50 exp.
// ============================================================
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
void checkLevelUp(Player& p) {
    int threshold = p.level * 50;
    while (p.exp >= threshold) {
        p.exp    -= threshold;
        p.level  += 1;
        p.attack += 5;
        p.maxHp  += 20;
<<<<<<< HEAD
        p.hp      = p.maxHp;

        cout << "\n*** LEVEL UP! ***\n";
        cout << "Selamat " << p.name << "! Kamu naik ke Level " << p.level << "!\n";
        cout << "ATK +5 | Max HP +20 | HP pulih penuh!\n";
        threshold = p.level * 50; 
=======
        p.hp      = p.maxHp; // HP penuh saat level up

        cout << "\n*** LEVEL UP! ***\n";
        cout << "Selamat Iqq! Kamu naik ke Level " << p.level << "!\n";
        cout << "ATK +5 | Max HP +20 | HP pulih penuh!\n";

        threshold = p.level * 50; // Update threshold untuk level berikutnya
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    }
}

// ============================================================
<<<<<<< HEAD
//   TAMPILKAN STATUS
=======
//   TAMPILKAN STATUS PLAYER
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
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
<<<<<<< HEAD
=======
    // Tampilkan isi inventory manual, slot per slot
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    for (int i = 0; i < MAX_INVENTORY; i++) {
        cout << "[Slot " << (i + 1) << "] ";
        if (p.inventory[i] == -1) {
            cout << "(kosong)\n";
        } else {
<<<<<<< HEAD
            cout << shopItems[p.inventory[i]].name << " | ATK+" << shopItems[p.inventory[i]].atkBonus << " | HP+" << shopItems[p.inventory[i]].hpBonus << "\n";
=======
            // Ambil data item berdasarkan indeks yang tersimpan
            cout << shopItems[p.inventory[i]].name
                 << " | ATK+" << shopItems[p.inventory[i]].atkBonus
                 << " | HP+" << shopItems[p.inventory[i]].hpBonus << "\n";
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
        }
    }
}

// ============================================================
<<<<<<< HEAD
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
=======
//   SISTEM GRINDING
//   Player bertarung melawan musuh acak untuk dapat EXP & Coin.
//   Iqq, perhatikan penggunaan rand() % N untuk nilai acak.
// ============================================================
void grindMenu(Player& p) {
    clearScreen();
    cout << "=== MENU GRINDING ===\n\n";

    // Musuh dengan stats acak berdasarkan lantai saat ini
    int enemyHp  = 30 + (p.currentFloor * 20) + rand() % 20;
    int enemyAtk = 5  + (p.currentFloor * 3)  + rand() % 5;

    cout << "Seekor monster muncul di lantai " << p.currentFloor << "!\n";
    cout << "Monster HP  : " << enemyHp  << "\n";
    cout << "Monster ATK : " << enemyAtk << "\n";
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    cout << "\n1. Serang!\n2. Kabur\n> ";

    int choice;
    cin >> choice;

    if (choice == 2) {
<<<<<<< HEAD
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
=======
        cout << "Kamu berhasil kabur!\n";
        pause();
        return;
    }

    // Simulasi pertarungan sederhana: giliran demi giliran
    int playerHp  = p.hp;
    int rounds    = 0;

    cout << "\n--- PERTARUNGAN ---\n";
    while (playerHp > 0 && enemyHp > 0) {
        // Player menyerang musuh
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
        int dmgToEnemy = getEffectiveAtk(p) + rand() % 5;
        enemyHp -= dmgToEnemy;
        cout << "Kamu menyerang! Damage: " << dmgToEnemy;

        if (enemyHp <= 0) {
<<<<<<< HEAD
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
=======
            cout << " -> Monster KALAH!\n";
            break;
        }

        // Musuh menyerang player
        int dmgToPlayer = enemyAtk + rand() % 3;
        playerHp -= dmgToPlayer;
        cout << " | Monster balik serang! Kamu kena " << dmgToPlayer << " damage. HP: " << playerHp << "\n";
        rounds++;
    }

    // Update HP player (tidak boleh < 0)
    p.hp = (playerHp > 0) ? playerHp : 0;

    if (playerHp > 0) {
        // Menang: hitung reward
        int expGain  = 20 + (p.currentFloor * 10) + rand() % 15;
        int coinGain = 10 + (p.currentFloor * 5)  + rand() % 10;
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201

        p.exp  += expGain;
        p.coin += coinGain;

        cout << "\n[MENANG!] EXP +" << expGain << " | Coin +" << coinGain << "\n";
<<<<<<< HEAD
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
=======
        checkLevelUp(p); // Cek apakah naik level
    } else {
        // Kalah: HP player jadi 1 (tidak mati permanen di versi ini)
        p.hp = 1;
        cout << "\n[KALAH!] Kamu hampir mati! HP tersisa: 1\n";
        cout << "Pergi ke shop untuk recovery!\n";
    }

    pause();
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
}

// ============================================================
//   SISTEM SHOP
<<<<<<< HEAD
=======
//   Iqq, logika BELI di sini yang paling krusial:
//   kita harus scan array inventory[] secara manual
//   untuk mencari slot kosong (-1) sebelum memasukkan item.
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
// ============================================================
void shopMenu(Player& p) {
    int choice;
    do {
        clearScreen();
        cout << "=== TOKO ITEM ===\n";
        cout << "Coin kamu: " << p.coin << "\n\n";
        cout << "--- DAFTAR ITEM ---\n";

<<<<<<< HEAD
=======
        // Tampilkan semua item yang tersedia di toko
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
        for (int i = 0; i < MAX_ITEMS; i++) {
            cout << (i + 1) << ". " << shopItems[i].name
                 << " | Harga: " << shopItems[i].price
                 << " | ATK+" << shopItems[i].atkBonus
                 << " | HP+" << shopItems[i].hpBonus << "\n";
        }

<<<<<<< HEAD
        cout << "\n11. Jual Item\n12. Beli Ramuan HP (+30 HP, 20 coin)\n0. Kembali\n> ";
        cin >> choice;

        if (choice >= 1 && choice <= MAX_ITEMS) {
            int idx = choice - 1; 

=======
        cout << "\n7. Jual Item\n8. Beli Ramuan HP (+30 HP, 20 coin)\n0. Kembali\n> ";
        cin >> choice;

        // ---- BELI ITEM (pilihan 1-6) ----
        if (choice >= 1 && choice <= MAX_ITEMS) {
            int idx = choice - 1; // Indeks item di shopItems[]

            // Cek apakah cukup coin
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
            if (p.coin < shopItems[idx].price) {
                cout << "Coin tidak cukup!\n";
                pause();
                continue;
            }

<<<<<<< HEAD
            int emptySlot = -1;
            for (int i = 0; i < MAX_INVENTORY; i++) {
                if (p.inventory[i] == -1) {
                    emptySlot = i; 
                    break;         
                }
            }

=======
            // Cari slot kosong di inventory secara MANUAL
            // Iqq, ini inti dari array manual! Kita loop dari index 0
            // dan cari slot pertama yang nilainya -1 (kosong).
            int emptySlot = -1;
            for (int i = 0; i < MAX_INVENTORY; i++) {
                if (p.inventory[i] == -1) {
                    emptySlot = i; // Simpan index slot kosong
                    break;         // Berhenti di slot kosong pertama
                }
            }

            // Jika tidak ada slot kosong, tolak pembelian
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
            if (emptySlot == -1) {
                cout << "Inventory penuh! Jual item dulu.\n";
                pause();
                continue;
            }

<<<<<<< HEAD
            p.coin -= shopItems[idx].price;
            p.inventory[emptySlot] = idx; 

=======
            // Proses pembelian: kurangi coin, isi slot inventory
            p.coin -= shopItems[idx].price;
            p.inventory[emptySlot] = idx; // Simpan INDEKS item, bukan objeknya

            // Tambah bonus HP dari item secara langsung
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
            p.maxHp += shopItems[idx].hpBonus;
            p.hp    += shopItems[idx].hpBonus;

            cout << shopItems[idx].name << " berhasil dibeli dan masuk ke slot " << (emptySlot + 1) << "!\n";
            pause();

<<<<<<< HEAD
        } else if (choice == 11) {
            cout << "\n--- INVENTORY KAMU ---\n";

=======
        // ---- JUAL ITEM ----
        } else if (choice == 7) {
            cout << "\n--- INVENTORY KAMU ---\n";

            // Cek apakah ada item untuk dijual
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
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

<<<<<<< HEAD
                    p.maxHp -= shopItems[itemIdx].hpBonus;
                    if (p.hp > p.maxHp) p.hp = p.maxHp; 

                    p.coin += sellPrice;
                    p.inventory[slotIdx] = -1; 
=======
                    // Kembalikan bonus HP yang diberikan item
                    p.maxHp -= shopItems[itemIdx].hpBonus;
                    if (p.hp > p.maxHp) p.hp = p.maxHp; // HP tidak melebihi maxHp

                    p.coin += sellPrice;
                    p.inventory[slotIdx] = -1; // Kosongkan slot (set ke -1)
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201

                    cout << shopItems[itemIdx].name << " dijual seharga " << sellPrice << " coin!\n";
                }
                pause();
            }

<<<<<<< HEAD
        } else if (choice == 12) {
=======
        // ---- BELI RAMUAN HP ----
        } else if (choice == 8) {
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
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
<<<<<<< HEAD
// ============================================================
void towerMenu(Player& p) {
=======
//   Syarat naik lantai berdasarkan level minimum.
//   Iqq, array syaratLevel[] ini menyimpan level minimum
//   untuk masuk ke lantai 2, 3, dan 4.
// ============================================================
void towerMenu(Player& p) {
    // syaratLevel[i] = level minimum untuk naik ke lantai (i+2)
    // Lantai 2 butuh level 3, Lantai 3 butuh level 6, Lantai 4 butuh level 10
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    int syaratLevel[MAX_FLOORS - 1] = {3, 6, 10};

    clearScreen();
    cout << "=== TOWER MENU ===\n\n";
    printTower();
    cout << "\nKamu sekarang di lantai: " << p.currentFloor << "\n";
    cout << "Level kamu            : " << p.level << "\n\n";

<<<<<<< HEAD
=======
    // Tampilkan info syarat setiap lantai
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    cout << "--- SYARAT NAIK LANTAI ---\n";
    for (int i = 0; i < MAX_FLOORS - 1; i++) {
        cout << "Lantai " << (i + 2) << " : Level >= " << syaratLevel[i];
        if (p.level >= syaratLevel[i]) cout << " [TERPENUHI]";
        else cout << " [BELUM]";
        cout << "\n";
    }

<<<<<<< HEAD
    cout << "\n1. Naik Lantai (Lawan Bos)\n2. Turun Lantai\n0. Kembali\n> ";
=======
    cout << "\n1. Naik Lantai\n2. Turun Lantai\n0. Kembali\n> ";
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    int choice;
    cin >> choice;

    if (choice == 1) {
        if (p.currentFloor >= MAX_FLOORS) {
            cout << "Kamu sudah di lantai tertinggi!\n";
<<<<<<< HEAD
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
=======
        } else {
            // Cek syarat level untuk naik ke lantai berikutnya
            int nextFloor   = p.currentFloor; // index 0-based untuk syaratLevel
            int levelNeeded = syaratLevel[nextFloor - 1];

            if (p.level >= levelNeeded) {
                p.currentFloor++;
                cout << "Berhasil naik ke lantai " << p.currentFloor << "!\n";
            } else {
                cout << "Level belum cukup! Butuh Level " << levelNeeded << ".\n";
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
            }
        }
    } else if (choice == 2) {
        if (p.currentFloor <= 1) {
            cout << "Sudah di lantai dasar!\n";
        } else {
            p.currentFloor--;
            cout << "Turun ke lantai " << p.currentFloor << ".\n";
        }
<<<<<<< HEAD
        pause();
    }
}

// ============================================================
//   SAVE & LOAD
// ============================================================
void saveGame(const Player& p) {
    ofstream file("savegame.txt"); 
=======
    }

    pause();
}

// ============================================================
//   SAVE GAME (File Handling - fstream)
//   Iqq, di sini kita menulis data player ke file .txt
//   menggunakan ofstream. Setiap field ditulis per baris
//   agar mudah dibaca kembali saat load.
// ============================================================
void saveGame(const Player& p) {
    ofstream file("savegame.txt"); // Buka file untuk ditulis

>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    if (!file.is_open()) {
        cout << "Gagal menyimpan file!\n";
        pause();
        return;
    }
<<<<<<< HEAD
=======

    // Tulis data player ke file, satu nilai per baris
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    file << p.name         << "\n";
    file << p.hp           << "\n";
    file << p.maxHp        << "\n";
    file << p.attack       << "\n";
    file << p.coin         << "\n";
    file << p.exp          << "\n";
    file << p.level        << "\n";
    file << p.currentFloor << "\n";

<<<<<<< HEAD
    for (int i = 0; i < MAX_INVENTORY; i++) {
        file << p.inventory[i] << "\n";
    }
    file.close(); 
=======
    // Tulis isi inventory secara manual, slot per slot
    // Iqq, kita simpan nilai integer (-1 atau indeks item)
    for (int i = 0; i < MAX_INVENTORY; i++) {
        file << p.inventory[i] << "\n";
    }

    file.close(); // Selalu tutup file setelah selesai!
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    cout << "Game berhasil disimpan ke 'savegame.txt'!\n";
    pause();
}

<<<<<<< HEAD
bool loadGame(Player& p) {
    ifstream file("savegame.txt"); 
    if (!file.is_open()) return false; 
    
=======
// ============================================================
//   LOAD GAME (File Handling - fstream)
//   Iqq, di sini kita membaca kembali data dari file .txt
//   menggunakan ifstream. Urutan baca harus SAMA dengan
//   urutan tulis di fungsi saveGame() di atas.
// ============================================================
bool loadGame(Player& p) {
    ifstream file("savegame.txt"); // Buka file untuk dibaca

    if (!file.is_open()) {
        return false; // File tidak ditemukan
    }

    // Baca data player sesuai urutan penulisan
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
    file >> p.name;
    file >> p.hp;
    file >> p.maxHp;
    file >> p.attack;
    file >> p.coin;
    file >> p.exp;
    file >> p.level;
    file >> p.currentFloor;

<<<<<<< HEAD
    for (int i = 0; i < MAX_INVENTORY; i++) {
        file >> p.inventory[i];
    }
    file.close();
    return true; 
}

// ============================================================
//   MAIN MENU & MAIN FUNGSI
=======
    // Baca isi inventory secara manual, slot per slot
    for (int i = 0; i < MAX_INVENTORY; i++) {
        file >> p.inventory[i];
    }

    file.close();
    return true; // Load berhasil
}

// ============================================================
//   MAIN MENU (Menu Utama)
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
// ============================================================
void mainMenu(Player& p) {
    int choice;
    do {
        showStatus(p);
        cout << "\n=== MENU UTAMA ===\n";
        cout << "1. Grinding (Cari Monster)\n";
        cout << "2. Toko Item\n";
<<<<<<< HEAD
        cout << "3. Menu Tower (Lawan Bos)\n";
=======
        cout << "3. Menu Tower\n";
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
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

<<<<<<< HEAD
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
=======
// ============================================================
//   FUNGSI MAIN
//   Entry point program. Iqq, alur utamanya:
//   1. Tampilkan banner awal
//   2. Tanya mau new game atau load
//   3. Masuk ke main menu
// ============================================================
int main() {
    srand(time(0)); // Seed random agar nilai acak berbeda tiap run

    clearScreen();
    printBanner();

    Player player;
    cout << "\n1. New Game\n2. Load Game\n> ";
    int startChoice;
    cin >> startChoice;

    if (startChoice == 2) {
        if (loadGame(player)) {
            cout << "\nGame berhasil di-load! Selamat datang kembali, " << player.name << "!\n";
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
            pause();
        } else {
            cout << "File save tidak ditemukan. Memulai game baru...\n";
            pause();
<<<<<<< HEAD
            startChoice = 1; 
=======
            startChoice = 1; // Fallback ke new game
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
        }
    }

    if (startChoice == 1) {
<<<<<<< HEAD
        cout << "\nMasukkan nama karaktermu: "; cin >> name;
=======
        cout << "\nMasukkan nama karaktermu: ";
        char name[30];
        cin >> name;
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
        initPlayer(player, name);
        cout << "\nSelamat datang, " << player.name << "! Petualanganmu dimulai!\n";
        pause();
    }

<<<<<<< HEAD
    mainMenu(player);
    return 0;
}
=======
    // Masuk ke loop menu utama
    mainMenu(player);

    return 0;
}
>>>>>>> bb71a60031e08a5142ea286d6432e540412fc201
