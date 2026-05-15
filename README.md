# Hangman - SFML
A classic Hangman word-guessing game developed in C++ using the SFML (Simple and Fast Multimedia Library). Features include three difficulty levels, a category-based hint system, a countdown timer, save/resume functionality, and a persistent top-5 leaderboard.

## Compilation & Setup

### 1. Prerequisites
- **IDE:** Visual Studio 2019 or 2022
- **Library:** SFML 3.x (Ensure the version matches your compiler, e.g., Visual C++ 17 64-bit)

### 2. Project Configuration
1. **Open the Solution:** Launch Visual Studio and open the `.sln` or `.vcxproj` file.
2. **Include Directories:**
   - Right-click Project > Properties > C/C++ > General.
   - Add the SFML `include` folder path to Additional Include Directories.
3. **Library Directories:**
   - Go to Linker > General.
   - Add the SFML `lib` folder path to Additional Library Directories.
4. **Linker Dependencies:**
   - Go to Linker > Input.
   - Add the following to Additional Dependencies:
      sfml-graphics.lib
      sfml-window.lib
      sfml-system.lib
      sfml-audio.lib
      
### 3. Asset Management
The application requires resources in the same directory as the executable. Ensure the following are present:

**Fonts**
- `impact.ttf`, `times.ttf`, `arialbd.ttf`

**Images**
- `start1.png` — Main menu background
- `Color.jpg` — Game screen background
- `h1.png` through `h7.png` — Hangman stage sprites
- `lampfinal.png` — Hint / Pause / Save button icon
- `num.png` — Word index popup background
- `words.png` — Words guessed summary background
- `new.png` — Quit/Resume window background
- `picfi.png`, `mutefi.png` — Sound on/off icons

**Audio**
- `hangs.wav` — Background music
- `click.wav` — Button click sound effect

**Word Lists**
- `Easy.txt`, `Medium.txt`, `Hard.txt` — Category-word files (format: `Category:word1,word2,word3`)

**Save & Leaderboard Data** *(auto-generated at runtime)*
- `savegame.txt` — Mid-game save state
- `saveprogress.txt` — Between-word progress save
- `leaderboard.txt` — Persistent top-5 scores

### 4. Running the Game
1. Set the solution configuration to **x64**.
2. Copy the `.dll` files from your SFML `bin` folder into the output directory (e.g., `x64/Debug`).
3. Press **F5** to compile and launch.

---

## How to Play

- **Main Menu:** Choose Play to select a difficulty, view Instructions, or exit.
- **Guessing:** Type a letter on your keyboard to guess. Correct letters are revealed; wrong ones are recorded and advance the hangman drawing.
- **Timer:** Each word has a countdown timer bar (green → yellow → red). Running out of time counts as a loss.
- **Hint:** Click the lamp icon on the left to reveal the word's category. Each hint can only be used once per word.
- **Pause:** Click the lamp icon on the right to pause and choose to Resume or Restart.
- **Save:** Click the Save button at any point mid-game to save your exact state and quit. Your progress will be offered for resumption on next launch.
- **Win/Lose:** Correctly guessing the full word opens the Win screen. Six wrong guesses or a timeout opens the Lose screen.

---

## Difficulty Levels

| Level  | Timer Per Word | Word File     |
|--------|---------------|---------------|
| Easy   | 60 seconds    | `Easy.txt`    |
| Medium | 50 seconds    | `Medium.txt`  |
| Hard   | 40 seconds    | `Hard.txt`    |

---

## Technical Highlights

- **Modular Architecture:** Functionality is split across dedicated modules — `HangmanGame`, `WordManager`, `SaveGame`, `Leaderboard`, `Windows`, and `Audio` — each with its own header and implementation file.
- **Save System:** Two save types are supported: a full mid-game save (`savegame.txt`) capturing the exact word state, timer, wrong letters, and hint status; and a lighter progress save (`saveprogress.txt`) used between words after a win.
- **Word Loading:** Words are parsed from plain-text files in `Category:word1,word2` format. Unused-word tracking ensures no word repeats until the entire list is exhausted.
- **Leaderboard:** Scores for all three difficulty levels are stored per player name. The top 5 players by score are persisted to `leaderboard.txt` and displayed in-game.
- **Timer Bar:** A 30-segment colour-coded progress bar (green/yellow/red) provides a live visual countdown using `sf::Clock`.
- **Audio:** Background music loops via `sf::Sound`. A separate click sound plays on every button interaction. Both are loaded once via `initSounds()` and shared globally.
- **C-style Strings Only:** All string handling uses `char` arrays and manual character-by-character operations — no `std::string` — consistent with course constraints.

---

## Limitations

1. **Fixed Resolution:** The UI is laid out for a 1200×800 window; resizing is disabled via `sf::Style::Close | sf::Style::Titlebar`.
2. **Asset Dependency:** All image, font, and audio files must be in the same directory as the executable or the game will fail to launch.
3. **Single Save Slot:** Only one game can be saved at a time. Starting a new game while a save exists will prompt the player to resume or overwrite.
4. **Word File Format:** Word list files must follow the `Category:word1,word2` format exactly; malformed lines are silently skipped.
