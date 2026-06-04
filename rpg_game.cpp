#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

const int EQUIP_ITEMS   = 5;
const int BACKPACK_SIZE = 20;
const int MAX_ITEMS     = 16;
const int MAX_FLOORS    = 4;
const int MAX_MATERIALS = 8;

struct Item
{
    char name[30];
    int  price;
    int  atkBonus;
    int  hpBonus;
};

Item shopItems[MAX_ITEMS] = {
    {"Pedang Kayu",      50,  5,   0},
    {"Pedang Besi",     120, 15,   0},
    {"Kapak Orc",       180, 25,   0},
    {"Pedang Mistik",   300, 35,   0},
    {"Pedang Cahaya",   500, 60,   0},
    {"Tombak Naga",     420, 50,   0},
    {"Perisai Kayu",     40,  0,  20},
    {"Baju Besi",       100,  0,  40},
    {"Tameng Baja",     200,  0,  60},
    {"Jubah Naga",      250,  0,  80},
    {"Mahkota Raja",    600, 20, 150},
    {"Helm Baja",       130,  5,  35},
    {"Cincin Api",      150, 18,  10},
    {"Kalung Perkasa",  220, 12,  50},
    {"Gelang Hantu",    180, 22,   0},
    {"Sabuk Petarung",   90,  8,  25},
};

struct Material
{
    char name[20];
    int  sellPrice;
    int  healAmount;
};

Material materials[MAX_MATERIALS] = {
    {"Ranting",          5,  0},
    {"Batu Kecil",       8,  0},
    {"Serpihan Besi",   15,  0},
    {"Tulang Monster",  12,  0},
    {"Emas Bongkahan",  40,  0},
    {"Kristal Merah",   60,  0},
    {"Ramuan HP Kecil",  0, 20},
    {"Ramuan HP Besar",  0, 50},
};

int matSellPrice[MAX_MATERIALS] = {5, 8, 15, 12, 40, 60, 10, 25};

struct Player
{
    string name;
    int hp;
    int maxHp;
    int attack;
    int coin;
    int exp;
    int level;
    int currentFloor;
    int inventory[EQUIP_ITEMS];
    int backpack[BACKPACK_SIZE];
};

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
    cout << "\n[Tekan Enter untuk melanjutkan...]";
    cin.ignore();
    cin.get();
}

void banner()
{
    cout << "        ASCII RPG TOWER GAME          \n";
}

void printPlayer()
{
    cout << "   O   \n";
    cout << "  /|\\ \n";
    cout << "  / \\ \n";
}

void printMonster()
{
    cout << "   (\\__/)  \n";
    cout << "   (o .o)  \n";
    cout << "   (>  <)  \n";
}

void printBoss()
{
    cout << "    \\|/    \n";
    cout << "   (@_@)   \n";
    cout << "  /| V |\\  \n";
    cout << "   |   |   \n";
    cout << "  /     \\  \n";
}

void printTower()
{
    cout << " __     __     __" << endl;
    cout << "|  |___|  |___|  |" << endl;
    cout << "|                |" << endl;
    cout << "|     Floor 4    |" << endl;
    cout << "|                |" << endl;
    cout << "|     Floor 3    |" << endl;
    cout << "|                |" << endl;
    cout << "|     Floor 2    |" << endl;
    cout << "|                |" << endl;
    cout << "|     Floor 1    |" << endl;
    cout << "|________________|" << endl;
}

void SelectionS(Player& p)
{
    int temp;
    for (int i = 0; i < BACKPACK_SIZE; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < BACKPACK_SIZE; j++)
        {
            if (p.backpack[j] < p.backpack[minIndex])
            {
                minIndex = j;
            }
        }
        temp = p.backpack[minIndex];
        p.backpack[minIndex] = p.backpack[i];
        p.backpack[i] = temp;
    }
}

void InsertionS(Player& p)
{
    int j, temp;
    for (int i = 1; i < BACKPACK_SIZE; i++)
    {
        temp = p.backpack[i];
        j = i - 1;
        while (j >= 0 && p.backpack[j] > temp)
        {
            p.backpack[j + 1] = p.backpack[j];
            j = j - 1;
        }
        p.backpack[j + 1] = temp;
    }
}

int linearSearch(Player& p, int targetID)
{
    for (int i = 0; i < BACKPACK_SIZE; i++)
    {
        if (p.backpack[i] == targetID)
        {
            return i;
        }
    }
    return -1;
}

int binarySearch(Player& p, int targetID)
{
    InsertionS(p);
    int low = 0, high = BACKPACK_SIZE - 1;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (p.backpack[mid] == targetID)
        {
            return mid;
        }
        if (p.backpack[mid] < targetID)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}

int getTotalAtkBonus(const Player& p)
{
    int total = 0;
    for (int i = 0; i < EQUIP_ITEMS; i++)
    {
        if (p.inventory[i] != -1)
        {
            total += shopItems[p.inventory[i]].atkBonus;
        }
    }
    return total;
}

int getEffectiveAtk(const Player& p)
{
    return p.attack + getTotalAtkBonus(p);
}

bool addToBackpack(Player& p, int matIndex)
{
    for (int i = 0; i < BACKPACK_SIZE; i++)
    {
        if (p.backpack[i] == -1)
        {
            p.backpack[i] = matIndex;
            return true;
        }
    }
    return false;
}

void dropLoot(Player& p, bool isBoss)
{
    cout << "\n=== LOOT ===\n";

    int drops = isBoss ? (2 + rand() % 3) : (1 + rand() % 2);

    for (int d = 0; d < drops; d++)
    {
        int roll = rand() % 100;
        int matIdx = -1;

    if (roll < 15)
    {
        matIdx = 6 + rand() % 2;  // hasilnya 6 atau 7
    }
    else if (roll < 15 + p.currentFloor * 8)
    {
        matIdx = 4 + rand() % 2;  // hasilnya 4 atau 5
    }
    else if (roll < 50)
    {
        matIdx = 2 + rand() % 2;  // hasilnya 2 atau 3
    }
    else
    {
        matIdx = 0 + rand() % 2;  // hasilnya 0 atau 1
    }

        if (addToBackpack(p, matIdx))
        {
            cout << " + Kamu dapat: " << materials[matIdx].name << "!\n";
        }
        else
        {
            cout << " ! Backpack penuh, " << materials[matIdx].name << " terbuang!\n";
        }
    }
}

void initPlayer(Player& p, const string& playerName)
{
    p.name = playerName;
    p.hp = 100;
    p.maxHp = 100;
    p.attack = 10;
    p.coin = 100;
    p.exp = 0;
    p.level = 1;
    p.currentFloor = 1;

    for (int i = 0; i < EQUIP_ITEMS; i++)
    {
        p.inventory[i] = -1;
    }

    for (int i = 0; i < BACKPACK_SIZE; i++)
    {
        p.backpack[i] = -1;
    }
}

void checkLevelUp(Player& p)
{
    int threshold = p.level * 50;
    while (p.exp >= threshold)
    {
        p.exp    -= threshold;
        p.level  += 1;
        p.attack += 5;
        p.maxHp  += 20;
        p.hp      = p.maxHp;

        cout << "\n=== LEVEL UP! ===\n";
        cout << "Selamat " << p.name << "! Kamu naik ke Level " << p.level << "!\n";
        cout << "ATK +5 | Max HP +20 | HP pulih penuh!\n";

        threshold = p.level * 50;
    }
}

void showStatus(const Player& p)
{
    clearScreen();
    banner();

    cout << "\n=== STATUS PLAYER ===\n";
    printPlayer();
    cout << "Nama       : " << p.name << "\n";
    cout << "Level      : " << p.level << "\n";
    cout << "HP         : " << p.hp << " / " << p.maxHp << "\n";
    cout << "ATK        : " << p.attack << " (+" << getTotalAtkBonus(p) << " dari item) = " << getEffectiveAtk(p) << "\n";
    cout << "EXP        : " << p.exp << " / " << (p.level * 50) << "\n";
    cout << "Coin       : " << p.coin << "\n";
    cout << "Lantai     : " << p.currentFloor << " / " << MAX_FLOORS << "\n";

    cout << "\n=== INVENTORY (" << EQUIP_ITEMS << " slot equip) ===\n";
    for (int i = 0; i < EQUIP_ITEMS; i++)
    {
        cout << "[Slot " << (i + 1) << "] ";
        if (p.inventory[i] == -1)
        {
            cout << "(kosong)\n";
        }
        else
        {
            cout << shopItems[p.inventory[i]].name
                 << " | ATK+" << shopItems[p.inventory[i]].atkBonus
                 << " | HP+"  << shopItems[p.inventory[i]].hpBonus << "\n";
        }
    }

    int bpCount = 0;
    for (int i = 0; i < BACKPACK_SIZE; i++)
    {
        if (p.backpack[i] != -1)
        {
            bpCount++;
        }
    }
    cout << "\n[Backpack: " << bpCount << "/" << BACKPACK_SIZE << " slot terisi]\n";
}

bool startBattle(Player& p, bool isBoss, int floor)
{
    clearScreen();

    int enemyHp, enemyAtk;
    string enemyName;

    if (isBoss)
    {
        cout << "=== PERTARUNGAN MELAWAN BOS LANTAI " << floor << " ===\n\n";
        printBoss();
        enemyName = "GUARDIAN BOS";
        enemyHp   = 100 + (floor * 50) + rand() % 30;
        enemyAtk  = 15  + (floor * 8)  + rand() % 5;
    }
    else
    {
        cout << "=== PERTARUNGAN MONSTER ===\n\n";
        printMonster();
        enemyName = "Monster Liar";
        enemyHp   = 30 + (floor * 20) + rand() % 20;
        enemyAtk  = 5  + (floor * 3)  + rand() % 5;
    }

    cout << "\n      [ VS ]      \n\n";
    printPlayer();
    cout << "   ( " << p.name << " )\n";
    cout << "\n" << enemyName << " menghadangmu!\n";
    cout << "HP Musuh : " << enemyHp  << "\n";
    cout << "ATK Musuh: " << enemyAtk << "\n";
    cout << "\n1. Serang!\n2. Kabur\n> ";

    int choice;
    cin >> choice;

    if (choice == 2)
    {
        if (isBoss)
        {
            cout << "Kamu tidak bisa lari dari pertarungan bos!\n";
            pause();
        }
        else
        {
            cout << "Kamu berhasil kabur!\n";
            pause();
            return false;
        }
    }

    int playerHp = p.hp;
    cout << "\n=== MULAI PERTARUNGAN ===\n";

    while (playerHp > 0 && enemyHp > 0)
    {
        int dmgToEnemy = getEffectiveAtk(p) + rand() % 5;
        enemyHp -= dmgToEnemy;
        cout << "Kamu menyerang! Damage: " << dmgToEnemy;

        if (enemyHp <= 0)
        {
            cout << " -> " << enemyName << " KALAH!\n";
            break;
        }

        int dmgToPlayer = enemyAtk + rand() % 3;
        playerHp -= dmgToPlayer;
        cout << " | " << enemyName << " membalas! Kena " << dmgToPlayer << " damage. HP: " << playerHp << "\n";
    }

    p.hp = (playerHp > 0) ? playerHp : 0;

    if (playerHp > 0)
    {
        int expGain, coinGain;

        if (isBoss)
        {
            expGain  = 50  + (floor * 10) + rand() % 15;
            coinGain = 100 + (floor * 5)  + rand() % 10;
        }
        else
        {
            expGain  = 20 + (floor * 10) + rand() % 15;
            coinGain = 10 + (floor * 5)  + rand() % 10;
        }

        cout << "\n[MENANG!] EXP +" << expGain << " | Coin +" << coinGain << "\n";
        p.exp  += expGain;
        p.coin += coinGain;

        checkLevelUp(p);
        dropLoot(p, isBoss);

        return true;
    }
    else
    {
        p.hp = 1;
        cout << "\n[KALAH!] Kamu dihajar habis-habisan! HP tersisa: 1\n";
        cout << "Segera beli ramuan di toko!\n";
        return false;
    }
}

void grindMenu(Player& p)
{
    startBattle(p, false, p.currentFloor);
}

void shopMenu(Player& p)
{
    int choice;

    do
    {
        clearScreen();
        cout << "=== TOKO ITEM ===\n";
        cout << "Coin kamu: " << p.coin << "\n\n";

        cout << "=== SENJATA ===\n";
        for (int i = 0; i <= 5; i++)
        {
            cout << (i + 1) << ". " << shopItems[i].name
                 << " | Harga: " << shopItems[i].price
                 << " | ATK+" << shopItems[i].atkBonus
                 << " | HP+"  << shopItems[i].hpBonus << "\n";
        }

        cout << "\n=== ARMOR ===\n";
        for (int i = 6; i <= 11; i++)
        {
            cout << (i + 1) << ". " << shopItems[i].name
                 << " | Harga: " << shopItems[i].price
                 << " | ATK+" << shopItems[i].atkBonus
                 << " | HP+"  << shopItems[i].hpBonus << "\n";
        }

        cout << "\n=== AKSESORI ===\n";
        for (int i = 12; i < MAX_ITEMS; i++)
        {
            cout << (i + 1) << ". " << shopItems[i].name
                 << " | Harga: " << shopItems[i].price
                 << " | ATK+" << shopItems[i].atkBonus
                 << " | HP+"  << shopItems[i].hpBonus << "\n";
        }

        cout << "\n17. Jual Item Equip\n";
        cout << "18. Jual Material dari Backpack\n";
        cout << "19. Beli Ramuan HP (+30 HP, 20 coin)\n";
        cout << "20. Pakai Potion dari Backpack\n";
        cout << "0. Kembali\n> ";
        cin >> choice;

        if (choice >= 1 && choice <= MAX_ITEMS)
        {
            int idx = choice - 1;

            if (p.coin < shopItems[idx].price)
            {
                cout << "Coin tidak cukup!\n";
                pause();
                continue;
            }

            int emptySlot = -1;
            for (int i = 0; i < EQUIP_ITEMS; i++)
            {
                if (p.inventory[i] == -1)
                {
                    emptySlot = i;
                    break;
                }
            }

            if (emptySlot == -1)
            {
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
        }
        else if (choice == 17)
        {
            cout << "\n=== INVENTORY KAMU ===\n";

            bool hasItem = false;
            for (int i = 0; i < EQUIP_ITEMS; i++)
            {
                if (p.inventory[i] != -1)
                {
                    hasItem = true;
                    cout << (i + 1) << ". [Slot " << (i + 1) << "] "
                         << shopItems[p.inventory[i]].name
                         << " (jual: " << shopItems[p.inventory[i]].price / 2 << " coin)\n";
                }
            }

            if (!hasItem)
            {
                cout << "Inventory kosong!\n";
                pause();
                continue;
            }

            cout << "0. Batal\nPilih slot yang dijual: ";
            int sellSlot;
            cin >> sellSlot;

            if (sellSlot >= 1 && sellSlot <= EQUIP_ITEMS)
            {
                int slotIdx = sellSlot - 1;

                if (p.inventory[slotIdx] == -1)
                {
                    cout << "Slot kosong!\n";
                }
                else
                {
                    int itemIdx   = p.inventory[slotIdx];
                    int sellPrice = shopItems[itemIdx].price / 2;

                    p.maxHp -= shopItems[itemIdx].hpBonus;
                    if (p.hp > p.maxHp)
                    {
                        p.hp = p.maxHp;
                    }

                    p.coin += sellPrice;
                    p.inventory[slotIdx] = -1;

                    cout << shopItems[itemIdx].name << " dijual seharga " << sellPrice << " coin!\n";
                }
                pause();
            }
        }
        else if (choice == 18)
        {
            cout << "\n=== MATERIAL DI BACKPACK ===\n";

            bool hasMat = false;
            for (int i = 0; i < BACKPACK_SIZE; i++)
            {
                if (p.backpack[i] != -1)
                {
                    hasMat = true;
                    cout << "[" << (i + 1) << "] " << materials[p.backpack[i]].name
                         << " | Jual: " << matSellPrice[p.backpack[i]] << " coin\n";
                }
            }

            if (!hasMat)
            {
                cout << "Backpack kosong!\n";
                pause();
                continue;
            }

            cout << "\n0. Batal | 99. Jual SEMUA\nPilih slot: ";
            int sellSlot;
            cin >> sellSlot;

            if (sellSlot == 99)
            {
                int total = 0;
                for (int i = 0; i < BACKPACK_SIZE; i++)
                {
                    if (p.backpack[i] != -1)
                    {
                        total += matSellPrice[p.backpack[i]];
                        p.backpack[i] = -1;
                    }
                }
                p.coin += total;
                cout << "Semua material dijual! Total: " << total << " coin!\n";
            }
            else if (sellSlot >= 1 && sellSlot <= BACKPACK_SIZE)
            {
                int idx = sellSlot - 1;
                if (p.backpack[idx] == -1)
                {
                    cout << "Slot kosong!\n";
                }
                else
                {
                    int matId = p.backpack[idx];
                    p.coin += matSellPrice[matId];
                    cout << materials[matId].name << " dijual seharga " << matSellPrice[matId] << " coin!\n";
                    p.backpack[idx] = -1;
                }
            }
            pause();
        }
        else if (choice == 19)
        {
            if (p.coin < 20)
            {
                cout << "Coin tidak cukup! (butuh 20 coin)\n";
            }
            else
            {
                p.coin -= 20;
                p.hp = (p.hp + 30 > p.maxHp) ? p.maxHp : p.hp + 30;
                cout << "HP pulih +30! HP sekarang: " << p.hp << "\n";
            }
            pause();
        }
        else if (choice == 20)
        {
            cout << "\n=== POTION DI BACKPACK ===\n";

            bool hasPotion = false;
            for (int i = 0; i < BACKPACK_SIZE; i++)
            {
                if (p.backpack[i] == 6 || p.backpack[i] == 7)
                {
                    hasPotion = true;
                    cout << "[" << (i + 1) << "] " << materials[p.backpack[i]].name
                         << " (+" << materials[p.backpack[i]].healAmount << " HP)\n";
                }
            }

            if (!hasPotion)
            {
                cout << "Tidak ada potion di backpack!\n";
                pause();
                continue;
            }

            cout << "0. Batal\nPilih slot potion: ";
            int potSlot;
            cin >> potSlot;

            if (potSlot >= 1 && potSlot <= BACKPACK_SIZE)
            {
                int idx = potSlot - 1;
                if (p.backpack[idx] == 6 || p.backpack[idx] == 7)
                {
                    int heal = materials[p.backpack[idx]].healAmount;
                    p.hp = (p.hp + heal > p.maxHp) ? p.maxHp : p.hp + heal;
                    cout << "Kamu minum " << materials[p.backpack[idx]].name
                         << "! HP +" << heal << " -> " << p.hp << "\n";
                    p.backpack[idx] = -1;
                }
                else
                {
                    cout << "Itu bukan potion!\n";
                }
            }
            pause();
        }

    } while (choice != 0);
}

void towerMenu(Player& p)
{
    int syaratLevel[MAX_FLOORS - 1] = {3, 6, 10};

    clearScreen();
    cout << "=== TOWER MENU ===\n\n";
    printTower();
    cout << "\nKamu sekarang di lantai: " << p.currentFloor << "\n";
    cout << "Level kamu            : " << p.level << "\n\n";

    cout << "--- SYARAT NAIK LANTAI ---\n";
    for (int i = 0; i < MAX_FLOORS - 1; i++)
    {
        cout << "Lantai " << (i + 2) << " : Level >= " << syaratLevel[i];
        if (p.level >= syaratLevel[i])
        {
            cout << " [TERPENUHI]";
        }
        else
        {
            cout << " [BELUM]";
        }
        cout << "\n";
    }

    cout << "\n1. Naik Lantai (Lawan Bos)\n2. Turun Lantai\n0. Kembali\n> ";
    int choice;
    cin >> choice;

    if (choice == 1)
    {
        if (p.currentFloor >= MAX_FLOORS)
        {
            cout << "Kamu sudah di lantai tertinggi!\n";
            pause();
        }
        else
        {
            int levelNeeded = syaratLevel[p.currentFloor - 1];

            if (p.level >= levelNeeded)
            {
                cout << "\nBOS LANTAI MENGHALANGI JALANMU!\n";
                pause();

                bool win = startBattle(p, true, p.currentFloor);

                if (win)
                {
                    p.currentFloor++;
                    cout << "\nBOS DIKALAHKAN! Kamu berhasil naik ke lantai " << p.currentFloor << "!\n";
                }
                else
                {
                    cout << "\nKamu gagal mengalahkan bos. Latihan lagi sana!\n";
                }
                pause();
            }
            else
            {
                cout << "Level belum cukup! Butuh Level " << levelNeeded << ".\n";
                pause();
            }
        }
    }
    else if (choice == 2)
    {
        if (p.currentFloor <= 1)
        {
            cout << "Sudah di lantai dasar!\n";
        }
        else
        {
            p.currentFloor--;
            cout << "Turun ke lantai " << p.currentFloor << ".\n";
        }
        pause();
    }
}

void backpackMenu(Player& p)
{
    int choice;

    do
    {
        clearScreen();
        cout << "=== TAS BACKPACK (" << BACKPACK_SIZE << " SLOT) ===\n\n";

        int filled = 0;
        for (int i = 0; i < BACKPACK_SIZE; i++)
        {
            cout << "[" << (i + 1) << "] ";
            if (p.backpack[i] == -1)
            {
                cout << "(kosong)\n";
            }
            else
            {
                int mId = p.backpack[i];
                cout << materials[mId].name;
                if (materials[mId].healAmount > 0)
                {
                    cout << " [POTION +" << materials[mId].healAmount << " HP]";
                }
                else
                {
                    cout << " [Jual: " << matSellPrice[mId] << " coin]";
                }
                cout << "\n";
                filled++;
            }
        }

        cout << "\nTerisi: " << filled << "/" << BACKPACK_SIZE << "\n";
        cout << "\n1. Sortir (Selection Sort)\n";
        cout << "2. Sortir (Insertion Sort)\n";
        cout << "3. Cari Material (Linear Search)\n";
        cout << "4. Cari Material (Binary Search)\n";
        cout << "5. Pakai Potion dari Backpack\n";
        cout << "6. Buang Item dari Backpack\n";
        cout << "0. Kembali\n> ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                cout << "Mengurutkan dengan Selection Sort...\n";
                SelectionS(p);
                pause();
                break;

            case 2:
                cout << "Mengurutkan dengan Insertion Sort...\n";
                InsertionS(p);
                pause();
                break;

            case 3:
            {
                cout << "=== DAFTAR MATERIAL ===\n";
                for (int i = 0; i < MAX_MATERIALS; i++)
                {
                    cout << (i + 1) << ". " << materials[i].name << "\n";
                }

                cout << "Masukkan ID Material (1-" << MAX_MATERIALS << "): ";
                int id;
                cin >> id;

                int target = id - 1;
                int hasil  = linearSearch(p, target);

                if (hasil != -1)
                {
                    cout << materials[target].name << " ada di slot ke-" << (hasil + 1) << "!\n";
                }
                else
                {
                    cout << "Material tidak ditemukan di backpack.\n";
                }

                pause();
                break;
            }

            case 4:
            {
                cout << "=== DAFTAR MATERIAL ===\n";
                for (int i = 0; i < MAX_MATERIALS; i++)
                {
                    cout << (i + 1) << ". " << materials[i].name << "\n";
                }

                cout << "Masukkan ID Material (1-" << MAX_MATERIALS << "): ";
                int id2;
                cin >> id2;

                cout << "(Backpack akan diurutkan dulu untuk binary search)\n";
                int target2 = id2 - 1;
                int hasil2  = binarySearch(p, target2);

                if (hasil2 != -1)
                {
                    cout << materials[target2].name << " ada di slot ke-" << (hasil2 + 1) << "!\n";
                }
                else
                {
                    cout << "Material tidak ditemukan di backpack.\n";
                }

                pause();
                break;
            }

            case 5:
            {
                cout << "\n=== POTION DI BACKPACK ===\n";

                bool hasPotion = false;
                for (int i = 0; i < BACKPACK_SIZE; i++)
                {
                    if (p.backpack[i] == 6 || p.backpack[i] == 7)
                    {
                        hasPotion = true;
                        cout << "[" << (i + 1) << "] " << materials[p.backpack[i]].name
                             << " (+" << materials[p.backpack[i]].healAmount << " HP)\n";
                    }
                }

                if (!hasPotion)
                {
                    cout << "Tidak ada potion!\n";
                    pause();
                    break;
                }

                cout << "0. Batal\nPilih slot: ";
                int potSlot;
                cin >> potSlot;

                if (potSlot >= 1 && potSlot <= BACKPACK_SIZE)
                {
                    int idx = potSlot - 1;
                    if (p.backpack[idx] == 6 || p.backpack[idx] == 7)
                    {
                        int heal = materials[p.backpack[idx]].healAmount;
                        p.hp = (p.hp + heal > p.maxHp) ? p.maxHp : p.hp + heal;
                        cout << "Kamu minum " << materials[p.backpack[idx]].name
                             << "! HP +" << heal << " -> " << p.hp << "\n";
                        p.backpack[idx] = -1;
                    }
                    else
                    {
                        cout << "Itu bukan potion!\n";
                    }
                }
                pause();
                break;
            }

            case 6:
            {
                cout << "\nPilih slot yang ingin dibuang (1-" << BACKPACK_SIZE << "), 0 batal: ";
                int dumpSlot;
                cin >> dumpSlot;

                if (dumpSlot >= 1 && dumpSlot <= BACKPACK_SIZE)
                {
                    int idx = dumpSlot - 1;
                    if (p.backpack[idx] == -1)
                    {
                        cout << "Slot sudah kosong!\n";
                    }
                    else
                    {
                        cout << materials[p.backpack[idx]].name << " dibuang.\n";
                        p.backpack[idx] = -1;
                    }
                }
                pause();
                break;
            }

            case 0:
                return;

            default:
                cout << "Pilihan tidak valid!\n";
                pause();
                break;
        }

    } while (choice != 0);
}

void saveGame(const Player& p)
{
    ofstream file("savegame.txt");

    if (!file.is_open())
    {
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

    for (int i = 0; i < EQUIP_ITEMS; i++)
    {
        file << p.inventory[i] << "\n";
    }

    for (int i = 0; i < BACKPACK_SIZE; i++)
    {
        file << p.backpack[i] << "\n";
    }

    file.close();
    cout << "Game berhasil disimpan ke 'savegame.txt'!\n";
    pause();
}

bool loadGame(Player& p)
{
    ifstream file("savegame.txt");

    if (!file.is_open())
    {
        return false;
    }

    file >> p.name >> p.hp >> p.maxHp >> p.attack >> p.coin >> p.exp >> p.level >> p.currentFloor;

    for (int i = 0; i < EQUIP_ITEMS; i++)
    {
        file >> p.inventory[i];
    }

    for (int i = 0; i < BACKPACK_SIZE; i++)
    {
        file >> p.backpack[i];
    }

    file.close();
    return true;
}

void mainMenu(Player& p)
{
    int choice;

    do
    {
        showStatus(p);
        cout << "\n=== MENU UTAMA ===\n";
        cout << "1. Grinding (Cari Monster)\n";
        cout << "2. Toko Item\n";
        cout << "3. Menu Tower (Lawan Bos)\n";
        cout << "4. Backpack (Material Loot)\n";
        cout << "5. Simpan Game\n";
        cout << "0. Keluar\n";
        cout << "> ";
        cin >> choice;

        switch (choice)
        {
            case 1: grindMenu(p);    break;
            case 2: shopMenu(p);     break;
            case 3: towerMenu(p);    break;
            case 4: backpackMenu(p); break;
            case 5: saveGame(p);     break;
            case 0: cout << "Sampai jumpa, " << p.name << "!\n"; break;
            default: cout << "Pilihan tidak valid!\n"; pause(); break;
        }

    } while (choice != 0);
}

int main()
{
    srand(time(0));
    \\ini biar angkanya acak tiap detik

    int startChoice;
    string name;

    clearScreen();
    banner();

    Player player;
    cout << "\n1. New Game\n2. Load Game\n> ";
    cin >> startChoice;

    if (startChoice == 2)
    {
        if (loadGame(player))
        {
            cout << "\nGame berhasil diload! Selamat datang kembali, " << player.name << "!\n";
            pause();
        }
        else
        {
            cout << "File save tidak ditemukan. Memulai game baru...\n";
            pause();
            startChoice = 1;
        }
    }

    if (startChoice == 1)
    {
        cout << "\nMasukkan nama karaktermu: ";
        cin >> name;
        initPlayer(player, name);
        cout << "\nSelamat datang, " << player.name << "! Petualanganmu dimulai!\n";
        pause();
    }

    mainMenu(player);
    return 0;
}