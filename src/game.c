/*
 * game.c
 * Logica central: inicializacao, spawning, meteoros, superpoder, update.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "game.h"
#include "game_state.h"
#include "config.h"
#include "renderer.h"
#include "bfs.h"

/* ========================= TABELA DE FRUTAS ESPACIAIS ========================= */

static const struct {
    int         value;
    const char *symbol;
    int         color;
    int         rarity;
} FRUIT_TABLE[] = {
    { 10, "\xF0\x9F\x8C\x9F", 14, 0 },  /* Estrela     */
    { 20, "\xF0\x9F\x92\x8E", 13, 0 },  /* Cristal     */
    { 30, "\xF0\x9F\xA7\xAA",  9, 0 },  /* Tubo        */
    { 50, "\xF0\x9F\x9B\xB8", 14, 1 },  /* OVNI        */
    { 80, "\xF0\x9F\x91\xBE", 11, 2 },  /* Alien raro  */
};
#define FRUIT_TABLE_LEN 5

/* ========================= ESTRELAS DE FUNDO ========================= */

void game_init_stars(void) {
    for (int i = 0; i < STAR_COUNT; i++) {
        stars[i].x      = rand() % WIDTH + 1;
        stars[i].y      = rand() % (HEIGHT - TOP - 1) + TOP + 1;
        stars[i].bright = (rand() % 4 == 0) ? 1 : 0;
    }
}

/* ========================= METEOROS ========================= */

void game_init_meteors(void) {
    for (int i = 0; i < METEOR_COUNT; i++) {
        meteors[i].active     = 0;
        meteors[i].x          = 0;
        meteors[i].y          = 0;
        meteors[i].speed_tick = 3 + rand() % 4;
        meteors[i].tick_count = 0;
    }
}

static void spawn_meteor(int i) {
    meteors[i].x         = rand() % WIDTH + 1;
    meteors[i].y         = TOP + 1;
    meteors[i].active    = 1;
    meteors[i].speed_tick = 2 + rand() % 3;
    meteors[i].tick_count = 0;
}

int game_is_meteor(int x, int y) {
    for (int i = 0; i < METEOR_COUNT; i++)
        if (meteors[i].active && meteors[i].x == x && meteors[i].y == y) return 1;
    return 0;
}

void game_update_meteors(void) {
    if (!survivalMode) return;

    for (int i = 0; i < METEOR_COUNT; i++) {
        if (!meteors[i].active) {
            if (rand() % 20 == 0) spawn_meteor(i);
            continue;
        }

        meteors[i].tick_count++;
        if (meteors[i].tick_count < meteors[i].speed_tick) continue;
        meteors[i].tick_count = 0;

        erase_cell(meteors[i].x, meteors[i].y);
        meteors[i].y++;

        if (meteors[i].y >= HEIGHT) {
            meteors[i].active = 0;
            continue;
        }

        if (!superActive && game_is_on_snake(meteors[i].x, meteors[i].y)) {
            gameOver = 1;
            meteors[i].active = 0;
            return;
        }

        draw_meteor(meteors[i].x, meteors[i].y);
    }
}

/* ========================= SUPERPODER ========================= */

void game_activate_superpower(void) {
    if (superUsed || superActive) return;
    superActive    = 1;
    superStartTime = time(NULL);
    superUsed      = 1;
    draw_head(snake[0].x, snake[0].y);
    draw_superpower_hud();
}

void game_update_superpower(void) {
    if (!superActive) return;
    int elapsed = (int)(time(NULL) - superStartTime);
    if (elapsed >= SUPERPOWER_DURATION) {
        superActive = 0;
        draw_head(snake[0].x, snake[0].y);
        draw_superpower_hud();
    }
}

/* ========================= UTILITARIOS ========================= */

int game_is_on_snake(int x, int y) {
    for (int i = 0; i < snakeLen; i++)
        if (snake[i].x == x && snake[i].y == y) return 1;
    if (isMultiplayer) {
        for (int i = 0; i < snakeLen2; i++)
            if (snake2[i].x == x && snake2[i].y == y) return 1;
    }
    return 0;
}

int game_is_obstacle(int x, int y) {
    for (int i = 0; i < obstacleCount; i++)
        if (obstacles[i].x == x && obstacles[i].y == y) return 1;
    return 0;
}

int game_fruit_at(int x, int y) {
    for (int i = 0; i < FRUIT_COUNT; i++)
        if (fruits[i].x == x && fruits[i].y == y) return i;
    return -1;
}

/* ========================= OBSTACULOS ========================= */

void game_spawn_obstacles(void) {
    obstacleCount = 0;
    int mx = WIDTH / 2;
    int my = (TOP + HEIGHT) / 2;

    for (int i = 0; i < OBSTACLE_COUNT; i++) {
        int x, y, ok;
        int attempts = 0;
        do {
            ok = 1;
            x = rand() % WIDTH + 1;
            y = rand() % (HEIGHT - TOP - 1) + TOP + 1;
            if (abs(x - mx) <= 5 && abs(y - my) <= 2) { ok = 0; continue; }
            if (game_is_on_snake(x, y))  { ok = 0; continue; }
            if (game_is_obstacle(x, y))  { ok = 0; continue; }
            attempts++;
        } while (!ok && attempts <= 200);

        if (ok) {
            obstacles[obstacleCount].x = x;
            obstacles[obstacleCount].y = y;
            obstacleCount++;
        }
    }
}

/* ========================= FRUTAS ========================= */

static void spawn_fruit_anim(int i) {
    int col = arena_col(fruits[i].x);
    int y   = fruits[i].y;
    for (int f = 0; f < 2; f++) {
        console_goto(col, y);
        console_color(C_FLASH);
        printf("* ");
        fflush(stdout);
        Sleep(40);
        erase_cell(fruits[i].x, y);
        Sleep(30);
    }
    draw_fruit(i);
}

void game_spawn_fruit(int i) {
    int x, y, ok;
    do {
        ok = 1;
        x = rand() % WIDTH + 1;
        y = rand() % (HEIGHT - TOP - 1) + TOP + 1;
        if (game_is_on_snake(x, y)) { ok = 0; continue; }
        if (game_is_obstacle(x, y)) { ok = 0; continue; }
        for (int j = 0; j < FRUIT_COUNT; j++) {
            if (j != i && fruits[j].x == x && fruits[j].y == y) { ok = 0; break; }
        }
    } while (!ok);

    int roll = rand() % 100;
    int fi;
    if      (roll < 10) fi = 4;
    else if (roll < 40) fi = 3;
    else                fi = rand() % 3;

    fruits[i].x      = x;
    fruits[i].y      = y;
    fruits[i].value  = FRUIT_TABLE[fi].value;
    fruits[i].symbol = FRUIT_TABLE[fi].symbol;
    fruits[i].color  = FRUIT_TABLE[fi].color;
    fruits[i].rarity = FRUIT_TABLE[fi].rarity;

    spawn_fruit_anim(i);
}

/* ========================= INICIALIZACAO ========================= */

void game_init(void) {
    snakeLen      = 5;
    score         = 0;
    level         = 1;
    speedMs       = INITIAL_SPEED;
    paused        = 0;
    gameOver      = 0;
    pausedSeconds = 0;
    lastElapsed   = -1;
    lastLevel     = -1;
    lastScore     = -1;
    lastFruits    = -1;
    lastCombo     = -1;
    fruitsEaten   = 0;
    comboCount    = 0;
    superActive   = 0;
    superUsed     = 0;
    bfs_path_len      = 0;
    bfs_visited_count = 0;

    int mx = WIDTH  / 2;
    int my = (TOP + HEIGHT) / 2;
    for (int i = 0; i < snakeLen; i++) {
        snake[i].x = mx - i;
        snake[i].y = my;
    }

    dir     = DIR_RIGHT;
    nextDir = DIR_RIGHT;

    if (isMultiplayer) {
        snakeLen2 = 5;
        int mx2 = WIDTH  / 2;
        int my2 = (TOP + HEIGHT) / 2 + 3;
        for (int i = 0; i < snakeLen2; i++) {
            snake2[i].x = mx2 + i;
            snake2[i].y = my2;
        }
        dir2     = DIR_LEFT;
        nextDir2 = DIR_LEFT;
    }

    game_init_stars();
    game_init_meteors();
    console_clear();
    game_spawn_obstacles();
    draw_board_border();
    draw_board_background();
    draw_all_obstacles();
    draw_side_panel_frame();

    for (int i = snakeLen - 1; i >= 0; i--) {
        if (i == 0)             draw_head(snake[i].x, snake[i].y);
        else if (i==snakeLen-1) draw_tail_seg(snake[i].x, snake[i].y);
        else                    draw_body(snake[i].x, snake[i].y);
    }

    if (isMultiplayer) {
        for (int i = snakeLen2 - 1; i >= 0; i--) {
            draw_snake2(snake2[i].x, snake2[i].y, i == 0);
        }
    }

    for (int i = 0; i < FRUIT_COUNT; i++) game_spawn_fruit(i);

    bfs_find_nearest_fruit();
    draw_bfs_visited_overlay();
    draw_bfs_path_overlay();
    startTime = time(NULL);
    update_side_panel();
}

/* ========================= UPDATE ========================= */

static Direction direction_from_bfs(void) {
    if (bfs_path_len == 0) return dir;
    int ddx = bfs_path[0].x - snake[0].x;
    int ddy = bfs_path[0].y - snake[0].y;
    if (ddy == -1) return DIR_UP;
    if (ddx ==  1) return DIR_RIGHT;
    if (ddy ==  1) return DIR_DOWN;
    if (ddx == -1) return DIR_LEFT;
    return dir;
}

void game_update(void) {
    if (paused || gameOver) return;

    /* --- CORREÇÃO: SE ATIVAR AUTOMÁTICO, DESLIGA MULTIPLAYER E LIMPA COBRA 2 --- */
    if (autoMode && isMultiplayer) {
        isMultiplayer = 0;
        // Apaga a cobra 2 inteira da tela para não deixar rastro
        for (int i = 0; i < snakeLen2; i++) {
            erase_cell(snake2[i].x, snake2[i].y);
        }
        snakeLen2 = 0; // Reseta o tamanho para garantir que não processe colisões
    }

    clear_bfs_overlay();
    game_update_superpower();
    game_update_meteors();

    /* --- LÓGICA DE DIREÇÃO --- */
    if (autoMode) {
        bfs_find_nearest_fruit();
        nextDir = direction_from_bfs();
    }
    dir = nextDir;

    /* --- LOGICA COBRA 1 --- */
    int nx = snake[0].x;
    int ny = snake[0].y;
    if      (dir == DIR_UP)    ny--;
    else if (dir == DIR_RIGHT) nx++;
    else if (dir == DIR_DOWN)  ny++;
    else if (dir == DIR_LEFT)  nx--;

    // Colisões Cobra 1
    if (nx < 1 || nx > WIDTH || ny <= TOP || ny >= HEIGHT) {
        if (superActive) {
            if (nx < 1) nx = WIDTH;
            if (nx > WIDTH) nx = 1;
            if (ny <= TOP) ny = HEIGHT - 1;
            if (ny >= HEIGHT) ny = TOP + 1;
        } else { gameOver = 1; return; }
    }
    
    if (game_is_obstacle(nx, ny) && !superActive) { gameOver = 1; return; }
    if (survivalMode && game_is_meteor(nx, ny) && !superActive) { gameOver = 1; return; }

    int fruitIndex = game_fruit_at(nx, ny);
    if (!superActive) {
        int limit = (fruitIndex == -1) ? snakeLen - 1 : snakeLen;
        for (int i = 0; i < limit; i++)
            if (snake[i].x == nx && snake[i].y == ny) { gameOver = 1; return; }
    }

    // Movimentação Cobra 1
    draw_body(snake[0].x, snake[0].y);
    Segment oldTail = snake[snakeLen - 1];

    for (int i = snakeLen; i > 0; i--) snake[i] = snake[i - 1];
    snake[0].x = nx;
    snake[0].y = ny;

    if (fruitIndex != -1) {
        score += fruits[fruitIndex].value;
        snakeLen++;
        if (snakeLen >= MAX_SNAKE) snakeLen = MAX_SNAKE - 1;
        fruitsEaten++;
        comboCount++;
        
        int newLevel = score / LEVEL_STEP + 1;
        if (newLevel > level) {
            level = newLevel;
            speedMs = (speedMs - 8 < MIN_SPEED) ? MIN_SPEED : speedMs - 8;
            if (level >= 3) survivalMode = 1;
            effect_anim_level_up();
            draw_board_border();
        }
        game_spawn_fruit(fruitIndex);
        draw_tail_seg(snake[snakeLen-1].x, snake[snakeLen-1].y);
        effect_flash_eat(fruits[fruitIndex].rarity);
    } else {
        erase_cell(oldTail.x, oldTail.y);
        draw_tail_seg(snake[snakeLen-1].x, snake[snakeLen-1].y);
    }
    draw_head(nx, ny);

    /* --- LOGICA COBRA 2 (APENAS SE MULTIPLAYER ATIVO E AUTO OFF) --- */
    if (isMultiplayer && !autoMode) {
        dir2 = nextDir2;
        int nx2 = snake2[0].x;
        int ny2 = snake2[0].y;
        if      (dir2 == DIR_UP)    ny2--;
        else if (dir2 == DIR_RIGHT) nx2++;
        else if (dir2 == DIR_DOWN)  ny2++;
        else if (dir2 == DIR_LEFT)  nx2--;

        if (nx2 < 1 || nx2 > WIDTH || ny2 <= TOP || ny2 >= HEIGHT || game_is_obstacle(nx2, ny2)) {
            gameOver = 1; return;
        }
        
        // Cobra 2 bate na Cobra 1
        for (int i = 0; i < snakeLen; i++) 
            if (snake[i].x == nx2 && snake[i].y == ny2) { gameOver = 1; return; }

        int fruitIndex2 = game_fruit_at(nx2, ny2);
        
        // Cobra 2 bate em si mesma
        int limit2 = (fruitIndex2 == -1) ? snakeLen2 - 1 : snakeLen2;
        for (int i = 0; i < limit2; i++)
            if (snake2[i].x == nx2 && snake2[i].y == ny2) { gameOver = 1; return; }

        draw_snake2(snake2[0].x, snake2[0].y, 0); 
        Segment oldTail2 = snake2[snakeLen2 - 1];

        for (int i = snakeLen2; i > 0; i--) snake2[i] = snake2[i - 1];
        snake2[0].x = nx2;
        snake2[0].y = ny2;

        if (fruitIndex2 != -1) {
            score += fruits[fruitIndex2].value;
            snakeLen2++;
            game_spawn_fruit(fruitIndex2);
            draw_snake2(snake2[snakeLen2-1].x, snake2[snakeLen2-1].y, 0);
        } else {
            erase_cell(oldTail2.x, oldTail2.y);
            draw_snake2(snake2[snakeLen2-1].x, snake2[snakeLen2-1].y, 0);
        }
        draw_snake2(nx2, ny2, 1);
    }

    /* --- FINALIZAÇÃO E INTERFACE --- */
    if (autoMode) {
        // Só calcula e desenha o rastro do BFS se o robô estiver ativo
        bfs_find_nearest_fruit();
        draw_bfs_visited_overlay();
        draw_bfs_path_overlay();
    }

    int elapsed = (int)(time(NULL) - startTime) - pausedSeconds;
    update_side_panel(); // Atualiza o painel lateral
}