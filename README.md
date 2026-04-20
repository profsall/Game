Oke iqq, ini dokumentasi lengkap buat kode game RPG Tower yang udah kita bahas. Gue tulis pake gaya laporan mahasiswa tingkat akhir biar kelihatan pro tapi tetep santai pas dibaca. Formatnya udah gue sesuaikan jadi `README.md` biar lu tinggal copy-paste aja ke repo GitHub lu.

---

# Dokumentasi Proyek: ASCII RPG Tower Game

Yo iqq! Jadi ini adalah dokumentasi teknis buat game RPG berbasis CLI (Command Line Interface) yang kita kerjain. Game ini fokus sama sistem pendakian tower, grinding level, dan manajemen resource (coin & inventory). 

## 1. Ikhtisar (Overview)
Game ini dibuat pake C++ standar dengan pendekatan prosedural. Inti permainannya adalah pemain harus naik sampai lantai 4, tapi tiap lantai punya syarat level tertentu dan harus ngalahin bos dulu. Ada sistem *permadeath* ringan di mana kalau HP habis, pemain nggak langsung game over tapi harus balik ke toko buat beli ramuan.

## 2. Struktur Data Utama
Di sini kita pakai dua struct utama buat nampung data supaya rapi:

* **Struct `Item`**: Digunakan buat nyimpen data barang di toko. Isinya ada nama (char array), harga, bonus attack, dan bonus HP.
* **Struct `Player`**: Ini jantungnya data pemain. Isinya variabel buat nyimpen status kayak HP, Max HP, ATK, Coin, EXP, Level, lantai sekarang, sama array `inventory` buat nyimpen index item yang dibeli.

## 3. Database & Konstanta
Gue udah set beberapa limit biar game-nya nggak nge-bug:
* `MAX_INVENTORY`: Pemain cuma bisa bawa 5 item.
* `MAX_ITEMS`: Ada 10 jenis item unik yang tersedia di toko, mulai dari Pedang Kayu sampe Mahkota Raja.
* `MAX_FLOORS`: Total ada 4 lantai yang bisa dijelajahi.

## 4. Penjelasan Fungsi (Breakdown Kode)

### A. Fungsi Utilitas & Status
* `getTotalAtkBonus(const Player& p)`: Fungsi ini bakal nge-loop isi inventory pemain, terus ngejumlahin semua bonus ATK dari item yang dipunya.
* `getEffectiveAtk(const Player& p)`: Ini buat ngitung total serangan asli (stat dasar + bonus item).
* `showStatus(const Player& p)`: Fungsi buat nampilin dashboard pemain. Di sini semua statistik sama isi tas lu bakal kelihatan jelas.

### B. Sistem Leveling
* `checkLevelUp(Player& p)`: Logikanya sederhana tapi efektif. Tiap kali EXP pemain nembus ambang batas (`level * 50`), pemain naik level. Pas naik level, ATK nambah 5, Max HP nambah 20, dan HP langsung penuh lagi.

### C. Mekanisme Pertarungan (Battle System)
* `startBattle(Player& p, bool isBoss, int floor)`: Ini fungsi yang paling kompleks. Dia bisa handle monster biasa atau bos. 
    * Statistik musuh bakal di-generate acak (scaled) berdasarkan lantai.
    * Sistemnya *turn-based* sederhana. Lu mukul, musuh bales.
    * Kalau menang dapet EXP dan Coin. Kalau kalah, HP diset jadi 1 biar lu dipaksa ke toko.

### D. Toko & Ekonomi
* `shopMenu(Player& p)`: Di sini lu bisa beli item, jual item (dapet setengah harga), atau beli ramuan HP. 
    * Pas beli item, bonus HP-nya langsung nambahin Max HP pemain secara permanen selama item itu ada di tas.
    * Pas dijual, Max HP pemain bakal dikurangin lagi biar adil.

### E. Sistem Tower
* `towerMenu(Player& p)`: Ini buat kontrol perpindahan lantai. 
    * Ada syarat level: Lantai 2 (Level 3), Lantai 3 (Level 6), dan Lantai 4 (Level 10).
    * Mau naik lantai? Harus lawan Bos dulu. Kalau kalah, ya tetep di lantai lama.

### F. Save & Load (Persistence)
* `saveGame(const Player& p)`: Semua data pemain ditulis ke file `savegame.txt` baris demi baris.
* `loadGame(Player& p)`: Ngebaca data dari file tadi buat dimasukin lagi ke struct `Player`. Kalau filenya nggak ada, game bakal otomatis mulai baru.

## 5. Analisis File Save Terakhir
Berdasarkan file `savegame.txt` yang ada, status terakhir karakter adalah:
* **Nama**: w
* **Status**: Level 7 di Lantai 3.
* **Kesehatan**: HP 217 dari total 300.
* **Kekuatan**: ATK dasar 40.
* **Ekonomi**: 81 Coin dan 319 EXP.
* **Inventory**: Di slot awal ada item index 2 (Pedang Besi), index 5 (Jubah Naga), dan index 4 (Pedang Mistik).

---

Semoga dokumentasi ini ngebantu lu buat jelasin tugas atau proyek ini ya broo. Kalau ada bagian logika yang mau diubah, kabari aja!
