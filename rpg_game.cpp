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
using namespace std;

// ============================================================
//   KONSTANTA GLOBAL
// ============================================================
const int MAX_INVENTORY = 5;   // Batas slot inventory manual
const int MAX_ITEMS     = 6;   // Jumlah item yang tersedia di shop
const int MAX_FLOORS    = 4;   // Jumlah lantai tower

// ============================================================
//   STRUCT ITEM
//   Merepresentasikan satu item (nama, harga, bonus attack/hp)
// ============================================================
struct Item {
    char name[30];
    int  price;
    int  atkBonus;
    int  hpBonus;
};

// ============================================================
//   DATABASE ITEM (array statis, tanpa vector!)
//   Iqq, ini adalah "toko" item yang tersedia untuk dibeli.
//   Indeks array ini dipakai sebagai ID item di inventory.
// ============================================================
Item shopItems[MAX_ITEMS] = {
    {"Pedang Kayu",     50,  5,  0},
    {"Perisai Kayu",    40,  0, 20},
    {"Pedang Besi",    120, 15,  0},
    {"Baju Besi",      100,  0, 40},
    {"Pedang Mistik",  300, 35,  0},
    {"Jubah Naga",     250,  0, 80}
};

// ============================================================
//   STRUCT PLAYER
//   Semua data player disimpan di sini.
//   inventory[] menyimpan INDEKS item dari shopItems[].
//   Nilai -1 berarti slot kosong.
// ============================================================
struct Player {
    char name[30];
    int  hp;
    int  maxHp;
    int  attack;
    int  coin;
    int  exp;
    int  level;
    int  currentFloor;
    int  inventory[MAX_INVENTORY]; // Array manual! Isi: indeks item atau -1
};

// ============================================================
//   FUNGSI UTILITAS
// ============================================================

// Cross-platform clear screen (Windows vs Linux)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Pause dan tunggu input Enter dari user
void pause() {
    cout << "\n[Tekan Enter untuk melanjutkan...]";
    cin.ignore();
    cin.get();
}

// Hitung total bonus ATK dari semua item di inventory
int getTotalAtkBonus(const Player& p) {
    int total = 0;
    for (int i = 0; i < MAX_INVENTORY; i++) {
        // Cek apakah slot tidak kosong (-1 = kosong)
        if (p.inventory[i] != -1) {
            total += shopItems[p.inventory[i]].atkBonus;
        }
    }
    return total;
}

// Hitung total ATK efektif player (base + bonus item)
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
    for (int i = 0; i < MAX_INVENTORY; i++) {
        p.inventory[i] = -1;
    }
}

// ============================================================
//   LEVEL UP
//   Dipanggil setiap kali exp memenuhi threshold.
//   Threshold: level * 50 exp.
// ============================================================
void checkLevelUp(Player& p) {
    int threshold = p.level * 50;
    while (p.exp >= threshold) {
        p.exp    -= threshold;
        p.level  += 1;
        p.attack += 5;
        p.maxHp  += 20;
        p.hp      = p.maxHp; // HP penuh saat level up

        cout << "\n*** LEVEL UP! ***\n";
        cout << "Selamat Iqq! Kamu naik ke Level " << p.level << "!\n";
        cout << "ATK +5 | Max HP +20 | HP pulih penuh!\n";

        threshold = p.level * 50; // Update threshold untuk level berikutnya
    }
}

// ============================================================
//   TAMPILKAN STATUS PLAYER
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
    // Tampilkan isi inventory manual, slot per slot
    for (int i = 0; i < MAX_INVENTORY; i++) {
        cout << "[Slot " << (i + 1) << "] ";
        if (p.inventory[i] == -1) {
            cout << "(kosong)\n";
        } else {
            // Ambil data item berdasarkan indeks yang tersimpan
            cout << shopItems[p.inventory[i]].name
                 << " | ATK+" << shopItems[p.inventory[i]].atkBonus
                 << " | HP+" << shopItems[p.inventory[i]].hpBonus << "\n";
        }
    }
}

// ============================================================
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
    cout << "\n1. Serang!\n2. Kabur\n> ";

    int choice;
    cin >> choice;

    if (choice == 2) {
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
        int dmgToEnemy = getEffectiveAtk(p) + rand() % 5;
        enemyHp -= dmgToEnemy;
        cout << "Kamu menyerang! Damage: " << dmgToEnemy;

        if (enemyHp <= 0) {
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

        p.exp  += expGain;
        p.coin += coinGain;

        cout << "\n[MENANG!] EXP +" << expGain << " | Coin +" << coinGain << "\n";
        checkLevelUp(p); // Cek apakah naik level
    } else {
        // Kalah: HP player jadi 1 (tidak mati permanen di versi ini)
        p.hp = 1;
        cout << "\n[KALAH!] Kamu hampir mati! HP tersisa: 1\n";
        cout << "Pergi ke shop untuk recovery!\n";
    }

    pause();
}

// ============================================================
//   SISTEM SHOP
//   Iqq, logika BELI di sini yang paling krusial:
//   kita harus scan array inventory[] secara manual
//   untuk mencari slot kosong (-1) sebelum memasukkan item.
// ============================================================
void shopMenu(Player& p) {
    int choice;
    do {
        clearScreen();
        cout << "=== TOKO ITEM ===\n";
        cout << "Coin kamu: " << p.coin << "\n\n";
        cout << "--- DAFTAR ITEM ---\n";

        // Tampilkan semua item yang tersedia di toko
        for (int i = 0; i < MAX_ITEMS; i++) {
            cout << (i + 1) << ". " << shopItems[i].name
                 << " | Harga: " << shopItems[i].price
                 << " | ATK+" << shopItems[i].atkBonus
                 << " | HP+" << shopItems[i].hpBonus << "\n";
        }

        cout << "\n7. Jual Item\n8. Beli Ramuan HP (+30 HP, 20 coin)\n0. Kembali\n> ";
        cin >> choice;

        // ---- BELI ITEM (pilihan 1-6) ----
        if (choice >= 1 && choice <= MAX_ITEMS) {
            int idx = choice - 1; // Indeks item di shopItems[]

            // Cek apakah cukup coin
            if (p.coin < shopItems[idx].price) {
                cout << "Coin tidak cukup!\n";
                pause();
                continue;
            }

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
            if (emptySlot == -1) {
                cout << "Inventory penuh! Jual item dulu.\n";
                pause();
                continue;
            }

            // Proses pembelian: kurangi coin, isi slot inventory
            p.coin -= shopItems[idx].price;
            p.inventory[emptySlot] = idx; // Simpan INDEKS item, bukan objeknya

            // Tambah bonus HP dari item secara langsung
            p.maxHp += shopItems[idx].hpBonus;
            p.hp    += shopItems[idx].hpBonus;

            cout << shopItems[idx].name << " berhasil dibeli dan masuk ke slot " << (emptySlot + 1) << "!\n";
            pause();

        // ---- JUAL ITEM ----
        } else if (choice == 7) {
            cout << "\n--- INVENTORY KAMU ---\n";

            // Cek apakah ada item untuk dijual
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

                    // Kembalikan bonus HP yang diberikan item
                    p.maxHp -= shopItems[itemIdx].hpBonus;
                    if (p.hp > p.maxHp) p.hp = p.maxHp; // HP tidak melebihi maxHp

                    p.coin += sellPrice;
                    p.inventory[slotIdx] = -1; // Kosongkan slot (set ke -1)

                    cout << shopItems[itemIdx].name << " dijual seharga " << sellPrice << " coin!\n";
                }
                pause();
            }

        // ---- BELI RAMUAN HP ----
        } else if (choice == 8) {
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
//   Syarat naik lantai berdasarkan level minimum.
//   Iqq, array syaratLevel[] ini menyimpan level minimum
//   untuk masuk ke lantai 2, 3, dan 4.
// ============================================================
void towerMenu(Player& p) {
    // syaratLevel[i] = level minimum untuk naik ke lantai (i+2)
    // Lantai 2 butuh level 3, Lantai 3 butuh level 6, Lantai 4 butuh level 10
    int syaratLevel[MAX_FLOORS - 1] = {3, 6, 10};

    clearScreen();
    cout << "=== TOWER MENU ===\n\n";
    printTower();
    cout << "\nKamu sekarang di lantai: " << p.currentFloor << "\n";
    cout << "Level kamu            : " << p.level << "\n\n";

    // Tampilkan info syarat setiap lantai
    cout << "--- SYARAT NAIK LANTAI ---\n";
    for (int i = 0; i < MAX_FLOORS - 1; i++) {
        cout << "Lantai " << (i + 2) << " : Level >= " << syaratLevel[i];
        if (p.level >= syaratLevel[i]) cout << " [TERPENUHI]";
        else cout << " [BELUM]";
        cout << "\n";
    }

    cout << "\n1. Naik Lantai\n2. Turun Lantai\n0. Kembali\n> ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        if (p.currentFloor >= MAX_FLOORS) {
            cout << "Kamu sudah di lantai tertinggi!\n";
        } else {
            // Cek syarat level untuk naik ke lantai berikutnya
            int nextFloor   = p.currentFloor; // index 0-based untuk syaratLevel
            int levelNeeded = syaratLevel[nextFloor - 1];

            if (p.level >= levelNeeded) {
                p.currentFloor++;
                cout << "Berhasil naik ke lantai " << p.currentFloor << "!\n";
            } else {
                cout << "Level belum cukup! Butuh Level " << levelNeeded << ".\n";
            }
        }
    } else if (choice == 2) {
        if (p.currentFloor <= 1) {
            cout << "Sudah di lantai dasar!\n";
        } else {
            p.currentFloor--;
            cout << "Turun ke lantai " << p.currentFloor << ".\n";
        }
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

    if (!file.is_open()) {
        cout << "Gagal menyimpan file!\n";
        pause();
        return;
    }

    // Tulis data player ke file, satu nilai per baris
    file << p.name         << "\n";
    file << p.hp           << "\n";
    file << p.maxHp        << "\n";
    file << p.attack       << "\n";
    file << p.coin         << "\n";
    file << p.exp          << "\n";
    file << p.level        << "\n";
    file << p.currentFloor << "\n";

    // Tulis isi inventory secara manual, slot per slot
    // Iqq, kita simpan nilai integer (-1 atau indeks item)
    for (int i = 0; i < MAX_INVENTORY; i++) {
        file << p.inventory[i] << "\n";
    }

    file.close(); // Selalu tutup file setelah selesai!
    cout << "Game berhasil disimpan ke 'savegame.txt'!\n";
    pause();
}

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
    file >> p.name;
    file >> p.hp;
    file >> p.maxHp;
    file >> p.attack;
    file >> p.coin;
    file >> p.exp;
    file >> p.level;
    file >> p.currentFloor;

    // Baca isi inventory secara manual, slot per slot
    for (int i = 0; i < MAX_INVENTORY; i++) {
        file >> p.inventory[i];
    }

    file.close();
    return true; // Load berhasil
}

// ============================================================
//   MAIN MENU (Menu Utama)
// ============================================================
void mainMenu(Player& p) {
    int choice;
    do {
        showStatus(p);
        cout << "\n=== MENU UTAMA ===\n";
        cout << "1. Grinding (Cari Monster)\n";
        cout << "2. Toko Item\n";
        cout << "3. Menu Tower\n";
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
            pause();
        } else {
            cout << "File save tidak ditemukan. Memulai game baru...\n";
            pause();
            startChoice = 1; // Fallback ke new game
        }
    }

    if (startChoice == 1) {
        cout << "\nMasukkan nama karaktermu: ";
        char name[30];
        cin >> name;
        initPlayer(player, name);
        cout << "\nSelamat datang, " << player.name << "! Petualanganmu dimulai!\n";
        pause();
    }

    // Masuk ke loop menu utama
    mainMenu(player);

    return 0;
}
