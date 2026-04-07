/*
 * game_state.c
 * Definicao das variaveis globais. Nenhuma logica aqui.
 */

#include "game_state.h"
HANDLE hConsole = INVALID_HANDLE_VALUE;

Segment  snake[MAX_SNAKE];
int      snakeLen  = 0;
Direction dir      = DIR_RIGHT;
Direction nextDir  = DIR_RIGHT;

Fruit fruits[FRUIT_COUNT];

Segment obstacles[OBSTACLE_COUNT];
int     obstacleCount = 0;

RankEntry ranking[MAX_RANKING];

int score       = 0;
int level       = 1;
int speedMs     = INITIAL_SPEED;
int fruitsEaten = 0;
int comboCount  = 0;

int paused       = 0;
int gameOver     = 0;
int autoMode     = 0;
int showBfsPath  = 1;
int showBfsVisit = 0;

/* Superpoder */
int    superActive    = 0;
time_t superStartTime = 0;
int    superUsed      = 0;

/* Meteoros */
Meteor meteors[METEOR_COUNT];

/* Estrelas */
Star stars[STAR_COUNT];

int menuIndex      = 0;
int gameOverIndex  = 0;
int menuNeedsRedraw     = 1;
int gameoverNeedsRedraw = 1;
int rankingNeedsRedraw  = 1;

time_t startTime     = 0;
time_t pauseStart    = 0;
int    pausedSeconds = 0;

int lastElapsed = -1;
int lastLevel   = -1;
int lastScore   = -1;
int lastFruits  = -1;
int lastCombo   = -1;

int survivalMode  = 0;
int meteorWarning = 0;

/* Multiplayer - Cobra 2 */
Segment  snake2[MAX_SNAKE];
int      snakeLen2 = 0;
Direction dir2     = DIR_LEFT;
Direction nextDir2 = DIR_LEFT;
int      isMultiplayer = 0;

/* Fantasmas */
Ghost ghosts[5];
