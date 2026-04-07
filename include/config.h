#ifndef CONFIG_H
#define CONFIG_H

/* ========================= DIMENSOES ========================= */

#define WIDTH           40
#define HEIGHT          26
#define TOP              3

#define ARENA_LEFT       1
#define ARENA_RIGHT     (ARENA_LEFT + WIDTH*2 + 1)

#define PANEL_X         (ARENA_RIGHT + 3)
#define PANEL_W         24

#define CON_COLS        (PANEL_X + PANEL_W + 2)
#define CON_ROWS        35

/* ========================= JOGO ========================= */

#define FRUIT_COUNT      6
#define MAX_SNAKE      800
#define MAX_RANKING      5
#define RANKING_FILE    "ranking.txt"
#define OBSTACLE_COUNT  20

#define INITIAL_SPEED  140
#define MIN_SPEED       50
#define LEVEL_STEP      80

/* ========================= BFS ========================= */

#define BFS_MAX     (WIDTH * HEIGHT)

/* ========================= GALAXIA / METEORITOS ========================= */

#define STAR_COUNT       30
#define METEOR_COUNT      5
#define SUPERPOWER_DURATION 8

/* ========================= CORES ========================= */

#define C_RESET      7
#define C_BORDER     11
#define C_PANEL       9
#define C_PANEL_HDR  11
#define C_PANEL_VAL  15
#define C_SCORE      14
#define C_LEVEL      10
#define C_TIME       13
#define C_SPEED      12
#define C_RECORD     10
#define C_RUNNING    10
#define C_PAUSED     12
#define C_BG          0
#define C_TITLE      14
#define C_SELECT     14
#define C_NORMAL      7
#define C_FLASH      15
#define C_FLASH2     14
#define C_GAMEOVER   12
#define C_BEST       10
#define C_WALL        8
#define C_COMBO      12
#define C_BAR_FILL    9
#define C_BAR_EMPTY   8

/* Cores de borda por nivel (tema espacial) */
#define C_BORDER_L1   9
#define C_BORDER_L2   5
#define C_BORDER_L3  11
#define C_BORDER_L4  13
#define C_BORDER_L5   3

/* Cores BFS */
#define C_OBSTACLE    6
#define C_BFS_VISIT   2
#define C_BFS_PATH   14
#define C_MODE_AUTO  10
#define C_MODE_MAN   12
#define C_BFS_INFO   11

/* Cores tema galaxia */
#define C_STAR        8
#define C_STAR_BRIGHT 15
#define C_METEOR     12
#define C_SUPERPOWER 14
#define C_SUPER_BODY 11
#define C_SPACE_BG    0

#endif /* CONFIG_H */
