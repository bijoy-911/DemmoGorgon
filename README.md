# DEMOGORGON — Stranger Things Inspired Game

## Game Description

**DEMOGORGON** is a 2D graphics project created using the **iGraphics** library in **C/C++**.  
The game features two playable characters (**Steve** and **Eleven**) with different projectile styles, enemy waves, pickups (bullets and shield), and a boss fight with a dedicated boss arena background and music.

---

## Features
- Two playable characters: **Steve** and **Eleven**.
- Running, jumping, attack/shoot, and damage animations.
- Enemy AI that approaches the player and attacks at close range.
- Projectile shooting system (bullets consumed from ammo count).
- Pickup system:
  - **Bullet pickup** increases ammo (random amount).
  - **Shield pickup** gives **no damage** effect for **10 seconds**.
- Boss fight system:
  - Boss spawns after reaching a score threshold.
  - Background changes to **bossarea.png**.
  - Music changes to **bossFight.mp3**.
  - Boss shoots projectiles and takes damage.
- UI elements:
  - Health bar
  - Score display
  - Bullet counter with a fire icon
  - Enemy count
  - Pickup messages (Bullets gained / Shield gained)

---

## Project Details
IDE: Visual Studio 2010/2013

Language: C, C++

Platform: Windows PC

Genre: 2D action adventure

Library: iGraphics (OpenGL/GLUT based)

---

## How to Run the Project

Make sure you have the following installed:
- **Visual Studio 2010/2013**
- **iGraphics Library** (included with project files)
- OpenGL/GLUT requirements (used by iGraphics)

### Open the project in Visual Studio
- Open Visual Studio
- Go to **File → Open → Project/Solution**
- Select the project `.sln` file
- Click **Build → Build Solution**
- Run the program using **Debug → Start Without Debugging**

> If you see `LNK1104: cannot open ... .exe (Access is denied)`, close the running game from Task Manager and rebuild.

---

## How to Play

### **Controls**
| Action | Key |
|-------|-----|
| Move Left | `A` |
| Move Right | `D` |
| Jump | `Space` |
| Shoot | `F` |
| Pause/Resume | `P` |
| Exit to Menu | `ESC` |

### **Game Rules**
- Player starts with **100 HP** and a limited number of bullets.
- Shooting consumes **1 bullet** per shot.
- Enemies approach the player and attack at close range.
- Defeating enemies increases score and may drop pickups:
  - **Bullets pickup**: adds ammo.
  - **Shield pickup**: grants **invulnerability for 10 seconds**.
- Boss fight starts after reaching the score threshold (`BOSS_TRIGGER_SCORE`):
  - Background changes to **bossarea.png**
  - Music changes to **bossFight.mp3**
- If player HP reaches **0**, the game ends and the score is saved to the high score list.

---

## Assets / Files Required (Photos Folder)

Make sure these files exist inside the `Photos/` directory (names must match exactly):

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

### Sprites
- Steve sprites (idle/run/attack/jump/damage as included)
- Eleven sprites (new set: idle/run/damage/attack + jump sprites if used)
- Enemy sprites (idle/run/attack/death frames)
- Boss sprites (idle/attack/hit/dead frames)

---

## High Scores
- High scores are saved in a file named: `highscores.dat`
- The system keeps the top 10 scores.

---

## Project Contributors

1. Aquib
2. Rifat

---

## Screenshots

<img src="<img width="2404" height="1464" alt="Screenshot 2026-04-12 114651" src="https://github.com/user-attachments/assets/6a0875a4-3405-41b1-be50-90ab9ab8b727" />" width="200" height="200">

<img src="<img width="2404" height="1464" alt="Screenshot 2026-04-12 114309" src="https://github.com/user-attachments/assets/6ab608cb-c589-4804-b978-0337a3d4de03" />" width="200" height="200">

---

## Youtube Link

(https://youtu.be/uy4Qdr9DwoE))

---

## Project Report
(https://drive.google.com/file/d/1w_VfwWBSpd8fdADi1ynqD3skfo_E92f_/view?usp=sharing)

