#ifndef GAME_H
#define GAME_H

#include "iGraphics.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#undef max
#undef min

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

// Constants
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 700
#define WORLD_WIDTH 3000
#define MAX_ENEMIES 8
#define PLAYER_SPEED 4
#define ENEMY_SPEED 0.8
#define PROJECTILE_SPEED 8
#define MAX_PROJECTILES 50
#define MAX_COINS 100
#define MAX_BOSS_PROJECTILES 50
#define BOSS_PROJECTILE_SPEED 3
#define BOSS_TRIGGER_SCORE 20
const float GRAVITY = 0.9f;
const float JUMP_VEL = 14.0f;
const int   PRE_JUMP_FRAMES = 6;
const int   LAND_FRAMES = 6;
// Animation states
enum AnimationState {
	STATE_IDLE,
	STATE_RUNNING,
	STATE_ATTACK,
	STATE_ATTACKING,
	STATE_DAMAGE,
	STATE_DYING,
	STATE_DEAD
};

// Game states
enum GameState {
	START_PAGE,
	INSTRUCTIONS,
	SCORE_PAGE,
	CREDITS,
	CHARACTER_SELECT,
	PLAYING,
	GAME_OVER,
	PAUSED
};

// Character types
enum CharacterType {
	STEVE,
	ELEVEN,
	NONE
};
enum PickupType {
	PICKUP_BULLETS = 0,
	PICKUP_SHIELD = 1
};
struct BossProjectile {
	float x, y;
	float dx, dy;
	bool active;
	int frame;
};

struct Boss {
	float x, y;
	int health;
	int maxHealth;
	bool active;
	bool spawned;
	bool facingLeft;
	AnimationState animState;
	int animFrame;
	int animDelay;
	int attackCooldown;
	int hitFlashTimer;
};

// Projectile structure
struct Projectile {
	float x, y;
	bool active;
	CharacterType type;
	float dx, dy;
	int frame;
};


// Enemy structure
struct Enemy {
	float x, y;
	int maxHealth;
	bool active;
	int health;
	float speed;
	int frame;
	float angle;
	float targetX, targetY;
	AnimationState animState;
	int animFrame;
	int animDelay;
	int hitFlashTimer;
	int attackCooldown;
	bool facingLeft;
	bool coinDropped;
};

// Player structure
struct Player {
	float x, y;
	int health;
	int score;
	CharacterType type;
	AnimationState animState;
	int animFrame;
	int animDelay;
	int direction; // 0:right, 1:left, 2:up, 3:down
	bool isAttacking;
	int attackFrame;
	float angle;
	float vy;
	int jumpPhase;      // 0 none, 1 pre, 2 up, 3 peak, 4 down, 5 land
	int jumpTimer;
	float airBoostVx;
};

// Button structure
struct Button {
	int x, y, w, h;
	char text[50];
	bool hover;
	bool visible;
	void(*action)();
};
struct Coin {
	float x, y;
	bool active;
	PickupType type;
	int amount;
};

// Function declarations
void initGame();
void drawStartPage();
void drawInstructions();
void drawScorePage();
void drawCredits();
void drawCharacterSelect();
void drawGame();
void drawGameOver();
void drawPause();
void updateGame();
void spawnEnemy();
void moveEnemies();
void shootProjectile(float dx, float dy);
void updateProjectiles();
void checkCollisions();
void resetGame();
void checkButtonHover(int mx, int my);
void loadImages();
void saveHighScore();
void loadHighScore();
void updateAnimations();
void spawnBoss();
void updateBoss();
void drawBoss();
void shootBossProjectile();
void updateBossProjectiles();
void checkBossCollisions();

// Button actions
void actionStartGame();
void actionInstructions();
void actionHighScores();
void actionCredits();
void actionExit();
void actionBack();
void actionSelectSteve();
void actionSelectEleven();
void actionPlayAgain();
void actionResume();
void actionMainMenu();
// Global variables
extern GameState currentState;
extern Player player;
extern Enemy enemies[MAX_ENEMIES];
extern Projectile projectiles[MAX_PROJECTILES];
extern int mouseX, mouseY;
extern int highScores[10];
extern bool gamePaused;
extern Button buttons[20];
extern int buttonCount;
extern int frameCount;
extern int enemySpawnTimer;
extern int score;
extern unsigned int coinImg;
extern Boss boss;
extern BossProjectile bossProjectiles[MAX_BOSS_PROJECTILES];
extern unsigned int bossIdle;
extern unsigned int bossHit1;
extern unsigned int bossHit2;
extern unsigned int bossDead1;
extern unsigned int bossDead2;
extern unsigned int bossAttack1;
extern unsigned int bossAttack2;
extern bool bossFightStarted;

// Image textures
// Page Images
extern unsigned int homepageBg;
extern unsigned int menuBg;
extern unsigned int instructionBg;
extern unsigned int scoreBg;
extern unsigned int creditsBg;
extern unsigned int gameBg;
extern unsigned int characterSelectBg;

// Button Images
extern unsigned int startBtnImg;
extern unsigned int instructionBtnImg;
extern unsigned int scoreBtnImg;
extern unsigned int creditBtnImg;
extern unsigned int exitBtnImg;

// Steve Images
extern unsigned int steveIdle;
extern unsigned int steveRun1;
extern unsigned int steveRun2;
extern unsigned int stevePreShoot;
extern unsigned int steveAttack1;
extern unsigned int steveAttack2;

// Eleven Images
extern unsigned int elevenIdle;
extern unsigned int elevenRun1;
extern unsigned int elevenRun2;
extern unsigned int elevenPreShoot;
extern unsigned int elevenAttack1;

// Enemy Images
extern unsigned int enemyIdle;
extern unsigned int enemyRun1;
extern unsigned int enemyRun2;
extern unsigned int enemyAttack1;
extern unsigned int enemyAttack2;
extern unsigned int enemyDamage;
extern unsigned int enemyDie;
extern unsigned int elevenPreJump;
extern unsigned int elevenJumpStart;
extern unsigned int elevenJumpPeak;
extern unsigned int elevenJumpDown;
extern unsigned int elevenToLand;
extern unsigned int elevenLand;
// Health Bar Image
extern unsigned int healthBarImg;

extern unsigned int coinImg;
extern Coin coins[MAX_COINS];
extern int totalCoins;
void spawnCoin(float x, float y);
void collectCoins();
void drawCoins();
void spawnBulletPickup(float x, float y, int amount);
void spawnShieldPickup(float x, float y);
extern int bulletCount;
extern unsigned int fireImg;
extern unsigned int bulletPickupImg;
extern unsigned int shieldPickupImg;

extern bool shieldActive;
extern int shieldTimer;

extern int enemiesKilledTotal;
extern int bulletMsgTimer; 
extern unsigned int bossAreaBg;
extern int bossHitsTaken;
#endif