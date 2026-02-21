# Arcandia: Shadows of the Four Towers

A story-driven RPG built in C++ with a terminal-based UI (TUI) featuring ANSI colors, Unicode box-drawing, and animated text.

## Features

### Core Systems
- **Turn-based combat** with attack, defend, skill, item, and escape options
- **Critical hits and dodge** mechanics with animated banners
- **Skill system** featuring 10 skills across attack, heal, and buff categories (mana-based)
- **Inventory system** with 20+ items: consumables, armor, accessories, scrolls, and bombs
- **Equipment** with auto-equip, defense bonuses, and stat modifiers

### World and Story
- **4 towers** with unique themes (Forest, Crystal Cave, Fire, Shadow), each with 5 floors
- **Story chapters** with typewriter-style dialogue and cinematic cutscenes
- **10 side quests** given by NPCs, with kill tracking and reward claiming
- **Random tower events**: treasure chests, traps, rest spots, wandering merchants, lore artifacts
- **Bestiary** that auto-logs encountered monsters with stats and kill counts
- **8 lore collectibles** scattered throughout the towers

### Quality of Life
- **3 save slots** with full state serialization (inventory, skills, quests, bestiary, lore)
- **Difficulty selection**: Easy, Normal, Hard (affects enemy stats and rewards)
- **Sound effects** using Windows Beep (toggleable)
- **Tower minimap** showing floor progress
- **Configurable settings** for text speed, difficulty, and sound (saved to file)

## Requirements

- **Compiler**: g++ with C++17 support
- **OS**: Windows (uses `windows.h`, `conio.h`, and `Beep()`)
- **Terminal**: Windows Terminal recommended for full ANSI and Unicode support

## Build

```bash
make            # Build the game
make clean      # Remove build artifacts
make rebuild    # Clean + build
make run        # Build and run
```

The executable is output to `bin/game.exe`.

## Project Structure

```
Game/
├── include/         # Header files
│   ├── Bestiary.h
│   ├── Config.h
│   ├── Entity.h
│   ├── Filesystem.h
│   ├── Inventory.h
│   ├── Quest.h
│   ├── Shop.h
│   ├── Skills.h
│   ├── Story.h
│   ├── Tower.h
│   └── TUI.h
├── src/             # Source files
│   ├── Main.cpp
│   ├── Entity.cpp
│   ├── Tower.cpp
│   ├── Shop.cpp
│   ├── Filesystem.cpp
│   ├── TUI.cpp
│   ├── Story.cpp
│   ├── Config.cpp
│   ├── Inventory.cpp
│   ├── Skills.cpp
│   ├── Quest.cpp
│   └── Bestiary.cpp
├── build/           # Object files (generated)
├── bin/             # Executable output (generated)
├── Makefile
├── LICENSE
└── .gitignore
```

## Module Overview

| Module     | Description                                          |
|------------|------------------------------------------------------|
| Entity     | Player and enemy structs, stats, level-up, crit/dodge |
| Tower      | Tower progression, battle loop, random events         |
| Shop       | Sword upgrades, item shop, equipment shop             |
| Inventory  | Item database, equip system, lore collectibles        |
| Skills     | Skill database, mana cost, battle skill menu          |
| Quest      | Quest templates, tracking, NPC board, rewards         |
| Bestiary   | Monster encyclopedia, encounter/kill logging          |
| Config     | Game settings, difficulty multipliers, persistence    |
| Filesystem | Save/load with 3 slots, full state serialization      |
| Story      | Prologue, chapter intros/outros, ending, dialogue     |
| TUI        | ANSI rendering, box-drawing, animations, sound FX     |

## License

See [LICENSE](LICENSE) for details.
