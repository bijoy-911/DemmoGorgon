#include "game.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")


GameState currentState = START_PAGE;
Player player;
Enemy enemies[MAX_ENEMIES];
Projectile projectiles[MAX_PROJECTILES];
int mouseX = 0, mouseY = 0;
int highScores[10] = { 0 };
bool gamePaused = false;
Button buttons[20];
int buttonCount = 0;
int frameCount = 0;
int enemySpawnTimer = 0;
int score = 0;
const float GROUND_NORMAL = 140.0f;
const float GROUND_BOSS = 120.0f;   
float ground = GROUND_NORMAL;
float SCALE = 0.5f;   
int playerWidth = 240 * SCALE;
int playerHeight = 320 * SCALE;
int enemyWidth = 200 * SCALE;
int enemyHeight = 260 * SCALE;
float cameraX = 0.0f;
Coin coins[MAX_COINS];
int totalCoins = 0;
unsigned int coinImg = 0;
const float ENEMY_ATTACK_RANGE = 90.0f;  
const int   ENEMY_ATTACK_DAMAGE = 4;     
const int   ENEMY_ATTACK_COOLDOWN = 60;   
Boss boss;
BossProjectile bossProjectiles[MAX_BOSS_PROJECTILES];
bool bossFightStarted = false;
int bulletCount = 20;
unsigned int fireImg = 0;
unsigned int bossIdle = 0;
unsigned int bossHit1 = 0;
unsigned int bossHit2 = 0;
unsigned int bossDead1 = 0;
unsigned int bossDead2 = 0;
unsigned int bossAttack1 = 0;
unsigned int bossAttack2 = 0;
unsigned int healthBarImg = 0;
unsigned int stevePreJump = 0;
unsigned int steveJump1 = 0;
unsigned int steveJump2 = 0;
unsigned int steveJump3 = 0;
unsigned int steveLand = 0;
unsigned int elevenPreJump = 0;     
unsigned int elevenJumpStart = 0;   
unsigned int elevenJumpPeak = 0;    
unsigned int elevenJumpDown = 0;    
unsigned int elevenToLand = 0;     
unsigned int elevenLand = 0;
unsigned int bulletPickupImg = 0;
unsigned int shieldPickupImg = 0;
int nextBossBulletDropHit = 8;
int nextBossShieldDropHit = 15;
unsigned int bossAreaBg = 0;
bool bossDroppedBullets = false;
bool bossDroppedShield = false;
bool bossCameraLocked = false;
float bossCameraLockX = 0.0f;
float bossArenaLeft = 0.0f;
float bossArenaRight = 0.0f;
int bossShootAnimTimer = 0;
const int BOSS_SHOOT_ANIM_TICKS = 25;
bool shieldActive = false;
int shieldTimer = 0;

int enemiesKilledTotal = 0;

int bulletMsgTimer = 0;
int shieldMsgTimer = 0;
int lastBulletsGained = 0;

int bossHitsTaken = 0;
int bossHitsForShootReset = 0;   
const int SHOOT_RESET_AFTER_HITS = 3;


const int SHIELD_DURATION_TICKS = 10 * 50; 
const int MSG_DURATION_TICKS = 2 * 50;     
void playMusic(const char* filename) {
	mciSendString("close bgm", NULL, 0, NULL);

	char command[200];
	sprintf_s(command, "open \"%s\" type mpegvideo alias bgm", filename);
	mciSendString(command, NULL, 0, NULL);

	mciSendString("play bgm repeat", NULL, 0, NULL);
}

void stopMusic() {
	mciSendString("close bgm", NULL, 0, NULL);
}


unsigned int homepageBg = 0;
unsigned int menuBg = 0;
unsigned int instructionBg = 0;
unsigned int scoreBg = 0;
unsigned int creditsBg = 0;
unsigned int gameBg = 0;
unsigned int characterSelectBg = 0;


unsigned int startBtnImg;
unsigned int instructionBtnImg;
unsigned int scoreBtnImg;
unsigned int creditBtnImg;
unsigned int exitBtnImg;
unsigned int backBtnImg;


unsigned int steveIdle = 0;

unsigned int steveRun1 = 0, steveRun2 = 0, steveRun3 = 0;
unsigned int steveRun4 = 0, steveRun5 = 0, steveRun6 = 0;

unsigned int steveDamage1 = 0, steveDamage2 = 0, steveDamage3 = 0;

unsigned int stevePreAttack = 0;
unsigned int steveShoot = 0;
unsigned int stevePostShoot = 0;

unsigned int elevenIdle = 0;


unsigned int elevenRun1 = 0;
unsigned int elevenRun2 = 0;
unsigned int elevenRun3 = 0;
unsigned int elevenRun4 = 0;
unsigned int elevenRun5 = 0;
unsigned int elevenRun6 = 0;


unsigned int elevenPreDamage = 0;
unsigned int elevenDamage = 0;
unsigned int elevenPostDamage = 0;


unsigned int elevenPreAttack = 0;
unsigned int elevenPreAttack1 = 0;
unsigned int elevenAttack = 0;


unsigned int enemyIdle = 0;

unsigned int enemyRun1 = 0, enemyRun2 = 0, enemyRun3 = 0, enemyRun4 = 0, enemyRun5 = 0;

unsigned int enemyDeath1 = 0, enemyDeath2 = 0, enemyDeath3 = 0, enemyDeath4 = 0, enemyDeath5 = 0;

unsigned int enemyAttack1 = 0, enemyAttack2 = 0, enemyAttack3 = 0;


void addHighScore(int newScore);
void saveHighScore();
void loadHighScore();


void actionStartGame() {
	currentState = CHARACTER_SELECT;
	stopMusic();
	playMusic("Photos/Ingame.mp3");
}

void actionInstructions() {
	currentState = INSTRUCTIONS;
}

void actionHighScores() {
	loadHighScore();
	currentState = SCORE_PAGE;
}

void actionCredits() {
	currentState = CREDITS;
}

void actionExit() {
	stopMusic();
	mciSendString("close all", NULL, 0, NULL);
	exit(0);
}

void actionBack() {
	if (currentState == CHARACTER_SELECT || currentState == PLAYING) {
		stopMusic();
		playMusic("Photos/bg.mp3");
	}
	currentState = START_PAGE;
}

void actionSelectSteve() {
	resetGame();
	player.type = STEVE;
	currentState = PLAYING;
}

void actionSelectEleven() {
	resetGame();
	player.type = ELEVEN;
	currentState = PLAYING;
}

void actionPlayAgain() {
	addHighScore(player.score);
	resetGame();
	currentState = START_PAGE;
}

void actionResume() {
	gamePaused = false;
	currentState = PLAYING;
}

void actionMainMenu() {
	gamePaused = false;
	currentState = START_PAGE;
	stopMusic();
	playMusic("Photos/bg.mp3");
}


void saveHighScore() {
	FILE* file = nullptr;
	if (fopen_s(&file, "highscores.dat", "wb") == 0 && file) {
		fwrite(highScores, sizeof(int), 10, file);
		fclose(file);
	}
}


void loadHighScore() {
	FILE* file = nullptr;
	if (fopen_s(&file, "highscores.dat", "rb") == 0 && file) {
		fread(highScores, sizeof(int), 10, file);
		fclose(file);
	}
	else {

		for (int i = 0; i < 10; i++) {
			highScores[i] = 100 - i * 10;
		}
	}
}


void addHighScore(int newScore) {
	for (int i = 0; i < 10; i++) {
		if (newScore > highScores[i]) {
			for (int j = 9; j > i; j--) {
				highScores[j] = highScores[j - 1];
			}
			highScores[i] = newScore;
			break;
		}
	}
	saveHighScore();
}

void initButtons() {
	buttonCount = 0;


	buttons[buttonCount++] = { WINDOW_WIDTH / 2 - 100, 500, 200, 50, "START GAME", false, true, actionStartGame };
	buttons[buttonCount++] = { WINDOW_WIDTH / 2 - 100, 430, 200, 50, "INSTRUCTIONS", false, true, actionInstructions };
	buttons[buttonCount++] = { WINDOW_WIDTH / 2 - 100, 360, 200, 50, "HIGH SCORES", false, true, actionHighScores };
	buttons[buttonCount++] = { WINDOW_WIDTH / 2 - 100, 290, 200, 50, "CREDITS", false, true, actionCredits };
	buttons[buttonCount++] = { WINDOW_WIDTH / 2 - 100, 220, 200, 50, "EXIT", false, true, actionExit };


	buttons[buttonCount++] = { 187, 64, 283, 58, "STEVE", false, true, actionSelectSteve };
	buttons[buttonCount++] = { 706, 64, 372, 58, "ELEVEN", false, true, actionSelectEleven };

	buttons[buttonCount++] = { 50, 50, 100, 40, "BACK", false, true, actionBack };


	buttons[buttonCount++] = { WINDOW_WIDTH / 2 - 100, 250, 200, 50, "PLAY AGAIN", false, true, actionPlayAgain };


	buttons[buttonCount++] = { WINDOW_WIDTH / 2 - 100, 400, 200, 50, "RESUME", false, true, actionResume };
	buttons[buttonCount++] = { WINDOW_WIDTH / 2 - 100, 330, 200, 50, "MAIN MENU", false, true, actionMainMenu };
}

void loadImages() {

	homepageBg = iLoadImage("Photos/Homepage.png");
	menuBg = iLoadImage("Photos/menu.png");


	instructionBg = iLoadImage("Photos/SteveInstructions.png");   


	scoreBg = iLoadImage("Photos/scoreBoard.png");

	creditsBg = iLoadImage("Photos/Credit.png");


	gameBg = iLoadImage("Photos/gamebg.png");     


	characterSelectBg = iLoadImage("Photos/CharacterChoose.png");


	startBtnImg = iLoadImage("Photos/buttonStock1.png");
	instructionBtnImg = iLoadImage("Photos/Instructions.png");
	scoreBtnImg = iLoadImage("Photos/HighScore.png");
	creditBtnImg = iLoadImage("Photos/buttonStock1h.png");
	exitBtnImg = iLoadImage("Photos/buttonStock1k.png");
	backBtnImg = iLoadImage("Photos/backButton.png");

	steveIdle = iLoadImage("Photos/steveidle.png");

	steveRun1 = iLoadImage("Photos/steverun1.png");
	steveRun2 = iLoadImage("Photos/steverun2.png");
	steveRun3 = iLoadImage("Photos/steverun3.png");
	steveRun4 = iLoadImage("Photos/steverun4.png");
	steveRun5 = iLoadImage("Photos/steverun5.png");
	steveRun6 = iLoadImage("Photos/steverun6.png");

	steveDamage1 = iLoadImage("Photos/stevedamage1.png");
	steveDamage2 = iLoadImage("Photos/stevedamage2.png");
	steveDamage3 = iLoadImage("Photos/stevedamage3.png");

	stevePreAttack = iLoadImage("Photos/stevepreshoot.png");
	steveShoot = iLoadImage("Photos/steveshoot.png");
	stevePostShoot = iLoadImage("Photos/stevepostshoot.png");

	elevenIdle = iLoadImage("Photos/elevenidle.png");

	elevenRun1 = iLoadImage("Photos/elevenrun1.png");
	elevenRun2 = iLoadImage("Photos/elevenrun2.png");
	elevenRun3 = iLoadImage("Photos/elevenrun3.png");
	elevenRun4 = iLoadImage("Photos/elevenrun4.png");
	elevenRun5 = iLoadImage("Photos/elevenrun5.png");
	elevenRun6 = iLoadImage("Photos/elevenrun6.png");


	elevenPreDamage = iLoadImage("Photos/elevenpredamage.png");
	elevenDamage = iLoadImage("Photos/elevendamage.png");
	elevenPostDamage = iLoadImage("Photos/elevenpostdamage.png");


	elevenPreAttack = iLoadImage("Photos/elevenpreattack.png");
	elevenPreAttack1 = iLoadImage("Photos/elevenpreattack1.png");
	elevenAttack = iLoadImage("Photos/elevenattack.png");


	enemyIdle = iLoadImage("Photos/enemyidle.png");


	enemyRun1 = iLoadImage("Photos/enemyrun1.png");
	enemyRun2 = iLoadImage("Photos/enemyrun2.png");
	enemyRun3 = iLoadImage("Photos/enemyrun3.png");
	enemyRun4 = iLoadImage("Photos/enemyrun4.png");
	enemyRun5 = iLoadImage("Photos/enemyrun5.png");


	enemyDeath1 = iLoadImage("Photos/enemydamage.png");
	enemyDeath2 = iLoadImage("Photos/enemydamage1.png");
	enemyDeath3 = iLoadImage("Photos/enemydamage3.png");
	enemyDeath4 = iLoadImage("Photos/enemydamage4.png");
	enemyDeath5 = iLoadImage("Photos/enemydamage5.png");


	enemyAttack1 = iLoadImage("Photos/enemyattack1.png");
	enemyAttack2 = iLoadImage("Photos/enemyattack2.png");
	enemyAttack3 = iLoadImage("Photos/enemyattack3.png");

	coinImg = iLoadImage("Photos/coin.png");
	bossIdle = iLoadImage("Photos/bossidle.png");
	bossHit1 = iLoadImage("Photos/bossprehit1.png");
	bossHit2 = iLoadImage("Photos/bosshit2.png");
	bossDead1 = iLoadImage("Photos/bossdead1.png");
	bossDead2 = iLoadImage("Photos/bossdead2.png");
	bossAttack1 = iLoadImage("Photos/bossattack1.png");
	bossAttack2 = iLoadImage("Photos/bossattack2.png");

	healthBarImg = iLoadImage("Photos/health.png");

	stevePreJump = iLoadImage("Photos/steveprejump.png");
	steveJump1 = iLoadImage("Photos/stevejump1.png");
	steveJump2 = iLoadImage("Photos/stevejump2.png");
	steveJump3 = iLoadImage("Photos/stevejump3.png");
	steveLand = iLoadImage("Photos/steveland.png");

	elevenPreJump = iLoadImage("Photos/elevenprejump.png");
	elevenJumpStart = iLoadImage("Photos/elevenprejump1.png");
	elevenJumpPeak = iLoadImage("Photos/elevenjump1.png");
	elevenJumpDown = iLoadImage("Photos/elevenjump2.png");
	elevenToLand = iLoadImage("Photos/elevenjump3.png");
	elevenLand = iLoadImage("Photos/elevenland.png");
	fireImg = iLoadImage("Photos/fire.png");
	bulletPickupImg = iLoadImage("Photos/bullet.png");
	shieldPickupImg = iLoadImage("Photos/shield.png");
	bossAreaBg = iLoadImage("Photos/bossarea.png");
}

void initGame() {
	playMusic("Photos/bg.mp3");
	srand((unsigned int)time(NULL));
	initButtons();
	loadHighScore();
	loadImages();
	resetGame();
}
void startJump() {
	if (player.jumpPhase != 0) return;   

	player.jumpPhase = 1;                
	player.jumpTimer = PRE_JUMP_FRAMES;
	player.vy = 0;


	player.airBoostVx = (player.direction == 0) ? 2.2f : -2.2f;
}

void updateJump() {

	if (player.jumpPhase == 5) { 
		if (--player.jumpTimer <= 0) {
			player.jumpPhase = 0; 
		}
		return;
	}


	if (player.jumpPhase == 1) { 
		if (--player.jumpTimer <= 0) {
			player.vy = JUMP_VEL;
			player.jumpPhase = 2; 
		}
		return;
	}


	if (player.jumpPhase >= 2 && player.jumpPhase <= 4) {


		player.x += player.airBoostVx;
		player.airBoostVx *= 0.92f;


		player.y += player.vy;
		player.vy -= GRAVITY;


		if (player.vy > 3.0f) player.jumpPhase = 2;          
		else if (fabs(player.vy) <= 3.0f) player.jumpPhase = 3; 
		else player.jumpPhase = 4;                           


		if (player.y <= ground) {
			player.y = ground;
			player.vy = 0;
			player.jumpPhase = 5;    
			player.jumpTimer = LAND_FRAMES;
		}
	}
}
void resetGame() {
	ground = GROUND_NORMAL;
	player.x = 160.0f;
	player.y = ground;
	player.health = 100;
	player.score = 0;
	player.type = NONE;
	player.animState = STATE_IDLE;
	player.animFrame = 0;
	player.animDelay = 0;
	player.direction = 0;
	player.isAttacking = false;
	player.attackFrame = 0;
	player.angle = 0;
	player.vy = 0;
	player.jumpPhase = 0;
	player.jumpTimer = 0;
	player.airBoostVx = 0;

	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemies[i].active = false;
		enemies[i].health = 3;     
		enemies[i].maxHealth = 3;
		enemies[i].frame = rand() % 360;
		enemies[i].angle = 0;
		enemies[i].animState = STATE_IDLE;
		enemies[i].animFrame = 0;
		enemies[i].animDelay = 0;
		enemies[i].attackCooldown = 0;
		enemies[i].hitFlashTimer = 0;
		enemies[i].facingLeft = false;
		enemies[i].coinDropped = false;
	}

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		projectiles[i].active = false;
		projectiles[i].frame = 0;
	}

	enemySpawnTimer = 0;
	score = 0;
	cameraX = 0.0f;
	bulletCount = 20;
	for (int i = 0; i < MAX_COINS; i++) {
		coins[i].active = false;
		coins[i].x = 0;
		coins[i].y = 0;
		coins[i].type = PICKUP_BULLETS;
		coins[i].amount = 0;
	}

	shieldActive = false;
	shieldTimer = 0;

	enemiesKilledTotal = 0;

	bulletMsgTimer = 0;
	shieldMsgTimer = 0;
	lastBulletsGained = 0;
	bossCameraLocked = false;
	bossCameraLockX = 0.0f;
	bossArenaLeft = 0.0f;
	bossArenaRight = 0.0f;
	boss.active = false;
	boss.spawned = false;
	boss.health = 50;
	boss.maxHealth = 50;
	boss.x = WORLD_WIDTH - 500;
	boss.y = ground;
	boss.facingLeft = true;
	boss.animState = STATE_IDLE;
	boss.animFrame = 0;
	boss.animDelay = 0;
	boss.attackCooldown = 0;
	boss.hitFlashTimer = 0;
	bossHitsTaken = 0;
	bossHitsForShootReset = 0;
	bossDroppedBullets = false;
	bossDroppedShield = false;
	nextBossBulletDropHit = 8;
	nextBossShieldDropHit = 15;

	bossFightStarted = false;

	for (int i = 0; i < MAX_BOSS_PROJECTILES; i++) {
		bossProjectiles[i].active = false;
		bossProjectiles[i].frame = 0;
	}

}
void spawnBoss() {
	if (boss.spawned) return;

	boss.active = true;
	boss.spawned = true;
	boss.health = 50;
	boss.maxHealth = 50;
	boss.x = player.x + 500;
	if (boss.x > WORLD_WIDTH - 250) boss.x = WORLD_WIDTH - 250;
	boss.y = ground;
	boss.facingLeft = true;
	boss.animState = STATE_IDLE;
	boss.animFrame = 0;
	boss.animDelay = 0;
	boss.attackCooldown = 150; 
	boss.hitFlashTimer = 0;

	bossFightStarted = true;
	ground = GROUND_BOSS;
	
	bossCameraLocked = true;
	bossCameraLockX = cameraX;             
	bossArenaLeft = bossCameraLockX;
	bossArenaRight = bossCameraLockX + WINDOW_WIDTH;

	
	cameraX = bossCameraLockX;

	
	float bossHalfW = (300 * SCALE) * 0.5f;
	boss.x = player.x + 350;
	if (boss.x > bossArenaRight - bossHalfW) boss.x = bossArenaRight - bossHalfW;
	if (boss.x < bossArenaLeft + bossHalfW)  boss.x = bossArenaLeft + bossHalfW;
	
	if (player.jumpPhase == 0) player.y = ground;
	boss.y = ground;

	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].active) enemies[i].y = ground;
	}
	stopMusic();
	playMusic("Photos/bossFight.mp3");
	
	for (int i = 0; i < MAX_ENEMIES; i++) {
		enemies[i].active = false;
	}
	bossHitsTaken = 0;
	bossHitsForShootReset = 0;
	bossDroppedBullets = false;
	bossDroppedShield = false;
	nextBossBulletDropHit = 8;
	nextBossShieldDropHit = 15;
}
void spawnBulletPickup(float x, float y, int amount) {
	for (int i = 0; i < MAX_COINS; i++) {
		if (!coins[i].active) {
			coins[i].active = true;
			coins[i].x = x;
			coins[i].y = y;
			coins[i].type = PICKUP_BULLETS;
			coins[i].amount = amount;
			break;
		}
	}
}

void spawnShieldPickup(float x, float y) {
	for (int i = 0; i < MAX_COINS; i++) {
		if (!coins[i].active) {
			coins[i].active = true;
			coins[i].x = x;
			coins[i].y = y;
			coins[i].type = PICKUP_SHIELD;
			coins[i].amount = 0;
			break;
		}
	}
}

void spawnEnemy() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (!enemies[i].active) {
			enemies[i].active = true;
			enemies[i].health = 3;
			enemies[i].maxHealth = 3;
			enemies[i].speed = ENEMY_SPEED + (float)(rand() % 5) *0.15f;
			enemies[i].frame = rand() % 360;
			enemies[i].angle = (float)(rand() % 360);
			enemies[i].animState = STATE_IDLE;
			enemies[i].animFrame = 0;
			enemies[i].animDelay = 0;
			enemies[i].hitFlashTimer = 0;
			enemies[i].attackCooldown = 0;
			enemies[i].coinDropped = false;

			float leftVisible = cameraX;
			float rightVisible = cameraX + WINDOW_WIDTH;

			int side = rand() % 2; 

			if (side == 0) {
				
				enemies[i].x = leftVisible - (100 + rand() % 200);

				if (enemies[i].x < 30)
					enemies[i].x = 30;

				
				if (enemies[i].x > leftVisible - 80)
					enemies[i].x = leftVisible - 120;
				enemies[i].facingLeft = false;
			}
			else {
				
				enemies[i].x = rightVisible + (100 + rand() % 200);

				if (enemies[i].x > WORLD_WIDTH - 100)
					enemies[i].x = WORLD_WIDTH - 100;

				
				if (enemies[i].x < rightVisible + 80)
					enemies[i].x = rightVisible + 120;
				enemies[i].facingLeft = true;
			}

			enemies[i].y = ground;
			enemies[i].targetX = player.x;
			enemies[i].targetY = player.y;
			break;
		}
	}
}

void updateAnimations() {
	
	player.animDelay++;
	if (player.animDelay > 4) {
		player.animDelay = 0;
		player.animFrame++;

		switch (player.animState) {

		case STATE_RUNNING:
			
			if (player.animFrame > 5) player.animFrame = 0;
			break;

		case STATE_DAMAGE:
			
			if (player.animFrame > 2) {
				player.animFrame = 0;
				player.animState = STATE_IDLE;
			}
			break;

		case STATE_IDLE:
		default:
			player.animFrame = 0;
			break;
		}
	}
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].active) {
			enemies[i].animDelay++;
			if (enemies[i].animDelay >5) {
				enemies[i].animDelay = 0;
				enemies[i].animFrame++;

				switch (enemies[i].animState) {

				case STATE_RUNNING:
					if (enemies[i].animFrame > 4) enemies[i].animFrame = 0; 
					break;

				case STATE_ATTACK:
					if (enemies[i].animFrame > 2) enemies[i].animFrame = 0; 
					break;

				case STATE_DYING:
					if (enemies[i].animFrame > 4) {
						enemies[i].active = false;
					}
					break;

				default:
					enemies[i].animFrame = 0;
					break;
				}
			}
		}
	}
}

void moveEnemies() {
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].active && enemies[i].animState != STATE_DYING) {

			if (enemies[i].attackCooldown > 0) {
				enemies[i].attackCooldown--;
			}

			if (frameCount % 30 == 0) {
				enemies[i].targetX = player.x;
				enemies[i].targetY = player.y;
			}
			float dx = enemies[i].targetX - enemies[i].x;
			float dy = enemies[i].targetY - enemies[i].y;
			float distToPlayer = (float)sqrt(dx * dx + dy * dy);
			if (distToPlayer <= ENEMY_ATTACK_RANGE) {

				enemies[i].animState = STATE_ATTACK;

				if (enemies[i].attackCooldown <= 0) {

					if (!shieldActive) {
						player.health -= ENEMY_ATTACK_DAMAGE;
						if (player.health < 0) player.health = 0;


						player.animState = STATE_DAMAGE;
						player.animFrame = 0;
						player.animDelay = 0;
					}

					enemies[i].attackCooldown = ENEMY_ATTACK_COOLDOWN;

					if (player.health <= 0) {
						addHighScore(player.score);
						currentState = GAME_OVER;
					}
				}


				continue;
			}

			if (dx < 0) {
				enemies[i].facingLeft = true;
			}
			else if (dx > 0) {
				enemies[i].facingLeft = false;
			}

			bool enemyInFront = false;
			for (int j = 0; j < MAX_ENEMIES; j++) {
				if (i != j && enemies[j].active) {

					float djx = enemies[j].x - enemies[i].x;
					float djy = enemies[j].y - enemies[i].y;
					float distBetween = (float)sqrt(djx * djx + djy * djy);


					float distJToPlayer = (float)sqrt(
						(enemies[j].targetX - enemies[j].x) * (enemies[j].targetX - enemies[j].x) +
						(enemies[j].targetY - enemies[j].y) * (enemies[j].targetY - enemies[j].y)
						);


					float minDistance = 150.0f;

					if (distBetween < minDistance) {

						if (distJToPlayer < distToPlayer) {
							enemyInFront = true;
							break;
						}
						else {

							if (distBetween > 0) {
								float pushX = (enemies[i].x - enemies[j].x) / distBetween;
								enemies[i].x += pushX * 2.0f;
							}
						}
					}
				}
			}


			if (!enemyInFront && distToPlayer > 5) {
				enemies[i].animState = STATE_RUNNING;

				float moveX = (dx / distToPlayer) * enemies[i].speed;
				float moveY = (dy / distToPlayer) * enemies[i].speed;

				enemies[i].x += moveX;
				enemies[i].y += moveY;
			}
			else if (distToPlayer <= 5) {
				enemies[i].animState = STATE_IDLE;
			}
			else {

				enemies[i].animState = STATE_RUNNING;
			}


			if (enemies[i].x < 30) enemies[i].x = 30;
			if (enemies[i].x > WORLD_WIDTH - 70) enemies[i].x = (float)(WORLD_WIDTH - 70);

			enemies[i].frame = (enemies[i].frame + 3) % 360;
		}
	}
}
void shootProjectile(float dx, float dy) {
	if (player.type == NONE) return;

	player.animState = STATE_ATTACK;
	player.animFrame = 0;

	float len = (float)sqrt(dx * dx + dy * dy);
	if (len > 0) {
		dx /= len;
		dy /= len;
	}
	else {
		dx = 1;
		dy = 0;
	}

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (!projectiles[i].active) {
			projectiles[i].active = true;


			if (dx >= 0)
				projectiles[i].x = player.x + playerWidth * 0.5f - 10;
			else
				projectiles[i].x = player.x - playerWidth * 0.5f + 10;

			projectiles[i].y = player.y + playerHeight * 0.35f;  
			projectiles[i].type = player.type;
			projectiles[i].dx = dx * PROJECTILE_SPEED;
			projectiles[i].dy = dy * PROJECTILE_SPEED;
			projectiles[i].frame = 0;
			bossShootAnimTimer = BOSS_SHOOT_ANIM_TICKS;
			boss.animState = STATE_ATTACK;
			boss.animFrame = 0;
			boss.animDelay = 0;
			break;
		}
	}
}

void updateProjectiles() {
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (projectiles[i].active) {

			projectiles[i].x += projectiles[i].dx;
			projectiles[i].y += projectiles[i].dy;
			projectiles[i].frame++;


			if (projectiles[i].x < -30 || projectiles[i].x > WORLD_WIDTH + 30 ||
				projectiles[i].y < -30 || projectiles[i].y > WINDOW_HEIGHT + 30) {
				projectiles[i].active = false;
			}
		}
	}
}
void checkCollisions() {
	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (!projectiles[i].active) continue;

		for (int j = 0; j < MAX_ENEMIES; j++) {
			if (enemies[j].active && enemies[j].animState != STATE_DYING) {

				float enemyLeft = enemies[j].x - 80;
				float enemyRight = enemies[j].x + 80;
				float enemyBottom = ground + 20;
				float enemyTop = ground + 180;

				float px = projectiles[i].x;
				float py = projectiles[i].y;

				if (px >= enemyLeft && px <= enemyRight &&
					py >= enemyBottom && py <= enemyTop) {

					projectiles[i].active = false;
					enemies[j].health--;
					enemies[j].hitFlashTimer = 10;

					if (enemies[j].health <= 0) {
						enemies[j].health = 0;
						enemies[j].animState = STATE_DYING;
						enemies[j].animFrame = 0;
						player.score += 10;

						if (!enemies[j].coinDropped) {
							enemies[j].coinDropped = true;

							if (rand() % 100 < 60) {
								int gain = 10 + rand() % 6; 
								spawnBulletPickup(enemies[j].x, ground + 40, gain);
							}


							enemiesKilledTotal++;


							if (enemiesKilledTotal % 5 == 0) {
								if (rand() % 100 < 40) {
									spawnShieldPickup(enemies[j].x, ground + 40);
								}
							}
						}
					}
					else {
						enemies[j].animState = STATE_DAMAGE;
						enemies[j].animFrame = 0;
					}
					break;
				}
			}
		}
	}
}
void drawCoins() {
	for (int i = 0; i < MAX_COINS; i++) {
		if (!coins[i].active) continue;

		unsigned int img = 0;
		if (coins[i].type == PICKUP_BULLETS) img = bulletPickupImg;
		else if (coins[i].type == PICKUP_SHIELD) img = shieldPickupImg;

		if (!img) continue;

		iShowImage(
			(int)(coins[i].x - cameraX - 20),
			(int)coins[i].y,
			40, 40,
			img
			);
	}
}
void checkButtonHover(int mx, int my) {
	for (int i = 0; i < buttonCount; i++) {
		if (buttons[i].visible) {
			if (mx >= buttons[i].x && mx <= buttons[i].x + buttons[i].w &&
				my >= buttons[i].y && my <= buttons[i].y + buttons[i].h) {
				buttons[i].hover = true;
			}
			else {
				buttons[i].hover = false;
			}
		}
	}
}
void drawStartPage() {

	if (menuBg) {
		iShowImage(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, menuBg);
	}


	for (int i = 0; i < buttonCount; i++) {

		if (strcmp(buttons[i].text, "START GAME") == 0)
			iShowImage(buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h, startBtnImg);

		else if (strcmp(buttons[i].text, "INSTRUCTIONS") == 0)
			iShowImage(buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h, instructionBtnImg);

		else if (strcmp(buttons[i].text, "HIGH SCORES") == 0)
			iShowImage(buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h, scoreBtnImg);

		else if (strcmp(buttons[i].text, "CREDITS") == 0)
			iShowImage(buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h, creditBtnImg);

		else if (strcmp(buttons[i].text, "EXIT") == 0)
			iShowImage(buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h, exitBtnImg);
	}
}


void drawInstructions() {
	// Draw instruction background
	if (instructionBg) {
		iShowImage(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, instructionBg);
	}
	else {
		iSetColor(30, 30, 50);
		iFilledRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	iSetColor(255, 255, 255);
	iText(400, 400, " Use 'a' or 'd' KEYS to move your character ");
	iText(400, 350, " Press SPACE to Jump ");
	iText(400, 300, " Press 'f' to Shoot ");
	iText(400, 250, " Defeat DEMOGORGON enemies to earn points ");


	if (buttons[7].visible) {
		iShowImage(buttons[7].x, buttons[7].y,
			buttons[7].w, buttons[7].h,
			backBtnImg);
	}

}


void drawScorePage() {
	if (scoreBg) {
		iShowImage(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, scoreBg);
	}
	else {
		iSetColor(30, 30, 50);
		iFilledRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	iSetColor(255, 255, 0);
	iText(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT - 80, "HIGH SCORES");

	iSetColor(255, 255, 255);
	for (int i = 0; i < 10; i++) {
		char scoreLine[50];
		if (highScores[i] > 0) {
			sprintf_s(scoreLine, sizeof(scoreLine), "%d. %d", i + 1, highScores[i]);
			iText(WINDOW_WIDTH / 2 - 50, 550 - i * 40, scoreLine);
		}
	}


	if (buttons[7].visible) {
		iShowImage(buttons[7].x, buttons[7].y,
			buttons[7].w, buttons[7].h,
			backBtnImg);
	}
}


void drawCredits() {
	if (creditsBg) {
		iShowImage(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, creditsBg);
	}
	else {
		iSetColor(30, 30, 50);
		iFilledRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	}


	if (buttons[7].visible) {
		iShowImage(buttons[7].x, buttons[7].y,
			buttons[7].w, buttons[7].h,
			backBtnImg);
	}

}


void drawCharacterSelect() {
	if (characterSelectBg) {
		iShowImage(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, characterSelectBg);
	}
	else {
		iSetColor(30, 30, 50);
		iFilledRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	if (buttons[7].visible) {
		iShowImage(buttons[7].x, buttons[7].y,
			buttons[7].w, buttons[7].h,
			backBtnImg);
	}
}
void drawBoldItalicOrangeStrokeText(int x, int y, const char* str, float scale = 0.25f)
{

	iSetColor(255, 140, 0);

	glPushMatrix();
	glTranslatef((float)x, (float)y, 0.0f);


	const float shear = 0.35f;
	float m[16] = {
		1, 0, 0, 0,
		shear, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	glMultMatrixf(m);

	glScalef(scale, scale, 1.0f);


	for (int pass = 0; pass < 2; pass++) {
		glPushMatrix();
		glTranslatef((float)pass * 2.0f, 0.0f, 0.0f);
		for (const char* p = str; *p; ++p)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
		glPopMatrix();
	}

	glPopMatrix();
}
void drawGame() {

	if (bossFightStarted && bossAreaBg) {

		iShowImage(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, bossAreaBg);
	}
	else if (gameBg) {

		iShowImage((int)-cameraX, 0, WORLD_WIDTH, WINDOW_HEIGHT, gameBg);
	}
	else {
		iSetColor(40, 40, 60);
		iFilledRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	}


	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].active) {
			enemies[i].frame++;

			unsigned int currentEnemyImage = enemyIdle;

			if (enemies[i].animState == STATE_RUNNING) {
				int f = enemies[i].animFrame % 5;
				if (f == 0) currentEnemyImage = enemyRun1;
				else if (f == 1) currentEnemyImage = enemyRun2;
				else if (f == 2) currentEnemyImage = enemyRun3;
				else if (f == 3) currentEnemyImage = enemyRun4;
				else             currentEnemyImage = enemyRun5;
			}
			else if (enemies[i].animState == STATE_ATTACK) {
				int f = enemies[i].animFrame % 3;
				if (f == 0) currentEnemyImage = enemyAttack1;
				else if (f == 1) currentEnemyImage = enemyAttack2;
				else             currentEnemyImage = enemyAttack3;
			}
			else if (enemies[i].animState == STATE_DYING) {
				int f = enemies[i].animFrame % 5;
				if (f == 0) currentEnemyImage = enemyDeath1;
				else if (f == 1) currentEnemyImage = enemyDeath2;
				else if (f == 2) currentEnemyImage = enemyDeath3;
				else if (f == 3) currentEnemyImage = enemyDeath4;
				else             currentEnemyImage = enemyDeath5;
			}
			else {
				currentEnemyImage = enemyIdle;
			}

			if (currentEnemyImage) {
				if (enemies[i].hitFlashTimer > 0) {
					enemies[i].hitFlashTimer--;
				}
				int enemyDrawX = (int)(enemies[i].x - cameraX) - enemyWidth / 2;

				iShowImageFlipped(
					enemyDrawX,
					ground,
					enemyWidth,
					enemyHeight,
					currentEnemyImage,
					enemies[i].facingLeft
					);
			}
		}
	}

	drawBoss();
	drawCoins();
	for (int i = 0; i < MAX_BOSS_PROJECTILES; i++) {
		if (bossProjectiles[i].active) {

			int sx = (int)(bossProjectiles[i].x - cameraX);
			int sy = (int)(bossProjectiles[i].y);

			iSetColor(200, 0, 200);
			iFilledCircle(sx, sy, 10);

			iSetColor(255, 100, 255);
			iFilledCircle(sx, sy, 6);
		}
	}


	if (player.animState != STATE_DAMAGE) {
		if (player.isAttacking) {
			player.animState = STATE_ATTACK;
		}
		else if (isKeyPressed('a') || isKeyPressed('d')) {
			player.animState = STATE_RUNNING;
		}
		else {
			player.animState = STATE_IDLE;
		}
	}

	unsigned int currentPlayerImage = 0;
	bool playerFacingLeft = (player.direction == 1); 

	if (player.type == STEVE) {

		if (player.jumpPhase != 0) {
			if (player.jumpPhase == 1) currentPlayerImage = stevePreJump;
			else if (player.jumpPhase == 2) currentPlayerImage = steveJump1;
			else if (player.jumpPhase == 3) currentPlayerImage = steveJump2;
			else if (player.jumpPhase == 4) currentPlayerImage = steveJump3;
			else if (player.jumpPhase == 5) currentPlayerImage = steveLand;
		}
		else if (player.animState == STATE_DAMAGE) {
			if (player.animFrame == 0) currentPlayerImage = steveDamage1;
			else if (player.animFrame == 1) currentPlayerImage = steveDamage2;
			else                            currentPlayerImage = steveDamage3;
		}
		else if (player.isAttacking) {
			if (player.attackFrame > 8) currentPlayerImage = stevePreAttack;
			else if (player.attackFrame > 4) currentPlayerImage = steveShoot;
			else                             currentPlayerImage = stevePostShoot;
		}
		else if (isKeyPressed('a') || isKeyPressed('d')) {
			int f = player.animFrame % 6;
			if (f == 0) currentPlayerImage = steveRun1;
			else if (f == 1) currentPlayerImage = steveRun2;
			else if (f == 2) currentPlayerImage = steveRun3;
			else if (f == 3) currentPlayerImage = steveRun4;
			else if (f == 4) currentPlayerImage = steveRun5;
			else             currentPlayerImage = steveRun6;
		}
		else {
			currentPlayerImage = steveIdle;
		}
	}
	else if (player.type == ELEVEN) {
		if (player.jumpPhase != 0) {
			if (player.jumpPhase == 1) currentPlayerImage = elevenPreJump;       
			else if (player.jumpPhase == 2) currentPlayerImage = elevenJumpStart; 
			else if (player.jumpPhase == 3) currentPlayerImage = elevenJumpPeak;  
			else if (player.jumpPhase == 4) {
				if (player.y <= ground + 25) currentPlayerImage = elevenToLand;  
				else currentPlayerImage = elevenJumpDown;                         
			}
			else if (player.jumpPhase == 5) currentPlayerImage = elevenLand;      
		}

		else if (player.animState == STATE_DAMAGE) {
			if (player.animFrame == 0) currentPlayerImage = elevenPreDamage;
			else if (player.animFrame == 1) currentPlayerImage = elevenDamage;
			else                            currentPlayerImage = elevenPostDamage;
		}
		else if (player.isAttacking) {
			if (player.attackFrame > 8) currentPlayerImage = elevenPreAttack;
			else if (player.attackFrame > 4) currentPlayerImage = elevenPreAttack1;
			else                             currentPlayerImage = elevenAttack;
		}
		else if (isKeyPressed('a') || isKeyPressed('d')) {
			int f = player.animFrame % 6;
			if (f == 0) currentPlayerImage = elevenRun1;
			else if (f == 1) currentPlayerImage = elevenRun2;
			else if (f == 2) currentPlayerImage = elevenRun3;
			else if (f == 3) currentPlayerImage = elevenRun4;
			else if (f == 4) currentPlayerImage = elevenRun5;
			else             currentPlayerImage = elevenRun6;
		}
		else {
			currentPlayerImage = elevenIdle;
		}
	}

	if (currentPlayerImage) {
		int drawY = ground;
		int drawX = (int)(player.x - cameraX) - playerWidth / 2;

		iShowImageFlipped(
			drawX,
			(int)player.y,
			playerWidth,
			playerHeight,
			currentPlayerImage,
			playerFacingLeft  
			);

		for (int i = 0; i < MAX_PROJECTILES; i++) {
			if (projectiles[i].active) {
				int screenX = (int)(projectiles[i].x - cameraX);
				int screenY = (int)(projectiles[i].y);

				int trail1 = (int)(projectiles[i].dx * 1.5f);
				int trail2 = (int)(projectiles[i].dx * 0.8f);

				if (projectiles[i].type == STEVE) {
					iSetColor(255, 60, 0);
					iFilledCircle(screenX - trail1, screenY, 10);
					iSetColor(255, 120, 0);
					iFilledCircle(screenX - trail2, screenY, 8);
					iSetColor(255, 150, 0);
					iFilledCircle(screenX, screenY, 7);
					iSetColor(255, 230, 120);
					iFilledCircle(screenX, screenY, 4);
				}
				else if (projectiles[i].type == ELEVEN) {
					iSetColor(0, 70, 200);
					iFilledCircle(screenX - trail1, screenY, 10);
					iSetColor(0, 130, 255);
					iFilledCircle(screenX - trail2, screenY, 8);
					iSetColor(0, 180, 255);
					iFilledCircle(screenX, screenY, 7);
					iSetColor(180, 240, 255);
					iFilledCircle(screenX, screenY, 4);
				}
			}
		}


		for (int i = 0; i < MAX_BOSS_PROJECTILES; i++) {
			if (bossProjectiles[i].active) {
				int screenX = (int)(bossProjectiles[i].x - cameraX);
				int screenY = (int)(bossProjectiles[i].y);

				int trail1 = (int)(bossProjectiles[i].dx * 1.5f);
				int trail2 = (int)(bossProjectiles[i].dx * 0.8f);

				iSetColor(120, 0, 120);
				iFilledCircle(screenX - trail1, screenY, 10);

				iSetColor(180, 0, 180);
				iFilledCircle(screenX - trail2, screenY, 8);

				iSetColor(220, 0, 220);
				iFilledCircle(screenX, screenY, 7);

				iSetColor(255, 150, 255);
				iFilledCircle(screenX, screenY, 4);
			}
		}


	}


	int fillX = 116;
	int fillY = 645;
	int maxFillWidth = 175;
	int fillH = 15;


	iSetColor(0, 128, 0);
	iFilledRectangle(
		fillX,
		fillY,
		(player.health / 100.0) * maxFillWidth,
		fillH
		);

	iShowImage(20, WINDOW_HEIGHT - 80, 300, 60, healthBarImg);

	char scoreTextBuf[50];
	sprintf_s(scoreTextBuf, sizeof(scoreTextBuf), "Score: %d", player.score);
	iSetColor(255, 255, 0);
	iText(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 45, scoreTextBuf);

	int margin = 20;
	int iconW = 70, iconH = 70;

	int iconX = WINDOW_WIDTH - margin - iconW;
	int iconY = margin;

	if (fireImg) {
		iShowImage(iconX, iconY, iconW, iconH, fireImg);
	}

	char bulletsBuf[32];
	sprintf_s(bulletsBuf, sizeof(bulletsBuf), "%d", bulletCount);


	drawBoldItalicOrangeStrokeText(iconX - 60, iconY + 18, bulletsBuf, 0.30f);

	int enemyCount = 0;
	for (int i = 0; i < MAX_ENEMIES; i++) {
		if (enemies[i].active) enemyCount++;
	}
	char enemyText[50];
	sprintf_s(enemyText, sizeof(enemyText), "Enemies: %d", enemyCount);
	iSetColor(255, 255, 255);
	iText(WINDOW_WIDTH - 200, WINDOW_HEIGHT - 85, enemyText);
	if (bulletMsgTimer > 0) {
		char msg[64];
		sprintf_s(msg, "Bullets gained: %d", lastBulletsGained);
		iSetColor(255, 140, 0);
		iText(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT - 140, msg);
	}

	if (shieldMsgTimer > 0) {
		iSetColor(0, 200, 255);
		iText(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT - 170, "Shield gained for 10 sec");
	}
}
void collectCoins() {
	float playerHalfW = playerWidth * 0.25f;
	float playerLeft = player.x - playerHalfW;
	float playerRight = player.x + playerHalfW;
	float playerBottom = player.y;
	float playerTop = player.y + playerHeight * 0.60f;

	float pickupHalf = 20.0f;

	for (int i = 0; i < MAX_COINS; i++) {
		if (!coins[i].active) continue;

		float left = coins[i].x - pickupHalf;
		float right = coins[i].x + pickupHalf;
		float bottom = coins[i].y;
		float top = coins[i].y + 40;

		bool overlapX = (playerRight >= left && playerLeft <= right);
		bool overlapY = (playerTop >= bottom && playerBottom <= top);
		if (!overlapX || !overlapY) continue;

		coins[i].active = false;

		if (coins[i].type == PICKUP_BULLETS) {
			bulletCount += coins[i].amount;
			lastBulletsGained = coins[i].amount;
			bulletMsgTimer = MSG_DURATION_TICKS;
		}
		else if (coins[i].type == PICKUP_SHIELD) {
			shieldActive = true;
			shieldTimer = SHIELD_DURATION_TICKS;
			shieldMsgTimer = MSG_DURATION_TICKS;
		}
	}
}
void drawPause() {
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	iSetColor(255, 255, 0);
	iText(WINDOW_WIDTH / 2 - 50, WINDOW_HEIGHT / 2 + 100, "PAUSED");

	for (int i = 9; i < 11; i++) {
		if (buttons[i].visible) {
			iShowImage(buttons[i].x, buttons[i].y,
				buttons[i].w, buttons[i].h,
				startBtnImg); 
		}
	}
}


void drawGameOver() {
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	iSetColor(255, 0, 0);
	iText(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 + 120, "GAME OVER");

	char finalScore[50];
	sprintf_s(finalScore, sizeof(finalScore), "Final Score: %d", player.score);
	iSetColor(255, 255, 255);
	iText(WINDOW_WIDTH / 2 - 70, WINDOW_HEIGHT / 2 + 50, finalScore);

	bool isHighScore = false;
	for (int i = 0; i < 10; i++) {
		if (player.score > highScores[i]) {
			isHighScore = true;
			break;
		}
	}

	if (isHighScore) {
		iSetColor(255, 255, 0);
		iText(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 10, "NEW HIGH SCORE!");
	}

	if (buttons[8].visible) {
		iShowImage(buttons[8].x, buttons[8].y,
			buttons[8].w, buttons[8].h,
			backBtnImg);
	}
}

void updateGame() {
	if (shieldActive) {
		shieldTimer--;
		if (shieldTimer <= 0) {
			shieldTimer = 0;
			shieldActive = false;
		}
	}

	if (bulletMsgTimer > 0) bulletMsgTimer--;
	if (shieldMsgTimer > 0) shieldMsgTimer--;
	if (currentState != PLAYING || gamePaused) return;

	frameCount++;
	enemySpawnTimer++;

	updateAnimations();


	int spawnRate = 220 - player.score / 20;
	if (spawnRate < 90) spawnRate = 90;
	if (!bossFightStarted && enemySpawnTimer > spawnRate) {
		spawnEnemy();
		enemySpawnTimer = 0;
	}

	float moveX = 0, moveY = 0;


	if (player.isAttacking) {
		player.attackFrame--;
		if (player.attackFrame <= 0) {
			player.attackFrame = 0;
			player.isAttacking = false;
			player.animState = STATE_IDLE;
		}
	}

	if (player.score >= BOSS_TRIGGER_SCORE && !boss.spawned) {
		spawnBoss();
	}


	if (isKeyPressed('a')) {
		moveX -= PLAYER_SPEED;
		player.direction = 1; 
	}
	if (isKeyPressed('d')) {
		moveX += PLAYER_SPEED;
		player.direction = 0; 
	}

	player.x += moveX;


	float halfW = playerWidth * 0.5f;
	if (player.x < halfW) player.x = halfW;
	if (player.x > WORLD_WIDTH - halfW) player.x = WORLD_WIDTH - halfW;


	if (!bossCameraLocked) {

		float leftMargin = 300.0f;
		float rightMargin = 500.0f;

		float screenX = player.x - cameraX;

		if (screenX < leftMargin)  cameraX = player.x - leftMargin;
		if (screenX > rightMargin) cameraX = player.x - rightMargin;


		if (cameraX < 0) cameraX = 0;
		if (cameraX > WORLD_WIDTH - WINDOW_WIDTH)
			cameraX = WORLD_WIDTH - WINDOW_WIDTH;
	}
	else {

		cameraX = bossCameraLockX;
		float halfW = playerWidth * 0.5f;
		if (player.x < bossArenaLeft + halfW)  player.x = bossArenaLeft + halfW;
		if (player.x > bossArenaRight - halfW) player.x = bossArenaRight - halfW;
	}


	if (moveX != 0 && moveY != 0) {
		moveX *= 0.707f;
		moveY *= 0.707f;
	}


	if (player.jumpPhase == 0) player.y = ground;
	else updateJump();

	moveEnemies();
	updateProjectiles();
	checkCollisions();
	collectCoins();
	if (boss.active) {
		updateBoss();
		updateBossProjectiles();
		checkBossCollisions();
	}
	if (boss.spawned && !boss.active && boss.animState != STATE_DYING) {
		addHighScore(player.score);
		currentState = GAME_OVER;
	}
}
int page = 0;

void updateBoss() {
	if (!boss.active) return;

	if (boss.animState == STATE_DYING) {
		boss.animDelay++;
		if (boss.animDelay > 10) {
			boss.animDelay = 0;
			boss.animFrame++;
			if (boss.animFrame > 1) {
				boss.animFrame = 1;
				boss.active = false;
			}
		}
		return;
	}

	if (boss.animState == STATE_DAMAGE) {
		boss.animDelay++;
		if (boss.animDelay > 30) { 
			boss.animDelay = 0;
			boss.animFrame = 0;
			boss.animState = STATE_IDLE;
		}
		return;
	}

	if (boss.attackCooldown > 0)
		boss.attackCooldown--;

	if (bossShootAnimTimer > 0) {
		bossShootAnimTimer--;
		boss.animState = STATE_ATTACK;

		boss.animDelay++;
		if (boss.animDelay > 12) {         
			boss.animDelay = 0;
			boss.animFrame = (boss.animFrame + 1) % 2; 
		}
		return; 
	}
	float dx = player.x - boss.x;
	float dist = fabs(dx);

	boss.facingLeft = (dx < 0);

	
	float shootDistance = 400.0f;

	if (dist > shootDistance) {

		if (dx > 0) boss.x += 1.5f;
		else boss.x -= 1.5f;

		boss.animState = STATE_IDLE; 
	}
	else {
	
		boss.animState = STATE_IDLE;

		if (boss.attackCooldown <= 0) {
			boss.animState = STATE_ATTACK;
			shootBossProjectile();
			boss.attackCooldown = 120; 
		}
	}


	if (bossCameraLocked) {
		float bossWidth = 300.0f * SCALE;
		float bossHalfW = bossWidth * 0.5f;

		if (boss.x < bossArenaLeft + bossHalfW)  boss.x = bossArenaLeft + bossHalfW;
		if (boss.x > bossArenaRight - bossHalfW) boss.x = bossArenaRight - bossHalfW;
	}
	else {
		if (boss.x < 100) boss.x = 100;
		if (boss.x > WORLD_WIDTH - 100) boss.x = WORLD_WIDTH - 100;
	}


	boss.animDelay++;
	if (boss.animDelay > 20) {
		boss.animDelay = 0;
		boss.animFrame++;

		if (boss.animState == STATE_ATTACK && boss.animFrame > 1) boss.animFrame = 0;
		else if (boss.animState == STATE_IDLE) boss.animFrame = 0;
	}
}
void shootBossProjectile() {
	for (int i = 0; i < MAX_BOSS_PROJECTILES; i++) {
		if (!bossProjectiles[i].active) {
			bossProjectiles[i].active = true;
			bossProjectiles[i].x = boss.x;
			bossProjectiles[i].y = ground + 50;

			float dx = player.x - boss.x;
			float len = fabs(dx);

			if (len == 0) len = 1;

			bossProjectiles[i].dx = (dx / len) * BOSS_PROJECTILE_SPEED;
			bossProjectiles[i].dy = 0;
			bossProjectiles[i].frame = 0;
			break;
		}
	}
}
void updateBossProjectiles() {
	for (int i = 0; i < MAX_BOSS_PROJECTILES; i++) {
		if (bossProjectiles[i].active) {
			bossProjectiles[i].x += bossProjectiles[i].dx;
			bossProjectiles[i].y += bossProjectiles[i].dy;
			bossProjectiles[i].frame++;

			if (bossProjectiles[i].x < 0 || bossProjectiles[i].x > WORLD_WIDTH ||
				bossProjectiles[i].y < 0 || bossProjectiles[i].y > WINDOW_HEIGHT) {

				bossProjectiles[i].active = false;
			}
		}
	}
}
void checkBossCollisions() {
	if (!boss.active) return;


	float bossLeft = boss.x - 80;
	float bossRight = boss.x + 80;
	float bossBottom = ground + 20;
	float bossTop = ground + 280;

	for (int i = 0; i < MAX_PROJECTILES; i++) {
		if (!projectiles[i].active) continue;

		float px = projectiles[i].x;
		float py = projectiles[i].y;

		if (px >= bossLeft && px <= bossRight &&
			py >= bossBottom && py <= bossTop) {

			projectiles[i].active = false;

			boss.health--;
			bossHitsTaken++;


			float dropX = boss.x + (boss.facingLeft ? -160.0f : 160.0f);

			if (bossCameraLocked) {
				if (dropX < bossArenaLeft + 40)  dropX = bossArenaLeft + 40;
				if (dropX > bossArenaRight - 40) dropX = bossArenaRight - 40;
			}
			else {
				if (dropX < 40) dropX = 40;
				if (dropX > WORLD_WIDTH - 40) dropX = WORLD_WIDTH - 40;
			}


			while (bossHitsTaken >= nextBossBulletDropHit) {
				int gain = 10 + rand() % 6; 
				spawnBulletPickup(dropX, ground + 40, gain);
				nextBossBulletDropHit += 8;
			}


			while (bossHitsTaken >= nextBossShieldDropHit) {
				spawnShieldPickup(dropX, ground + 40);
				nextBossShieldDropHit += 15;
			}

			if (boss.health <= 0) {
				boss.health = 0;
				boss.animState = STATE_DYING;
				boss.animFrame = 0;
				player.score += 100;
			}
			else {

				bossHitsForShootReset++;
				if (bossHitsForShootReset >= SHOOT_RESET_AFTER_HITS) {
					bossHitsForShootReset = 0;

					boss.animState = STATE_DAMAGE;
					boss.animFrame = 0;
					boss.animDelay = 0;

					boss.attackCooldown = 40;
				}
			}
			if (boss.health <= 0) {
				boss.health = 0;
				boss.animState = STATE_DYING;
				boss.animFrame = 0;
				player.score += 100;
			}
		}
	}


	float playerLeft = player.x - playerWidth * 0.25f;
	float playerRight = player.x + playerWidth * 0.25f;
	float playerBottom = player.y;
	float playerTop = player.y + playerHeight * 0.60f;

	for (int i = 0; i < MAX_BOSS_PROJECTILES; i++) {
		if (!bossProjectiles[i].active) continue;

		float bx = bossProjectiles[i].x;
		float by = bossProjectiles[i].y;

		if (bx >= playerLeft && bx <= playerRight &&
			by >= playerBottom && by <= playerTop) {

			bossProjectiles[i].active = false;
			if (!shieldActive) {
				player.health -= 15; 
				if (player.health < 0) player.health = 0;
			}
			if (!shieldActive) {
				player.animState = STATE_DAMAGE;
				player.animFrame = 0;
				player.animDelay = 0;
			}

			if (player.health <= 0) {
				player.health = 0;
				addHighScore(player.score);
				currentState = GAME_OVER;
			}
		}
	}
}
void drawBoss() {
	int bossWidth = 300 * SCALE;
	int bossHeight = 350 * SCALE;
	if (!boss.spawned || (!boss.active && boss.animState != STATE_DYING)) return;

	unsigned int currentBossImage = bossIdle;

	if (boss.animState == STATE_ATTACK)
		currentBossImage = (boss.animFrame == 0) ? bossAttack1 : bossAttack2;
	else if (boss.animState == STATE_DAMAGE)
		currentBossImage = (boss.animFrame == 0) ? bossHit1 : bossHit2;
	else if (boss.animState == STATE_DYING)
		currentBossImage = (boss.animFrame == 0) ? bossDead1 : bossDead2;
	else
		currentBossImage = bossIdle;

	iShowImageFlipped(
		(int)(boss.x - cameraX) - bossWidth / 2,
		ground,
		bossWidth,
		bossHeight,
		currentBossImage,
		boss.facingLeft
		);


	iSetColor(255, 255, 255);


	iSetColor(60, 0, 60);
	iFilledRectangle(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT - 30, 400, 20);


	iSetColor(180, 0, 180);
	iFilledRectangle(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT - 30,
		(int)(400.0f * boss.health / boss.maxHealth), 20);


	iSetColor(255, 255, 255);
	iRectangle(WINDOW_WIDTH / 2 - 200, WINDOW_HEIGHT - 30, 400, 20);


	iSetColor(255, 255, 255);
	iText(WINDOW_WIDTH / 2 - 30, WINDOW_HEIGHT - 25, "BOSS");

	iSetColor(255, 255, 255);


}
void iDraw() {
	iClear();

	if (page == 0)
	{
		iShowImage(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, homepageBg);
	}
	else if (page == 1)
	{
		switch (currentState) {
		case START_PAGE:
			drawStartPage();
			break;
		case INSTRUCTIONS:
			drawInstructions();
			break;
		case SCORE_PAGE:
			drawScorePage();
			break;
		case CREDITS:
			drawCredits();
			break;
		case CHARACTER_SELECT:
			drawCharacterSelect();
			break;
		case PLAYING:
			drawGame();
			break;
		case PAUSED:
			drawGame();
			drawPause();
			break;
		case GAME_OVER:
			drawGameOver();
			break;
		}
	}
}
void changepage()
{
	if (page == 0)   
	{
		page = 1;
	}
}

void iMouseMove(int mx, int my) {
	mouseX = mx;
	mouseY = my;
	checkButtonHover(mx, my);
}


void iPassiveMouseMove(int mx, int my) {
	mouseX = mx;
	mouseY = my;
	checkButtonHover(mx, my);
}


void iMouse(int button, int state, int x, int y) {

	if (button == 0 && state == 0) { 
		for (int i = 0; i < buttonCount; i++) {
			if (buttons[i].visible &&
				x >= buttons[i].x && x <= buttons[i].x + buttons[i].w &&
				y >= buttons[i].y && y <= buttons[i].y + buttons[i].h) {

				bool validButton = false;

				switch (currentState) {
				case START_PAGE:
					validButton = (i >= 0 && i <= 4);
					break;
				case INSTRUCTIONS:
				case SCORE_PAGE:
				case CREDITS:
					validButton = (i == 7);
					break;
				case CHARACTER_SELECT:
					validButton = (i == 5 || i == 6 || i == 7);
					break;
				case GAME_OVER:
					validButton = (i == 8);
					break;
				case PAUSED:
					validButton = (i == 9 || i == 10);
					break;
				}

				if (validButton && buttons[i].action) {
					buttons[i].action();
				}
			}
		}
	}
}

void iKeyboard(unsigned char key) {

	if (key == ' ') {
		if (currentState == PLAYING && !gamePaused) {
			if (player.y <= ground + 0.1f && player.jumpPhase == 0) {
				startJump();
			}
		}
		return;
	}


	if (key == 'f') {
		if (currentState == PLAYING && !gamePaused && !player.isAttacking && bulletCount > 0) {
			player.isAttacking = true;
			player.attackFrame = 12;

			float dx = (player.direction == 0) ? 1.0f : -1.0f;
			shootProjectile(dx, 0);
			bulletCount--;
		}
		return;
	}


	if (key == 'p' || key == 'P') {
		if (currentState == PLAYING) {
			gamePaused = true;
			currentState = PAUSED;
		}
		else if (currentState == PAUSED) {
			gamePaused = false;
			currentState = PLAYING;
		}
		return;
	}


	if (key == 27) {
		if (currentState == PLAYING || currentState == PAUSED) {
			gamePaused = false;
			currentState = START_PAGE;
		}
		else {
			currentState = START_PAGE;
		}
		return;
	}
}

void iSpecialKeyboard(unsigned char key) {

}


void fixedUpdate() {

}


int main() {
	iInitialize(WINDOW_WIDTH, WINDOW_HEIGHT, "DEMOGORGON - Stranger Things Game");
	initGame();
	iSetTimer(20, updateGame);
	iSetTimer(4000, changepage);

	iStart();
	return 0;
}
