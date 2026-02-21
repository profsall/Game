#include "Story.h"
#include "Entity.h"
#include "Inventory.h"
#include "TUI.h"
#include <cstdlib>
#include <iostream>
using namespace std;

// ═══════════════════════════════════════════
//  Dialog Display Engine
// ═══════════════════════════════════════════

void showDialog(const vector<DialogLine>& lines) {
    int w = 56;
    for (const auto& dl : lines) {
        cout << endl;
        if (!dl.speaker.empty()) {
            cout << "  " << Color::BOLD << dl.color << Box::DIAMOND << " "
                 << dl.speaker << Color::RESET << endl;
        }

        cout << "  " << Color::DIM << Box::STL;
        for (int i = 0; i < w - 4; i++) cout << Box::SH;
        cout << Box::STR << Color::RESET << endl;

        cout << "  " << Color::DIM << Box::SV << Color::RESET << " ";
        typewriter(dl.text, 25, dl.color);
        int textLen = visibleLength(dl.text);
        int remaining = w - 6 - textLen;
        if (remaining > 0) cout << string(remaining, ' ');
        cout << " " << Color::DIM << Box::SV << Color::RESET << endl;

        cout << "  " << Color::DIM << Box::SBL;
        for (int i = 0; i < w - 4; i++) cout << Box::SH;
        cout << Box::SBR << Color::RESET << endl;

        sleepMs(300);
    }
}

void showNarration(const vector<string>& lines, int delayMs) {
    for (const auto& line : lines) {
        cout << "    ";
        typewriterLine(line, delayMs, Color::DIM + Color::WHITE);
        sleepMs(200);
    }
}

// ═══════════════════════════════════════════
//  Dialog Choice System (Feature #2)
// ═══════════════════════════════════════════

int showDialogChoice(const string& prompt, const vector<string>& choices) {
    cout << endl;
    int w = 50;
    drawDoubleBox(w, prompt, Color::BRIGHT_YELLOW);
    for (int i = 0; i < (int)choices.size(); i++) {
        drawBoxLine(w, Color::BRIGHT_WHITE + " [" + to_string(i + 1) + "] "
            + choices[i] + Color::RESET, Color::BRIGHT_YELLOW);
    }
    drawDoubleBoxEnd(w, Color::BRIGHT_YELLOW);

    return getChoice(1, (int)choices.size());
}

// ═══════════════════════════════════════════
//  Prologue
// ═══════════════════════════════════════════

void showPrologue() {
    clearScreen();
    cout << endl << endl;

    string c1 = Color::BRIGHT_YELLOW;
    string r  = Color::RESET;

    cout << "  " << c1 << Color::BOLD
         << centerText("P R O L O G U E", 56) << r << endl;
    cout << endl;

    cout << "  " << Color::DIM;
    for (int i = 0; i < 56; i++) cout << Box::H;
    cout << r << endl;
    cout << endl;

    vector<string> narration = {
        "Dahulu kala, di Kerajaan Arcandia yang damai...",
        "",
        "Empat menara misterius muncul dari kedalaman bumi.",
        "Dari dalam menara, gerombolan monster menyebar",
        "menghancurkan desa-desa dan menelan harapan rakyat.",
        "",
        "Kegelapan menguasai langit.",
        "Para kesatria kerajaan satu per satu gugur.",
        "Harapan nyaris padam...",
        "",
        "Hingga seorang pemuda bangun di antara reruntuhan.",
        "Dengan hanya sebuah pedang kayu tua di tangannya,",
        "ia berdiri menghadap menara-menara yang menjulang.",
        "",
        "Sumpahnya satu: menaklukkan keempat menara",
        "dan mengembalikan cahaya ke Arcandia."
    };

    showNarration(narration, 35);

    cout << endl;
    cout << "  " << Color::DIM;
    for (int i = 0; i < 56; i++) cout << Box::H;
    cout << r << endl;

    pausePrompt();
}

// ═══════════════════════════════════════════
//  Chapter Intros (with legacy call)
// ═══════════════════════════════════════════

void showChapterIntro(int chapter) {
    switch (chapter) {
    case 1:
        drawChapterTitle(1, "Hutan Gelap",
            "Di mana kegelapan pertama kali muncul...");
        break;
    case 2:
        drawChapterTitle(2, "Gua Kristal",
            "Cahaya yang menipu di kedalaman bumi...");
        break;
    case 3:
        drawChapterTitle(3, "Menara Api",
            "Api abadi yang membakar dari dalam bumi...");
        break;
    case 4:
        drawChapterTitle(4, "Kastil Kegelapan",
            "Pertempuran terakhir melawan sang Dark Lord...");
        break;
    }
}

// ═══════════════════════════════════════════
//  Chapter Intros WITH Dialog Choices (#2)
// ═══════════════════════════════════════════

void showChapterIntroWithChoices(int chapter, Player &hero) {
    showChapterIntro(chapter);

    switch (chapter) {
    case 1: {
        vector<DialogLine> dialog = {
            {"Tetua Desa", "Kesatria muda... hutan di utara adalah sumber kegelapan pertama.", Color::BRIGHT_GREEN},
            {"Tetua Desa", "Goblin King bersemayam di sana. Dia yang pertama harus dikalahkan.", Color::BRIGHT_GREEN},
        };
        showDialog(dialog);

        int choice = showDialogChoice("Bagaimana responsmu?", {
            "\"Aku siap. Akan kutaklukkan menara itu.\"",
            "\"Aku... takut. Tapi aku harus mencoba.\"",
            "\"Apa ada hal lain yang harus kuketahui?\""
        });

        if (choice == 1) {
            showDialog({{"Tetua Desa", "Semangat bagus! Terimalah ini sebagai tanda kepercayaan.", Color::BRIGHT_GREEN}});
            addItem(1, 1); // HP Potion+
            sfxItem();
            cout << "  " << Color::BRIGHT_GREEN << Box::STAR << " Mendapat HP Potion+!" << Color::RESET << endl;
        } else if (choice == 2) {
            showDialog({
                {"Tetua Desa", "Ketakutan adalah tanda kau menghargai nyawamu.", Color::BRIGHT_GREEN},
                {"Tetua Desa", "Ingat: serang saat musuh lengah. Defend saat ragu.", Color::BRIGHT_GREEN},
                {"Tetua Desa", "Dan jangan lupa, kau bisa kabur kalau keadaan terlalu berbahaya.", Color::BRIGHT_GREEN}
            });
            hero.dodgeRate += 2;
            cout << "  " << Color::BRIGHT_CYAN << "Dodge Rate +2% (pelajaran bertahan hidup)" << Color::RESET << endl;
        } else {
            showDialog({
                {"Tetua Desa", "Bijaksana... Dengarkan baik-baik.", Color::BRIGHT_GREEN},
                {"Tetua Desa", "Monster di hutan lemah terhadap serangan cepat beruntun.", Color::BRIGHT_GREEN},
                {"Tetua Desa", "Dan Goblin King punya kelemahan: dia sombong. Manfaatkan itu.", Color::BRIGHT_GREEN}
            });
            hero.critRate += 2;
            cout << "  " << Color::BRIGHT_YELLOW << "Crit Rate +2% (pengetahuan kelemahan musuh)" << Color::RESET << endl;
        }
        sleepMs(1000);
        break;
    }
    case 2: {
        vector<DialogLine> dialog = {
            {"Penjaga Gua", "Kau berhasil melewati hutan? Mengesankan.", Color::BRIGHT_MAGENTA},
            {"Penjaga Gua", "Tapi gua ini berbeda. Kristal memantulkan segalanya.", Color::BRIGHT_MAGENTA},
        };
        showDialog(dialog);

        int choice = showDialogChoice("Apa yang ingin kau tanyakan?", {
            "\"Ceritakan tentang Crystal Golem.\"",
            "\"Ada harta di dalam gua?\""
        });

        if (choice == 1) {
            showDialog({
                {"Penjaga Gua", "Crystal Golem terbuat dari kristal murni. Pertahanannya tinggi.", Color::BRIGHT_MAGENTA},
                {"Penjaga Gua", "Tapi semakin sering kau serang, kristalnya retak.", Color::BRIGHT_MAGENTA},
                {"Penjaga Gua", "Sabar dan konsisten. Itu kuncinya.", Color::BRIGHT_MAGENTA}
            });
        } else {
            showDialog({
                {"Penjaga Gua", "Haha! Seorang penjarah! Aku suka.", Color::BRIGHT_MAGENTA},
                {"Penjaga Gua", "Ada rahasia di lantai ketiga. Cari dinding yang berbeda.", Color::BRIGHT_MAGENTA}
            });
            hero.coin += 50;
            cout << "  " << Color::BRIGHT_YELLOW << "+50G (tips dari penjaga)" << Color::RESET << endl;
        }
        sleepMs(1000);
        break;
    }
    case 3: {
        vector<DialogLine> dialog = {
            {"Alkemis Tua", "Menara Api... tempat paling berbahaya di Arcandia.", Color::BRIGHT_RED},
        };
        showDialog(dialog);

        int choice = showDialogChoice("Bagaimana kau mempersiapkan diri?", {
            "\"Berikan aku ramuan tahan api!\"",
            "\"Aku akan mengandalkan kekuatanku sendiri.\""
        });

        if (choice == 1) {
            showDialog({{"Alkemis Tua", "Ini ramuan terakhirku. Gunakan dengan bijak.", Color::BRIGHT_RED}});
            addItem(14, 1); // Phoenix Feather
            sfxItem();
            cout << "  " << Color::BRIGHT_YELLOW << Box::STAR << " Mendapat Phoenix Feather!" << Color::RESET << endl;
        } else {
            showDialog({
                {"Alkemis Tua", "Kepercayaan diri yang menakjubkan...", Color::BRIGHT_RED},
                {"Alkemis Tua", "Semoga kekuatanmu cukup. Ini sedikit koin untuk perjalanan.", Color::BRIGHT_RED}
            });
            hero.coin += 100;
            cout << "  " << Color::BRIGHT_YELLOW << "+100G" << Color::RESET << endl;
        }
        sleepMs(1000);
        break;
    }
    case 4: {
        vector<DialogLine> dialog = {
            {"Arwah Kesatria", "Akhirnya... seseorang yang cukup kuat untuk sampai di sini.", Color::BRIGHT_CYAN},
            {"Arwah Kesatria", "Dark Lord menunggumu di puncak kastil.", Color::BRIGHT_CYAN},
            {"Arwah Kesatria", "Dia yang menciptakan keempat menara. Dia sumber segalanya.", Color::BRIGHT_CYAN},
        };
        showDialog(dialog);

        int choice = showDialogChoice("Kata terakhirmu sebelum masuk?", {
            "\"Aku akan mengakhiri ini.\"",
            "\"Ceritakan tentang masa lalunya.\"",
            "\"...\" (diam)"
        });

        if (choice == 1) {
            showDialog({{"Arwah Kesatria", "Tekad yang kuat. Arcandia percaya padamu.", Color::BRIGHT_CYAN}});
            hero.attack += 5;
            cout << "  " << Color::BRIGHT_RED << "ATK +5 (berkah arwah kesatria)" << Color::RESET << endl;
        } else if (choice == 2) {
            showDialog({
                {"Arwah Kesatria", "Dark Lord... dulunya dia juga seorang kesatria.", Color::BRIGHT_CYAN},
                {"Arwah Kesatria", "Dia mencari kekuatan untuk melindungi Arcandia.", Color::BRIGHT_CYAN},
                {"Arwah Kesatria", "Tapi kegelapan menelannya. Kekuasaan mengkorupsi niatnya.", Color::BRIGHT_CYAN},
                {"Arwah Kesatria", "Jangan buat kesalahan yang sama, anak muda.", Color::BRIGHT_CYAN}
            });
        } else {
            showDialog({
                {"Arwah Kesatria", "... Diammu bicara lebih keras dari kata-kata.", Color::BRIGHT_CYAN},
                {"Arwah Kesatria", "Pejuang sejati tidak butuh kata-kata.", Color::BRIGHT_CYAN}
            });
            hero.defense += 5;
            cout << "  " << Color::BRIGHT_CYAN << "DEF +5 (ketenangan batin)" << Color::RESET << endl;
        }
        sleepMs(1000);
        break;
    }
    }
    pausePrompt();
}

// ═══════════════════════════════════════════
//  Boss Pre-Battle Dialog (Feature #1)
// ═══════════════════════════════════════════

void showBossDialog(int towerNumber) {
    clearScreen();
    cout << endl;

    switch (towerNumber) {
    case 1: {
        int w = 52;
        drawDoubleBox(w, Box::SKULL + " RAJA GOBLIN " + Box::SKULL, Color::BRIGHT_RED);
        drawDoubleBoxEnd(w, Color::BRIGHT_RED);

        vector<DialogLine> dialog = {
            {"Raja Goblin", "HAHAHAHA! Seorang anak kecil datang ke rumahku!", Color::BRIGHT_RED},
            {"Raja Goblin", "Pedang kayumu itu mau dipakai untuk apa?", Color::BRIGHT_RED},
            {"Raja Goblin", "Kesatria terakhir yang datang... sudah jadi makanan anak buahku.", Color::BRIGHT_RED},
            {"Raja Goblin", "Sekarang giliranmu!", Color::BRIGHT_RED}
        };
        showDialog(dialog);
        break;
    }
    case 2: {
        int w = 52;
        drawDoubleBox(w, Box::DIAMOND + " GOLEM KRISTAL " + Box::DIAMOND, Color::BRIGHT_BLUE);
        drawDoubleBoxEnd(w, Color::BRIGHT_BLUE);

        vector<DialogLine> dialog = {
            {"Golem Kristal", "...", Color::BRIGHT_BLUE},
            {"Golem Kristal", "Ribuan... tahun... aku menjaga... gua ini...", Color::BRIGHT_BLUE},
            {"Golem Kristal", "Siapa... yang berani... mengganggu... tidurku?", Color::BRIGHT_BLUE},
            {"Golem Kristal", "Kristal... tidak bisa dihancurkan... oleh manusia...", Color::BRIGHT_BLUE}
        };
        showDialog(dialog);
        break;
    }
    case 3: {
        int w = 52;
        drawDoubleBox(w, Box::STAR + " NAGA API INFERNO " + Box::STAR, Color::BRIGHT_RED);
        drawDoubleBoxEnd(w, Color::BRIGHT_RED);

        vector<DialogLine> dialog = {
            {"Naga Api", "SERANGGA KECIL! KAU BERANI MEMANJAT MENARAKUU?!", Color::BRIGHT_RED},
            {"Naga Api", "AKU SUDAH MEMBAKAR RIBUAN KESATRIA SEBELUMMU!", Color::BRIGHT_RED},
            {"Naga Api", "APIMU BAHKAN TIDAK BISA MELELEHKAN ESMU SENDIRI!", Color::BRIGHT_RED},
            {"Naga Api", "MATI DALAM APIII!", Color::BRIGHT_RED}
        };
        showDialog(dialog);
        break;
    }
    case 4: {
        int w = 52;
        drawDoubleBox(w, Box::SKULL + " DARK LORD UMBRA " + Box::SKULL, Color::BRIGHT_MAGENTA);
        drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);

        vector<DialogLine> dialog = {
            {"Dark Lord", "...", Color::BRIGHT_MAGENTA},
            {"Dark Lord", "Kau berbeda dari yang lain.", Color::BRIGHT_MAGENTA},
            {"Dark Lord", "Mereka datang dengan amarah. Kau datang dengan... harapan?", Color::BRIGHT_MAGENTA},
            {"Dark Lord", "Pernahkah kau bertanya-tanya...", Color::BRIGHT_MAGENTA},
            {"Dark Lord", "Kenapa kau terbangun di antara reruntuhan hari itu?", Color::BRIGHT_MAGENTA},
            {"Dark Lord", "Kenapa kau satu-satunya yang selamat?", Color::BRIGHT_MAGENTA},
            {"Dark Lord", "Mungkin karena kita... tidak terlalu berbeda.", Color::BRIGHT_MAGENTA},
            {"Dark Lord", "Tapi cukup bicara. Tunjukkan padaku kekuatan 'harapan'-mu itu.", Color::BRIGHT_MAGENTA}
        };
        showDialog(dialog);
        break;
    }
    }
    pausePrompt("Tekan ENTER untuk bertarung...");
}

// ═══════════════════════════════════════════
//  Companion NPC — Lyra (Feature #3)
// ═══════════════════════════════════════════

void companionEncounter(int chapter, Player &hero) {
    clearScreen();
    cout << endl;

    switch (chapter) {
    case 1: {
        // First meeting — save her from goblins
        if (hero.companionMet) return;

        int w = 52;
        drawDoubleBox(w, Box::HEART + " PERTEMUAN TAK TERDUGA " + Box::HEART, Color::BRIGHT_MAGENTA);
        drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);

        vector<string> narration = {
            "Di antara pepohonan, kau mendengar teriakan minta tolong.",
            "Seorang gadis berpakaian pencuri terkepung goblin!",
        };
        showNarration(narration, 35);
        cout << endl;

        int choice = showDialogChoice("Apa yang kau lakukan?", {
            "Selamatkan dia!",
            "Lanjutkan perjalanan. Bukan urusanmu."
        });

        if (choice == 1) {
            showDialog({
                {"???", "T-terima kasih! Aku hampir mati di sana!", Color::BRIGHT_MAGENTA},
                {"???", "Namaku Lyra. Aku pencuri... eh, petualang tingkat pemula.", Color::BRIGHT_MAGENTA},
                {"Lyra", "Kau... mau menantang Menara Hutan sendirian?!", Color::BRIGHT_MAGENTA},
                {"Lyra", "Kau gila. Tapi kau juga berani.", Color::BRIGHT_MAGENTA},
                {"Lyra", "Ini ambilah. Aku menemukan ini di hutan tadi.", Color::BRIGHT_MAGENTA}
            });
            hero.companionMet = true;
            addItem(0, 2); // 2 HP Potions
            sfxItem();
            cout << "  " << Color::BRIGHT_GREEN << Box::STAR << " Mendapat 2x HP Potion dari Lyra!" << Color::RESET << endl;
            cout << "  " << Color::BRIGHT_MAGENTA << "Lyra akan mengingat kebaikanmu..." << Color::RESET << endl;
        } else {
            showNarration({"Kau berjalan melewati teriakan itu.", "Mungkin itu keputusan yang akan kau sesali..."}, 35);
        }
        sleepMs(1000);
        break;
    }
    case 2: {
        // Lyra appears again — shares info
        if (!hero.companionMet) return;

        int w = 52;
        drawDoubleBox(w, Box::DIAMOND + " TEMAN LAMA " + Box::DIAMOND, Color::BRIGHT_MAGENTA);
        drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);

        showDialog({
            {"Lyra", "Hey! Kita ketemu lagi!", Color::BRIGHT_MAGENTA},
            {"Lyra", "Kau masuk ke Gua Kristal juga? Aku cari harta di sini.", Color::BRIGHT_MAGENTA},
            {"Lyra", "Hati-hati, aku dengar Golem Kristal di puncak itu... mengerikan.", Color::BRIGHT_MAGENTA},
            {"Lyra", "Oh! Aku menemukan peta rahasia gua ini. Semoga berguna.", Color::BRIGHT_MAGENTA}
        });

        hero.dodgeRate += 1;
        cout << "  " << Color::BRIGHT_CYAN << "Dodge +1% (peta rahasia dari Lyra)" << Color::RESET << endl;
        sfxItem();
        sleepMs(1000);
        break;
    }
    case 3: {
        // Lyra in danger — player chooses to save or continue
        if (!hero.companionMet) return;

        int w = 52;
        drawDoubleBox(w, Box::SKULL + " LYRA DALAM BAHAYA! " + Box::SKULL, Color::BRIGHT_RED);
        drawDoubleBoxEnd(w, Color::BRIGHT_RED);

        vector<string> narration = {
            "Di tengah menara api, kau menemukan Lyra.",
            "Dia terjebak di antara lava dan monster api.",
            "Jika kau menyelamatkannya, kau akan kehilangan waktu.",
            "Boss menara mungkin menjadi lebih kuat..."
        };
        showNarration(narration, 30);

        int choice = showDialogChoice("Apa yang kau lakukan?", {
            "Selamatkan Lyra! (kehilangan 30% HP sebagai resiko)",
            "Lanjutkan ke boss. Lyra bisa jaga dirinya."
        });

        if (choice == 1) {
            hero.hp -= hero.maxHp * 30 / 100;
            if (hero.hp < 1) hero.hp = 1;
            hero.companionSaved = true;

            showDialog({
                {"Lyra", "K-kau... kau kembali untukku?", Color::BRIGHT_MAGENTA},
                {"Lyra", "*menangis* Tidak ada yang pernah kembali untukku.", Color::BRIGHT_MAGENTA},
                {"Lyra", "Aku... aku tidak akan melupakan ini. Selamanya.", Color::BRIGHT_MAGENTA},
                {"Lyra", "Berjanjilah kau akan selamat dari menara ini.", Color::BRIGHT_MAGENTA}
            });

            cout << "  " << Color::BRIGHT_RED << "-30% HP (resiko menyelamatkan)" << Color::RESET << endl;
            cout << "  " << Color::BRIGHT_MAGENTA << "Lyra akan membantumu di pertempuran terakhir..." << Color::RESET << endl;
        } else {
            showDialog({
                {"Lyra", "... Pergilah. Kalahkan boss-nya.", Color::BRIGHT_MAGENTA},
                {"Lyra", "Aku akan baik-baik saja.", Color::BRIGHT_MAGENTA}
            });
            showNarration({"Kau meninggalkan Lyra.", "Suaranya menghilang di balik api..."}, 35);
        }
        sleepMs(1000);
        break;
    }
    case 4: {
        // Lyra returns to help (if saved)
        if (!hero.companionMet || !hero.companionSaved) return;

        int w = 52;
        drawDoubleBox(w, Box::STAR + " BANTUAN TAK TERDUGA " + Box::STAR, Color::BRIGHT_MAGENTA);
        drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);

        showDialog({
            {"Lyra", "Kau pikir aku akan membiarkanmu menghadapi Dark Lord sendirian?", Color::BRIGHT_MAGENTA},
            {"Lyra", "Kau sudah menyelamatkanku dua kali.", Color::BRIGHT_MAGENTA},
            {"Lyra", "Sekarang giliranku.", Color::BRIGHT_MAGENTA},
            {"Lyra", "Aku tidak bisa bertarung... tapi aku bisa ini.", Color::BRIGHT_MAGENTA}
        });

        hero.companionHelped = true;
        hero.hp = hero.maxHp;
        hero.mp = hero.maxMp;
        hero.attack += 10;
        hero.defense += 5;
        sfxHeal();

        cout << "  " << Color::BRIGHT_GREEN << Box::SPARKLE << " Lyra memulihkan HP dan MP-mu!" << Color::RESET << endl;
        cout << "  " << Color::BRIGHT_RED << "+10 ATK " << Color::BRIGHT_CYAN << "+5 DEF "
             << Color::BRIGHT_MAGENTA << "(kekuatan persahabatan)" << Color::RESET << endl;

        showDialog({{"Lyra", "Sekarang pergilah. Akhiri segalanya. Aku percaya padamu.", Color::BRIGHT_MAGENTA}});

        sleepMs(1000);
        break;
    }
    }
}

// ═══════════════════════════════════════════
//  Mid-Tower Story Beats (Feature #6)
// ═══════════════════════════════════════════

void showMidTowerBeat(int towerNumber, int floor) {
    clearScreen();
    cout << endl;

    switch (towerNumber) {
    case 1: {
        if (floor == 3) {
            int w = 50;
            drawDoubleBox(w, Box::SKULL + " PENEMUAN " + Box::SKULL, Color::BRIGHT_RED);
            drawDoubleBoxEnd(w, Color::BRIGHT_RED);

            vector<string> narration = {
                "Kau menemukan tulang-belulang seorang kesatria.",
                "Di tangannya, sebuah jurnal usang...",
                "",
                "\"Hari ke-7. Monster semakin banyak.\"",
                "\"Hari ke-12. Teman-temanku sudah habis.\"",
                "\"Hari ke-15. Aku tidak bisa mundur lagi.\"",
                "\"Siapapun yang membaca ini... jangan buat kesalahanku.\"",
                "\"Jangan pernah maju tanpa persiapan.\"",
                "",
                "Tulisan itu berhenti di situ."
            };
            showNarration(narration, 35);
        } else if (floor == 4) {
            showDialog({
                {"Suara Misterius", "Kau semakin dekat dengan raja mereka...", Color::BRIGHT_GREEN},
                {"Suara Misterius", "Hutan ini mengujimu. Apakah kau layak?", Color::BRIGHT_GREEN}
            });
        }
        break;
    }
    case 2: {
        if (floor == 3) {
            vector<string> narration = {
                "Kristal-kristal di dinding bercahaya aneh.",
                "Kau melihat bayanganmu di kristal... tapi bukan dirimu.",
                "Bayangan itu lebih tua. Lebih kuat. Lebih gelap.",
                "Sebelum kau bisa melihat lebih jelas, bayangan itu hilang.",
                "",
                "...Apa itu?"
            };
            showNarration(narration, 35);
        } else if (floor == 4) {
            showDialog({
                {"Suara Kristal", "Semakin dalam kau masuk...", Color::BRIGHT_BLUE},
                {"Suara Kristal", "Semakin banyak yang terungkap.", Color::BRIGHT_BLUE},
                {"Suara Kristal", "Golem di atas sana menyimpan rahasia besar.", Color::BRIGHT_BLUE}
            });
        }
        break;
    }
    case 3: {
        if (floor == 3) {
            vector<string> narration = {
                "Di antara lava, kau menemukan fragmen tulisan kuno.",
                "",
                "\"Empat menara. Empat segel.\"",
                "\"Dibuat oleh seorang manusia yang ingin jadi dewa.\"",
                "\"Dia yang menciptakan... adalah dia yang harus dihancurkan.\"",
                "",
                "Tulisan itu terbakar habis oleh lava sebelum kau selesai membaca."
            };
            showNarration(narration, 35);
        } else if (floor == 4) {
            showDialog({
                {"Suara Api", "Kau hampir di puncak, manusia...", Color::BRIGHT_RED},
                {"Suara Api", "Naga Api Inferno tidak akan sengah sepertiku.", Color::BRIGHT_RED},
                {"Suara Api", "Bersiaplah untuk bertemu akhirmu.", Color::BRIGHT_RED}
            });
        }
        break;
    }
    case 4: {
        if (floor == 3) {
            int w = 50;
            drawDoubleBox(w, "RUANG KENANGAN", Color::BRIGHT_MAGENTA);
            drawDoubleBoxEnd(w, Color::BRIGHT_MAGENTA);

            vector<string> narration = {
                "Ruangan ini dipenuhi gambaran masa lalu.",
                "Seorang kesatria muda berlutut di depan altar.",
                "\"Berikan aku kekuatan untuk melindungi rakyatku.\"",
                "",
                "Altar itu bersinar gelap.",
                "\"Kekuatan yang kau minta... ada harganya.\"",
                "",
                "Kau mengenali wajah kesatria itu.",
                "Itu wajah Dark Lord... sebelum kegelapan menelannya."
            };
            showNarration(narration, 40);
        } else if (floor == 4) {
            showDialog({
                {"Suara Kegelapan", "Satu lantai lagi, kesatria.", Color::BRIGHT_MAGENTA},
                {"Suara Kegelapan", "Kau sudah tahu siapa aku sebenarnya, bukan?", Color::BRIGHT_MAGENTA},
                {"Suara Kegelapan", "Pertanyaannya... apa kau akan membuat pilihan yang berbeda?", Color::BRIGHT_MAGENTA}
            });
        }
        break;
    }
    }
    pausePrompt();
}

// ═══════════════════════════════════════════
//  Flashback / Memory Fragment (Feature #5)
// ═══════════════════════════════════════════

void showFlashback() {
    clearScreen();
    cout << endl;

    int w = 50;
    string c = Color::BRIGHT_MAGENTA;
    string r = Color::RESET;

    drawDoubleBox(w, Box::SPARKLE + " FRAGMEN INGATAN " + Box::SPARKLE, c);
    drawDoubleBoxEnd(w, c);
    cout << endl;

    int fragment = rand() % 6;

    switch (fragment) {
    case 0: {
        showNarration({
            "Sebuah ingatan samar melintas di pikiranmu...",
            "",
            "Kau kecil. Mungkin 5 atau 6 tahun.",
            "Seseorang menggendongmu di punggungnya.",
            "\"Jangan takut,\" bisiknya. \"Ayah akan melindungimu.\"",
            "",
            "Wajahnya kabur. Kau tidak bisa mengingatnya.",
            "Tapi kehangatan punggungnya... kau tidak akan lupa."
        }, 40);
        break;
    }
    case 1: {
        showNarration({
            "Tiba-tiba kepalamu berdenyut...",
            "",
            "Api. Reruntuhan. Jeritan.",
            "Kau berdiri di antara bangunan yang runtuh.",
            "Di tanganmu, sebuah pedang kayu. Bukan milikmu.",
            "Seseorang menaruhnya di tanganmu sebelum...",
            "",
            "Sebelum apa? Ingatannya berhenti di situ."
        }, 40);
        break;
    }
    case 2: {
        showNarration({
            "Fragmen ingatan muncul tiba-tiba...",
            "",
            "\"Kau harus lari!\" teriak seorang wanita.",
            "\"Jangan melihat ke belakang!\"",
            "",
            "Kau berlari. Hutan gelap di sekelilingmu.",
            "Suara ledakan di belakang. Cahaya gelap menelan segalanya.",
            "",
            "Lalu... kegelapan. Dan kau terbangun di reruntuhan."
        }, 40);
        break;
    }
    case 3: {
        showNarration({
            "Sebuah mimpi... atau ingatan?",
            "",
            "Sebuah kerajaan yang indah. Langit biru cerah.",
            "Orang-orang tersenyum di jalanan.",
            "Seorang ksatria berdiri di balkon istana.",
            "Semua orang menghormatinya.",
            "",
            "Lalu bayangan muncul di bawah kakinya.",
            "Dan senyum itu... berubah menjadi sesuatu yang lain."
        }, 40);
        break;
    }
    case 4: {
        showNarration({
            "Kepalamu terasa berat...",
            "",
            "\"Kekuatan itu bukan milik manusia,\" kata sebuah suara.",
            "\"Kau akan kehilangan segalanya jika kau menggunakannya.\"",
            "",
            "\"Aku tidak peduli. Selama rakyatku selamat.\"",
            "",
            "Dua suara itu... milik siapa?",
            "Mengapa terdengar begitu familiar?"
        }, 40);
        break;
    }
    case 5: {
        showNarration({
            "Sebuah kilatan ingatan...",
            "",
            "Kau melihat dirimu sendiri. Lebih tua. Lebih kuat.",
            "Berdiri di puncak menara yang sangat tinggi.",
            "Di bawahmu, kegelapan menelan dunia.",
            "",
            "\"Ini yang kau inginkan?\" tanya bayangan itu.",
            "\"Tidak,\" jawabmu. \"Ini yang harus kulakukan.\"",
            "",
            "...Masa depan? Atau masa lalu yang terlupakan?"
        }, 40);
        break;
    }
    }

    cout << endl;
    cout << "  " << Color::DIM << "Ingatan itu memudar..." << Color::RESET << endl;
    pausePrompt();
}

// ═══════════════════════════════════════════
//  Chapter Outros
// ═══════════════════════════════════════════

void showChapterOutro(int chapter) {
    clearScreen();
    switch (chapter) {
    case 1: {
        cout << endl;
        vector<string> narration = {
            "Goblin King telah dikalahkan.",
            "Hutan perlahan kembali tenang...",
            "Tapi kegelapan masih menyelimuti Arcandia.",
            "",
            "Perjalanan masih panjang.",
            "Gua Kristal menunggumu di timur..."
        };
        showNarration(narration, 40);
        break;
    }
    case 2: {
        cout << endl;
        vector<string> narration = {
            "Crystal Golem hancur berkeping-keping.",
            "Kristal-kristal di gua mulai meredup...",
            "",
            "Dua menara telah jatuh.",
            "Kekuatan kegelapan mulai goyah.",
            "Menara Api di selatan memanggilmu..."
        };
        showNarration(narration, 40);
        break;
    }
    case 3: {
        cout << endl;
        vector<string> narration = {
            "Fire Dragon jatuh dengan raungan terakhir.",
            "Api abadi di menara perlahan padam...",
            "",
            "Tiga menara telah ditaklukkan.",
            "Hanya tersisa satu: Kastil Kegelapan.",
            "Dark Lord menunggumu. Ini pertempuran terakhir."
        };
        showNarration(narration, 40);
        break;
    }
    case 4:
        break; // ending handles this
    }
    if (chapter < 4) {
        pausePrompt();
    }
}

// ═══════════════════════════════════════════
//  Multiple Endings (Feature #4)
// ═══════════════════════════════════════════

void showEnding(Player &hero) {
    clearScreen();
    cout << endl << endl << endl;

    string c1 = Color::BRIGHT_YELLOW;
    string c2 = Color::BRIGHT_CYAN;
    string c3 = Color::WHITE;
    string r  = Color::RESET;

    cout << "  " << c1 << Color::BOLD;
    for (int i = 0; i < 56; i++) cout << Box::H;
    cout << r << endl;
    cout << endl;

    cout << "  " << c1 << Color::BOLD
         << centerText(Box::STAR + " " + Box::STAR + " " + Box::STAR + "  F I N A L E  " + Box::STAR + " " + Box::STAR + " " + Box::STAR, 56)
         << r << endl;
    cout << endl;

    cout << "  " << c1 << Color::BOLD;
    for (int i = 0; i < 56; i++) cout << Box::H;
    cout << r << endl;
    cout << endl;

    sleepMs(2000);

    // Common narration
    vector<string> commonNarration = {
        "Dark Lord telah jatuh.",
        "",
        "Keempat menara runtuh satu per satu,",
        "mengembalikan cahaya ke tanah Arcandia.",
        "",
        "Langit yang gelap perlahan cerah.",
        "Bunga-bunga bermekaran di padang yang dulu tandus.",
        "Rakyat keluar dari persembunyian mereka...",
    };
    showNarration(commonNarration, 45);
    cout << endl;
    sleepMs(1000);

    // Determine ending
    // Ending C (Shadow) — collected all lore items
    extern int loreCount;
    bool allLore = (loreCount >= 8);

    // Ending A (Hero) — low kills, high quests, saved companion
    bool isHero = (hero.killCount < 50 && hero.questsCompleted >= 5 && hero.companionSaved);

    // Ending B (Warrior) — high kills
    // Default ending

    if (allLore) {
        // ── ENDING C: THE SHADOW ──
        cout << endl;
        cout << "  " << Color::BRIGHT_MAGENTA << Color::BOLD
             << centerText("- ENDING C: THE SHADOW -", 56) << r << endl;
        cout << endl;

        vector<string> endingC = {
            "Dengan semua artefak terkumpul, kebenaran terungkap.",
            "",
            "Dark Lord... adalah dirimu.",
            "Dari masa depan yang tak pernah terjadi.",
            "",
            "Dia yang mencari kekuatan untuk melindungi Arcandia.",
            "Dia yang tertelan kegelapan.",
            "Dia yang membangun menara-menara ini.",
            "Dia yang mengirim dirinya sendiri kembali ke masa lalu.",
            "",
            "\"Kalahkan aku,\" bisiknya sebelum hancur.",
            "\"Kalahkan aku agar kau tidak menjadi aku.\"",
            "",
            "Kau berdiri di puncak reruntuhan.",
            "Sekarang kau tahu mengapa kau terbangun di antara reruntuhan.",
            "Sekarang kau tahu mengapa hanya kau yang bisa menghancurkan menara.",
            "",
            "Karena menara itu... milikmu.",
            "",
            "Tapi kau memilih jalan yang berbeda.",
            "Kau memilih cahaya."
        };
        showNarration(endingC, 50);

    } else if (isHero) {
        // ── ENDING A: THE HERO ──
        cout << endl;
        cout << "  " << Color::BRIGHT_GREEN << Color::BOLD
             << centerText("- ENDING A: THE HERO -", 56) << r << endl;
        cout << endl;

        vector<string> endingA = {
            "Di tengah reruntuhan kastil kegelapan,",
            "berdiri seorang kesatria dengan hati yang lembut.",
            "",
        };
        showNarration(endingA, 45);

        if (hero.companionSaved) {
            vector<string> lyraEnding = {
                "Lyra berlari ke arahmu.",
                "\"Kau melakukannya!\" teriaknya sambil menangis.",
                "\"Aku tahu kau bisa.\"",
                ""
            };
            showNarration(lyraEnding, 45);
        }

        vector<string> endingA2 = {
            "Rakyat Arcandia menyambutmu sebagai pahlawan.",
            "Bukan karena kekuatanmu yang besar.",
            "Tapi karena hatimu yang tidak pernah berhenti peduli.",
            "",
            "Kau ditawarkan takhta.",
            "Dan kau menerimanya. Bukan untuk kekuasaan.",
            "Tapi untuk melindungi mereka yang tidak bisa melindungi diri.",
            "",
            "Raja " + hero.name + ".",
            "Sang Pahlawan Berhati Emas."
        };
        showNarration(endingA2, 45);

    } else {
        // ── ENDING B: THE WARRIOR ──
        cout << endl;
        cout << "  " << Color::BRIGHT_RED << Color::BOLD
             << centerText("- ENDING B: THE WARRIOR -", 56) << r << endl;
        cout << endl;

        vector<string> endingB = {
            "Di tengah reruntuhan kastil kegelapan,",
            "berdiri seorang pejuang dengan pedang berlumur darah.",
            "",
            "Kau tidak menghitung berapa yang telah kau kalahkan.",
            to_string(hero.killCount) + " monster telah jatuh oleh pedangmu.",
            "",
            "Rakyat menyambutmu dengan sorak sorai.",
            "Tapi dalam sorak itu, kau melihat sedikit ketakutan.",
            "Mereka tahu kekuatanmu. Dan kekuatan selalu ditakuti.",
            "",
            "Kau berjalan melewati kerumunan.",
            "Tidak butuh takhta. Tidak butuh gelar.",
            "",
            "Legenda mengatakan " + hero.name + " masih berkelana.",
            "Mencari pertempuran yang belum dimenangkan.",
            "Mencari lawan yang belum dikalahkan.",
            "",
            hero.name + " sang Legenda Perang."
        };
        showNarration(endingB, 45);
    }

    cout << endl;
    sleepMs(1000);

    // Stats recap
    cout << "  " << c1 << Color::BOLD;
    for (int i = 0; i < 56; i++) cout << Box::H;
    cout << r << endl;
    cout << endl;

    cout << "  " << c2 << Color::BOLD << centerText("S T A T I S T I K", 56) << r << endl;
    cout << endl;

    int w = 50;
    drawDoubleBox(w, "REKAP PERJALANAN", c2);
    drawBoxLine(w, Color::WHITE + " Nama      : " + Color::BRIGHT_WHITE + hero.name + Color::RESET, c2);
    drawBoxLine(w, Color::WHITE + " Level     : " + Color::BRIGHT_GREEN + to_string(hero.level) + Color::RESET, c2);
    drawBoxLine(w, Color::WHITE + " Kill Count: " + Color::BRIGHT_RED + to_string(hero.killCount) + Color::RESET, c2);
    drawBoxLine(w, Color::WHITE + " Gold      : " + Color::BRIGHT_YELLOW + to_string(hero.coin) + "G" + Color::RESET, c2);
    drawBoxLine(w, Color::WHITE + " Pedang    : " + Color::BRIGHT_CYAN + getSwordName(hero.swordLevel) + Color::RESET, c2);

    string companionStatus = hero.companionMet ? (hero.companionSaved ? "Diselamatkan" : "Bertemu") : "Tidak bertemu";
    drawBoxLine(w, Color::WHITE + " Lyra      : " + Color::BRIGHT_MAGENTA + companionStatus + Color::RESET, c2);

    drawSeparator(w, c2);

    string endingName;
    if (allLore) endingName = "C: The Shadow";
    else if (isHero) endingName = "A: The Hero";
    else endingName = "B: The Warrior";
    drawBoxLine(w, Color::WHITE + " Ending    : " + Color::BRIGHT_YELLOW + endingName + Color::RESET, c2);

    drawDoubleBoxEnd(w, c2);
    cout << endl;

    // Credits
    cout << "  " << c1 << Color::BOLD;
    for (int i = 0; i < 56; i++) cout << Box::H;
    cout << r << endl;
    cout << endl;

    cout << "  " << c2 << Color::BOLD
         << centerText("T E R I M A   K A S I H", 56) << r << endl;
    cout << "  " << c3 << Color::DIM
         << centerText("Arcandia: Shadows of the Four Towers", 56) << r << endl;
    cout << endl;

    cout << "  " << c1 << Color::BOLD;
    for (int i = 0; i < 56; i++) cout << Box::H;
    cout << r << endl;

    pausePrompt("Tekan ENTER untuk kembali ke menu utama...");
}
