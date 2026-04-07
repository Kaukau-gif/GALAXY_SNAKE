#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <time.h>
#include <windows.h>
#include "types.h"
#include "config.h"

/* ========================= CONSOLE ========================= */
extern HANDLE hConsole;

/* ========================= COBRAS ========================= */
// Player 1
extern Segment  snake[MAX_SNAKE];
extern int      snakeLen;
extern Direction dir;
extern Direction nextDir;

// Player 2 (Novo)
extern Segment  snake2[MAX_SNAKE];
extern int      snakeLen2;
extern Direction dir2, nextDir2;
extern int      isMultiplayer; // 1 = 2 Jogadores, 0 = Solo

/* ========================= FANTASMAS (Novo) ========================= */
typedef struct {
    int x, y;
    int active;
    int color;
} Ghost;

extern Ghost ghosts[5]; 

/* ========================= FRUTAS / OBSTACULOS ========================= */
extern Fruit fruits[FRUIT_COUNT];
extern Segment obstacles[OBSTACLE_COUNT];
extern int     obstacleCount;

/* ========================= RANKING ========================= */
extern RankEntry ranking[MAX_RANKING];

/* ========================= PONTUACAO / PROGRESSO ========================= */
extern int score;
extern int level;
extern int speedMs;
extern int fruitsEaten;
extern int comboCount;

/* ========================= FLAGS DE ESTADO ========================= */
extern int paused;
extern int gameOver;
extern int autoMode;
extern int showBfsPath;
extern int showBfsVisit;

/* ========================= SUPERPODER ========================= */
extern int superActive;        /* 1 = superpoder ativo */
extern time_t superStartTime;  /* quando comecou */
extern int superUsed;          /* ja usou nesta vida */

/* ========================= METEOROS ========================= */
extern Meteor meteors[METEOR_COUNT];

/* ========================= ESTRELAS DE FUNDO ========================= */
extern Star stars[STAR_COUNT];

/* ========================= MENUS ========================= */
extern int menuIndex;
extern int gameOverIndex;
extern int menuNeedsRedraw;
extern int gameoverNeedsRedraw;
extern int rankingNeedsRedraw;

/* ========================= TEMPO ========================= */
extern time_t startTime;
extern time_t pauseStart;
extern int    pausedSeconds;

/* ========================= HUD (cache) ========================= */
extern int lastElapsed;
extern int lastLevel;
extern int lastScore;
extern int lastFruits;
extern int lastCombo;

/* ========================= FASE SOBREVIVENCIA ========================= */
extern int survivalMode;       /* 1 = modo sobrevivencia ativo */
extern int meteorWarning;      /* pisca aviso de meteoro */

#endif /* GAME_STATE_H */
