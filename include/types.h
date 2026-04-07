#ifndef TYPES_H
#define TYPES_H

/* ========================= TIPOS BASICOS ========================= */

typedef struct {
    int x, y;
} Segment;

typedef struct {
    int         x, y, value;
    const char *symbol;
    int         color, rarity;
} Fruit;

typedef struct {
    char name[32];
    int  score;
} RankEntry;

typedef enum {
    STATE_MENU = 0,
    STATE_PLAYING,
    STATE_RANKING,
    STATE_GAMEOVER,
    STATE_EXIT
} GameState;

typedef enum {
    DIR_UP = 0,
    DIR_RIGHT,
    DIR_DOWN,
    DIR_LEFT
} Direction;

/* Alias para o no do BFS */
typedef Segment BfsNode;

/* Meteoro */
typedef struct {
    int x, y;
    int active;
    int speed_tick; /* ticks para mover */
    int tick_count;
} Meteor;

/* Estrela de fundo */
typedef struct {
    int x, y;
    int bright; /* 0=escura, 1=brilhante */
} Star;

#endif /* TYPES_H */
