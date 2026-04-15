# DEMOGORGON — Stranger Things Inspired 2D Game (iGraphics / C++)

A 2D action game inspired by *Stranger Things*, built in **C/C++** using **iGraphics (OpenGL/GLUT)** on **Windows**.  
Choose **Steve** or **Eleven**, survive enemy waves, collect pickups (bullets/shield), and defeat the boss.

---

## Features

### Core Gameplay
- **Character selection**: Steve / Eleven
- **Side-scrolling world** with camera follow
- **Jump system** with multi-phase jump sprites (pre-jump → up → peak → down → land)
- **Shooting** with projectile effects (fire/water style depending on character)
- **Enemy AI**: enemies target the player and avoid clustering with a simple “wait/push” behavior
- **Boss fight**:
  - Triggers at a score threshold (`BOSS_TRIGGER_SCORE`)
  - Switches to **boss arena background** (`bossarea.png`)
  - Switches to **boss fight music** (`bossFight.mp3`)
  - Boss shoots projectiles at intervals and reacts to damage

### Pickups (replaces coins)
- **Bullet pickup** (`bullet.png`): grants additional bullets (amount depends on drop logic).
- **Shield pickup** (`shield.png`): gives **invulnerability (no damage)** for **10 seconds**.
- On-screen messages:
  - `Bullets gained: <n>`
  - `Shield gained for 10 sec`

### UI
- Health bar
- Score display
- Bullet counter with a **fire icon** (`fire.png`) at bottom-right
- Enemy count display

### High Score System
- Top 10 scores saved to `highscores.dat` (binary file)
- Loads automatically on startup / high-score page

---

## Controls

### Keyboard
- **A / D**: Move left / right
- **Space**: Jump
- **F**: Shoot (consumes bullets)
- **P**: Pause / Resume
- **ESC**: Return to main menu from game

### Mouse
- Use mouse to click menu buttons:
  - Start Game, Instructions, High Scores, Credits, Exit, Back, etc.

---

## Game Flow
1. Start Page → Character Select
2. Choose **Steve** or **Eleven**
3. Fight regular enemies and collect pickups
4. Boss appears once score reaches `BOSS_TRIGGER_SCORE`
5. Background + music switch to boss arena
6. Defeat boss to finish / game over when HP reaches 0

---

## Assets Required (Photos folder)

Make sure these exist inside your `Photos/` folder (names must match):

### Backgrounds / UI
- `Homepage.png`
- `menu.png`
- `gamebg.png`
- `bossarea.png`
- `CharacterChoose.png`
- `health.png`
- `fire.png`
- `bullet.png`
- `shield.png`

### Audio
- `bg.mp3`
- `Ingame.mp3`
- `bossFight.mp3`

### Steve Sprites (examples)
- `steve1.png` (idle)
- `steve2.png`, `steve4.png` (run)
- `steve3.png` (shoot)
- `steveprejump.png`, `stevejump1.png`, `stevejump2.png`, `stevejump3.png`, `steveland.png`

### Eleven Sprites (examples)
- `eleven1.png` (idle)
- `eleven3.png`, `eleven4.png` (run)
- `elevenattack.png`, `elevenattack1.png` (shoot)
- `elevenprejump.png`, `elevenprejump1.png`, `elevenjump1.png`, `elevenjump2.png`, `elevenjump3.png`, `elevenland.png`

### Enemy + Boss Sprites
- Enemy demogorgon sprites (`DeMorgon...png`)
- Boss sprites (`bossidle.png`, `bossattack1.png`, etc.)

---

## Build / Run (Windows)

### Requirements
- Windows (tested with Visual Studio style setup)
- OpenGL + GLUT environment (included/used by iGraphics)
- `winmm.lib` (used for MCI audio playback)
- iGraphics source files (including `iGraphics.h` and implementation)
- `stb_image.h` (used by iGraphics image loader)

### Notes
- The project uses:
  ```cpp
  #pragma comment(lib, "winmm.lib")
for audio.

Steps
Open the project/solution in Visual Studio.
Ensure Photos/ folder is in the working directory (same level as the .exe or configured working dir).
Build and run.
Common Issues / Fixes
LNK1104: cannot open ... .exe (Access is denied)
The game .exe is still running or locked.
Fix:
Stop debugging (Shift+F5)
Task Manager → end DemmoGorgon.exe
Clean & Rebuild
If needed, move project out of Downloads/ and/or add antivirus exclusion.
Missing images/audio
If a texture/audio doesn’t load, check:
Exact filename spelling
Correct folder path Photos/...
Tuning / Configuration
Most gameplay tuning is in game.h, e.g.:

PLAYER_SPEED, ENEMY_SPEED
PROJECTILE_SPEED
BOSS_TRIGGER_SCORE
Jump physics: GRAVITY, JUMP_VEL, PRE_JUMP_FRAMES, LAND_FRAMES
Boss projectile speed: BOSS_PROJECTILE_SPEED
Credits
Built using iGraphics framework (OpenGL/GLUT-based)
Sprites, music, and theme inspired by Stranger Things (educational/non-commercial project)
License
This project is for learning/demo purposes. If you plan to publish, ensure you have the rights to all third‑party assets (images/audio).
