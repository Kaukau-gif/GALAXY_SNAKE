/*
 * renderer.c - SNAKE GALAXY EDITION
 * Tema: espaço sideral, meteoros, superpoder estelar
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#include "renderer.h"
#include "config.h"
#include "symbols.h"
#include "game_state.h"
#include "game.h"
#include "bfs.h"
#include "ranking.h"

/* ========================= INICIALIZAÇÃO ========================= */

// Garante que o handle do console seja capturado e configura o UTF-8
static void init_console_handle() {
    if (hConsole == INVALID_HANDLE_VALUE || hConsole == NULL) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleOutputCP(CP_UTF8);
    }
}

/* ========================= PRIMITIVAS ========================= */

void console_goto(int x, int y) {
    init_console_handle();
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, c);
}

void console_color(int color) {
    init_console_handle();
    SetConsoleTextAttribute(hConsole, color);
}

void console_hide_cursor(void) {
    init_console_handle();
    CONSOLE_CURSOR_INFO ci = { 1, FALSE };
    SetConsoleCursorInfo(hConsole, &ci);
}

void console_show_cursor(void) {
    init_console_handle();
    CONSOLE_CURSOR_INFO ci = { 20, TRUE };
    SetConsoleCursorInfo(hConsole, &ci);
}

void console_clear(void) { 
    system("cls"); 
}

void console_write_at(int x, int y, int color, const char *text) {
    console_goto(x, y);
    console_color(color);
    printf("%s", text);
    console_color(C_RESET);
    fflush(stdout);
}

void console_fill_spaces(int x, int y, int n) {
    console_goto(x, y);
    for (int i = 0; i < n; i++) putchar(' ');
    fflush(stdout);
}

/* ========================= CAIXAS ========================= */

void draw_box_single(int left, int top, int right, int bottom, int color) {
    console_color(color);
    console_goto(left, top);
    printf("%s", BOX_TL);
    for (int x = left + 1; x < right; x++) printf("%s", BOX_H);
    printf("%s", BOX_TR);
    for (int y = top + 1; y < bottom; y++) {
        console_goto(left,  y); printf("%s", BOX_V);
        console_goto(right, y); printf("%s", BOX_V);
    }
    console_goto(left, bottom);
    printf("%s", BOX_BL);
    for (int x = left + 1; x < right; x++) printf("%s", BOX_H);
    printf("%s", BOX_BR);
    console_color(C_RESET);
    fflush(stdout);
}

void draw_box_double(int left, int top, int right, int bottom, int color) {
    console_color(color);
    console_goto(left, top);
    printf("%s", BOX_TLH);
    for (int x = left + 1; x < right; x++) printf("%s", BOX_DH);
    printf("%s", BOX_TRH);
    for (int y = top + 1; y < bottom; y++) {
        console_goto(left,  y); printf("%s", BOX_DV);
        console_goto(right, y); printf("%s", BOX_DV);
    }
    console_goto(left, bottom);
    printf("%s", BOX_BLH);
    for (int x = left + 1; x < right; x++) printf("%s", BOX_DH);
    printf("%s", BOX_BRH);
    console_color(C_RESET);
    fflush(stdout);
}

void draw_separator_double(int left, int y, int right, int color) {
    console_color(color);
    console_goto(left, y);
    printf("%s", BOX_LM);
    for (int x = left + 1; x < right; x++) printf("%s", BOX_DH);
    printf("%s", BOX_RM);
    console_color(C_RESET);
    fflush(stdout);
}

void draw_centered_text(int left, int right, int y, int color, const char *text) {
    int width = right - left - 1;
    int len   = (int)strlen(text);
    int x     = left + 1 + (width - len) / 2;
    if (x < left + 1) x = left + 1;
    console_write_at(x, y, color, text);
}

void clear_inside_box(int left, int top, int right, int bottom) {
    for (int y = top + 1; y < bottom; y++) {
        console_goto(left + 1, y);
        for (int x = left + 1; x < right; x++) putchar(' ');
    }
    fflush(stdout);
}

/* ========================= BARRA DE PROGRESSO ========================= */

void draw_progress_bar(int x, int y, int bar_w, int value, int maxval) {
    if (maxval <= 0) maxval = 1;
    int filled = (value * bar_w) / maxval;
    if (filled > bar_w) filled = bar_w;
    console_goto(x, y);
    console_color(C_BAR_FILL);
    for (int i = 0; i < filled; i++)     printf("%s", BAR_FULL);
    console_color(C_BAR_EMPTY);
    for (int i = filled; i < bar_w; i++) printf("%s", BAR_HALF);
    console_color(C_RESET);
    fflush(stdout);
}

/* ========================= ARENA ========================= */

int arena_col(int logical_x) {
    return ARENA_LEFT + 1 + (logical_x - 1) * 2;
}

static int border_color_for_level(void) {
    if (superActive) return C_SUPERPOWER;
    switch ((level - 1) % 5) {
        case 0: return C_BORDER_L1;
        case 1: return C_BORDER_L2;
        case 2: return C_BORDER_L3;
        case 3: return C_BORDER_L4;
        case 4: return C_BORDER_L5;
    }
    return C_BORDER_L1;
}

void draw_board_border(void) {
    int bc = border_color_for_level();
    console_color(bc);

    console_goto(ARENA_LEFT, TOP);
    printf("%s", BOX_TL);
    for (int x = 0; x < WIDTH * 2; x++) printf("%s", BOX_H);
    printf("%s", BOX_TR);

    for (int y = TOP + 1; y < HEIGHT; y++) {
        console_goto(ARENA_LEFT,  y); printf("%s", BLOCK);
        console_goto(ARENA_RIGHT, y); printf("%s", BLOCK);
    }

    console_goto(ARENA_LEFT, HEIGHT);
    printf("%s", BOX_BL);
    for (int x = 0; x < WIDTH * 2; x++) printf("%s", BOX_H);
    printf("%s", BOX_BR);

    if (survivalMode) {
        console_goto(ARENA_LEFT + 2, TOP);
        console_color(C_METEOR);
        printf(" %s SURVIVAL ", SYM_METEOR);
    }

    console_color(C_RESET);
    fflush(stdout);
}

void draw_board_background(void) {
    for (int y = TOP + 1; y < HEIGHT; y++) {
        console_goto(ARENA_LEFT + 1, y);
        console_color(C_BG);
        for (int x = 0; x < WIDTH; x++) printf("  ");
    }
    console_color(C_RESET);
    draw_stars_background();
    fflush(stdout);
}

void draw_stars_background(void) {
    for (int i = 0; i < STAR_COUNT; i++) {
        int sx = stars[i].x;
        int sy = stars[i].y;

        if (game_is_on_snake(sx, sy)) continue;
        if (game_fruit_at(sx, sy) >= 0) continue;
        if (game_is_obstacle(sx, sy)) continue;
        if (game_is_meteor(sx, sy)) continue;

        console_goto(arena_col(sx), sy);
        console_color(stars[i].bright ? 15 : 8); 
        printf("%s", SYM_STAR_BG);
    }
    console_color(C_RESET);
    fflush(stdout);
}

/* ========================= METEORO ========================= */

void draw_meteor(int x, int y) {
    console_goto(arena_col(x), y);
    console_color(C_METEOR);
    printf("%s", SYM_METEOR);
    console_color(C_RESET);
    fflush(stdout);
}

/* ========================= SUPERPODER HUD ========================= */

void draw_superpower_hud(void) {
    int L = PANEL_X;
    char buf[48];
    if (superActive) {
        int rem = SUPERPOWER_DURATION - (int)(time(NULL) - superStartTime);
        if (rem < 0) rem = 0;
        snprintf(buf, sizeof(buf), "%s SUPER %ds ", SYM_SUPERSTAR, rem);
        console_write_at(L + 2, TOP + 30, C_SUPERPOWER, buf);
    } else if (superUsed) {
        console_write_at(L + 2, TOP + 30, 8, "  SUPER USADO   ");
    } else {
        console_write_at(L + 2, TOP + 30, C_MODE_AUTO, "  Q = SUPERPODER");
    }
}

/* ========================= COBRA ========================= */

void draw_head(int x, int y) {
    console_goto(arena_col(x), y);
    console_color(C_RESET);
    if (superActive)
        printf("%s", SYM_HEAD_SUPER);
    else
        printf("%s", autoMode ? SYM_HEAD_AUTO : SYM_HEAD);
    fflush(stdout);
}

void draw_body(int x, int y) {
    console_goto(arena_col(x), y);
    if (superActive) {
        console_color(C_SUPER_BODY);
        printf("%s", SYM_BODY_SUPER);
    } else {
        console_color(C_RESET);
        printf("%s", SYM_BODY);
    }
    fflush(stdout);
}

void draw_tail_seg(int x, int y) {
    console_goto(arena_col(x), y);
    console_color(superActive ? C_SUPER_BODY : C_RESET);
    printf("%s", SYM_TAIL);
    fflush(stdout);
}

void erase_cell(int x, int y) {
    for (int i = 0; i < STAR_COUNT; i++) {
        if (stars[i].x == x && stars[i].y == y) {
            console_goto(arena_col(x), y);
            console_color(stars[i].bright ? 15 : 8);
            printf("%s", SYM_STAR_BG);
            console_color(C_RESET);
            fflush(stdout);
            return;
        }
    }
    console_goto(arena_col(x), y);
    console_color(C_BG);
    printf("  ");
    console_color(C_RESET);
    fflush(stdout);
}

/* ========================= FRUTAS ========================= */

void draw_fruit(int index) {
    console_goto(arena_col(fruits[index].x), fruits[index].y);
    console_color(fruits[index].color);
    printf("%s", fruits[index].symbol);
    console_color(C_RESET);
    fflush(stdout);
}
/* ========================= COBRA 2 (NOVO) ========================= */

void draw_snake2(int x, int y, int isHead) {
    console_goto(arena_col(x), y);
    if (isHead) {
        console_color(14); // Amarelo brilhante para o Alien
        printf("👽 "); 
    } else {
        console_color(6);  // Laranja/Marrom para o corpo
        printf("🟠 "); 
    }
    fflush(stdout);
}

/* ========================= FANTASMAS (NOVO) ========================= */

void draw_ghosts(void) {
    for (int i = 0; i < 5; i++) {
        if (ghosts[i].active) {
            // Desenha o fantasma com a cor branca (15) ou ciano (11)
            console_write_at(arena_col(ghosts[i].x), ghosts[i].y, 11, "👻 ");
        }
    }
}
/* ========================= OBSTACULOS ========================= */

void draw_obstacle(int x, int y) {
    console_goto(arena_col(x), y);
    console_color(C_OBSTACLE);
    printf("%s ", SYM_OBSTACLE);
    console_color(C_RESET);
    fflush(stdout);
}

void draw_all_obstacles(void) {
    for (int i = 0; i < obstacleCount; i++)
        draw_obstacle(obstacles[i].x, obstacles[i].y);
}

/* ========================= BFS OVERLAY ========================= */

void draw_bfs_visited_overlay(void) {
    if (!showBfsVisit) return;
    for (int i = 0; i < bfs_visited_count; i++) {
        int x = bfs_visited_cells[i].x;
        int y = bfs_visited_cells[i].y;
        if (game_is_on_snake(x, y) || game_fruit_at(x, y) >= 0 || game_is_obstacle(x, y) || bfs_is_on_path(x, y)) 
            continue;
        console_goto(arena_col(x), y);
        console_color(C_BFS_VISIT);
        printf(". ");
        console_color(C_RESET);
    }
    fflush(stdout);
}

void draw_bfs_path_overlay(void) {
    if (!showBfsPath) return;
    for (int i = 0; i < bfs_path_len; i++) {
        int x = bfs_path[i].x;
        int y = bfs_path[i].y;
        if (game_is_on_snake(x, y) || game_fruit_at(x, y) >= 0 || game_is_obstacle(x, y)) 
            continue;
        console_goto(arena_col(x), y);
        console_color(C_BFS_PATH);
        printf("%s ", SYM_PATH);
        console_color(C_RESET);
    }
    fflush(stdout);
}

void clear_bfs_overlay(void) {
    for (int i = 0; i < bfs_visited_count; i++) {
        int x = bfs_visited_cells[i].x;
        int y = bfs_visited_cells[i].y;
        if (game_is_on_snake(x, y) || game_fruit_at(x, y) >= 0 || game_is_obstacle(x, y)) 
            continue;
        erase_cell(x, y);
    }
    console_color(C_RESET);
    fflush(stdout);
}

/* ========================= HUD LATERAL ========================= */

static const char *speed_icon(void) {
    if (speedMs > 110) return SYM_SPEED_SLOW;
    if (speedMs >  85) return SYM_SPEED_MED;
    if (speedMs >  65) return SYM_SPEED_FAST;
    return SYM_SPEED_INS;
}

static const char *speed_label(void) {
    if (speedMs > 110) return "SLOW  ";
    if (speedMs >  85) return "NORMAL";
    if (speedMs >  65) return "FAST  ";
    return "INSANE";
}

void draw_side_panel_frame(void) {
    int L = PANEL_X;
    int R = PANEL_X + PANEL_W;
    draw_box_double(L, TOP, R, HEIGHT + 2, C_PANEL);

    char title[48];
    snprintf(title, sizeof(title), "%s SNAKE GALAXY", SYM_GALAXY);
    draw_centered_text(L, R, TOP + 1, C_TITLE, title);
    draw_separator_double(L, TOP + 2, R, C_PANEL);

    console_write_at(L + 2, TOP +  3, C_PANEL_HDR, "SCORE");
    console_write_at(L + 2, TOP +  5, C_PANEL_HDR, "NIVEL");
    console_write_at(L + 2, TOP +  7, C_PANEL_HDR, "SPEED");
    console_write_at(L + 2, TOP +  9, C_PANEL_HDR, "TEMPO");
    console_write_at(L + 2, TOP + 11, C_PANEL_HDR, "RECORDE");

    draw_separator_double(L, TOP + 13, R, C_PANEL);
    console_write_at(L + 2, TOP + 14, C_PANEL_HDR, "MODO");
    draw_separator_double(L, TOP + 16, R, C_PANEL);

    char bfsline[32];
    snprintf(bfsline, sizeof(bfsline), "%s BFS INFO", SYM_BRAIN);
    console_write_at(L + 2, TOP + 17, C_PANEL_HDR, bfsline);
    draw_separator_double(L, TOP + 21, R, C_PANEL);

    char ctrlline[32];
    snprintf(ctrlline, sizeof(ctrlline), "%s CONTROLES", SYM_CTRL);
    console_write_at(L + 2, TOP + 22, C_PANEL_HDR, ctrlline);
}

void update_side_panel(void) {
    int  L = PANEL_X;
    char buf[64];

    if (lastScore != score) {
        snprintf(buf, sizeof(buf), "%06d", score);
        console_write_at(L + 2, TOP + 4, C_SCORE, buf);
        lastScore = score;
    }

    if (lastLevel != level) {
        snprintf(buf, sizeof(buf), "%d  ", level);
        console_write_at(L + 2, TOP + 6, C_LEVEL, buf);
        if (survivalMode)
            console_write_at(L + 6, TOP + 6, C_METEOR, " SURVIVAL!");
        lastLevel = level;
    }

    {
        int base = (level - 1) * LEVEL_STEP;
        int progress = score - base;
        draw_progress_bar(L + 2, TOP + 7, PANEL_W - 3, progress, LEVEL_STEP);
    }

    snprintf(buf, sizeof(buf), "%s %s", speed_icon(), speed_label());
    console_write_at(L + 2, TOP + 8, C_SPEED, buf);

    {
        int elapsed = (int)(time(NULL) - startTime) - pausedSeconds;
        if (paused) elapsed = (int)(pauseStart - startTime) - pausedSeconds;
        if (elapsed < 0) elapsed = 0;
        if (elapsed != lastElapsed) {
            snprintf(buf, sizeof(buf), "%02d:%02d", elapsed / 60, elapsed % 60);
            console_write_at(L + 2, TOP + 10, C_TIME, buf);
            lastElapsed = elapsed;
        }
    }

    {
        int rec = ranking_best_score();
        snprintf(buf, sizeof(buf), "%06d", rec);
        console_write_at(L + 2, TOP + 12, (score > 0 && score >= rec) ? C_BEST : C_PANEL_VAL, buf);
        if (score > 0 && score >= rec)
            console_write_at(L + 10, TOP + 12, C_BEST, SYM_CROWN);
        else
            console_write_at(L + 10, TOP + 12, C_RESET, "  ");
    }

    if (autoMode) {
        snprintf(buf, sizeof(buf), "%s AUTOMATICO  ", SYM_MENU_ROBOT);
        console_write_at(L + 2, TOP + 15, C_MODE_AUTO, buf);
    } else {
        snprintf(buf, sizeof(buf), "%s MANUAL      ", SYM_CTRL);
        console_write_at(L + 2, TOP + 15, C_MODE_MAN, buf);
    }

    snprintf(buf, sizeof(buf), "Caminho: %3d    ", bfs_path_len);
    console_write_at(L + 2, TOP + 18, C_BFS_INFO, buf);
    snprintf(buf, sizeof(buf), "Visitados: %3d  ", bfs_visited_count);
    console_write_at(L + 2, TOP + 19, C_BFS_INFO, buf);
    snprintf(buf, sizeof(buf), "Frutas: %d  ", fruitsEaten);
    console_write_at(L + 2, TOP + 20, C_PANEL_VAL, buf);

    if (autoMode)
        console_write_at(L + 2, TOP + 23, C_NORMAL, "T  Modo Manual  ");
    else
        console_write_at(L + 2, TOP + 23, C_NORMAL, "WASD  Mover     ");

    console_write_at(L + 2, TOP + 24, C_NORMAL, "V  Caminho BFS  ");
    console_write_at(L + 2, TOP + 25, C_NORMAL, "G  Visitados BFS");
    console_write_at(L + 2, TOP + 26, C_NORMAL, "T  Trocar Modo  ");
    console_write_at(L + 2, TOP + 27, C_NORMAL, "P  Pausar       ");
    console_write_at(L + 2, TOP + 28, C_SUPERPOWER, "Q  Superpoder   ");

    if (lastCombo != comboCount) {
        if (comboCount >= 3) {
            snprintf(buf, sizeof(buf), "%s x%d COMBO!  ", SYM_FIRE, comboCount);
            console_write_at(L + 2, TOP + 29, C_COMBO, buf);
        } else {
            console_fill_spaces(L + 2, TOP + 29, PANEL_W - 3);
        }
        lastCombo = comboCount;
    }

    draw_superpower_hud();

    if (paused) {
        snprintf(buf, sizeof(buf), "%s PAUSADO ", SYM_PAUSE);
        console_write_at(L + 2, TOP + 31, C_PAUSED, buf);
    } else {
        console_fill_spaces(L + 2, TOP + 31, 12);
    }
}

/* ========================= EFEITOS ========================= */

void effect_flash_eat(int rarity) {
    int flashes = rarity + 1;
    int flashColor = (rarity == 2) ? 11 : (rarity == 1 ? 14 : 10);
    for (int f = 0; f < flashes; f++) {
        draw_box_single(ARENA_LEFT, TOP, ARENA_RIGHT, HEIGHT, flashColor);
        Sleep(25);
        draw_box_single(ARENA_LEFT, TOP, ARENA_RIGHT, HEIGHT, border_color_for_level());
        Sleep(25);
    }
    draw_board_border();
}

void effect_superpower_activate(void) {
    for (int i = 0; i < 3; i++) {
        draw_box_single(ARENA_LEFT, TOP, ARENA_RIGHT, HEIGHT, C_SUPERPOWER);
        Sleep(60);
        draw_box_single(ARENA_LEFT, TOP, ARENA_RIGHT, HEIGHT, C_SUPER_BODY);
        Sleep(60);
    }
    draw_board_border();
    int cx = ARENA_LEFT + WIDTH;
    int cy = (TOP + HEIGHT) / 2;
    char msg[] = "  STAR POWER!  ";
    console_write_at(cx - (int)strlen(msg)/2, cy, C_SUPERPOWER, msg);
    Sleep(400);
    for (int x = 1; x <= WIDTH; x++) erase_cell(x, cy);
}

void effect_anim_death(void) {
    int cx = ARENA_LEFT + WIDTH;
    int cy = (TOP + HEIGHT) / 2;
    char msg[] = "  PERDIDO NO ESPACO  ";
    for (int i = 0; i < 4; i++) {
        draw_box_single(ARENA_LEFT, TOP, ARENA_RIGHT, HEIGHT, C_GAMEOVER);
        if (i % 2 == 0)
            console_write_at(cx - (int)strlen(msg)/2, cy, C_METEOR, msg);
        else
            console_fill_spaces(cx - (int)strlen(msg)/2, cy, (int)strlen(msg));
        Sleep(180);
    }
    draw_board_border();
}

void effect_anim_level_up(void) {
    int cx = ARENA_LEFT + WIDTH;
    int cy = (TOP + HEIGHT) / 2;
    char msg[48];
    if (level == 3 && survivalMode == 0)
        snprintf(msg, sizeof(msg), "  SURVIVAL ATIVADO!  ");
    else
        snprintf(msg, sizeof(msg), "  NIVEL %d - WARP!  ", level);

    for (int i = 0; i < 3; i++) {
        console_write_at(cx - (int)strlen(msg)/2, cy, C_FLASH, msg);
        Sleep(120);
        console_fill_spaces(cx - (int)strlen(msg)/2, cy, (int)strlen(msg));
        Sleep(80);
    }
    for (int x = 1; x <= WIDTH; x++) erase_cell(x, cy);
    draw_board_border();
}

/* ========================= PAUSA ========================= */

static void pause_box_coords(int *L, int *R, int *T, int *B) {
    int arenaW = WIDTH * 2;
    int boxW   = 22;
    *L = ARENA_LEFT + 1 + (arenaW - boxW) / 2;
    *R = *L + boxW;
    *T = (TOP + HEIGHT) / 2 - 2;
    *B = *T + 4;
}

void draw_pause_overlay(void) {
    int L, R, T, B;
    pause_box_coords(&L, &R, &T, &B);
    draw_box_double(L, T, R, B, C_PAUSED);
    clear_inside_box(L, T, R, B);
    char pauseTitle[32];
    snprintf(pauseTitle, sizeof(pauseTitle), "%s MISSAO PAUSADA", SYM_PAUSE);
    draw_centered_text(L, R, T + 1, C_PAUSED, pauseTitle);
    draw_centered_text(L, R, T + 2, C_NORMAL, "P para retomar");
}

void clear_pause_overlay(void) {
    int L, R, T, B;
    pause_box_coords(&L, &R, &T, &B);
    for (int y = T; y <= B; y++) {
        console_goto(L, y);
        for (int x = L; x <= R + 1; x++) putchar(' ');
    }
    draw_board_border();
    for (int y = T; y <= B; y++) {
        for (int lx = 1; lx <= WIDTH; lx++) {
            int col = arena_col(lx);
            if (col >= L && col <= R + 1) erase_cell(lx, y);
        }
    }
    draw_all_obstacles();
    for (int i = 0; i < FRUIT_COUNT; i++) draw_fruit(i);
    for (int i = 0; i < snakeLen; i++) {
        if (i == 0) draw_head(snake[i].x, snake[i].y);
        else draw_body(snake[i].x, snake[i].y);
    }
}

/* ========================= TELAS DE MENU E RANKING ========================= */

static void screen_cursor_blink(int x, int y, int selected) {
    static DWORD last = 0;
    static int on = 1;
    DWORD now = GetTickCount();
    if (now - last > 450) { on = !on; last = now; }
    console_goto(x, y);
    if (selected && on) { console_color(C_SUPERPOWER); printf(">"); }
    else { console_color(0); printf(" "); }
    console_color(C_RESET);
}

static void draw_title_art(int cx, int ty) {
    console_write_at(cx - 21, ty + 0, 5,  " ██████╗  ██████╗ ██████╗ ██████╗  █████╗ ");
    console_write_at(cx - 21, ty + 1, 5,  "██╔════╝ ██╔═══██╗██╔══██╗██╔══██╗██╔══██╗");
    console_write_at(cx - 21, ty + 2, 5,  "██║      ██║   ██║██████╔╝██████╔╝███████║");
    console_write_at(cx - 21, ty + 3, 13, "██║      ██║   ██║██╔══██╗██╔══██╗██╔══██║");
    console_write_at(cx - 21, ty + 4, 13, "╚██████╗ ╚██████╔╝██████╔╝██║  ██║██║  ██║");
    console_write_at(cx - 21, ty + 5, 13, " ╚═════╝  ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝");
    draw_centered_text(10, 69, ty + 7, 14, "G A L A X Y   E D I T I O N"); 
}

void screen_render_menu(void) {
    int L = 10, R = 69, T = 1, B = 31;
    int cx = (L + R) / 2;

    // 1. Lista de nomes atualizada (5 itens)
    const char *labels[] = {
        "INICIAR MISSAO", 
        "DUPLA GALACTICA", 
        "PILOTO AUTOMATICO", 
        "RANKING", 
        "DESCONECTAR"
    };

    // 2. Icones para cada modo (usando símbolos que costumam funcionar no terminal)
    const char *icons[] = {"🚀", "👥", "🤖", "🏆", "❌"};
    
    if (menuNeedsRedraw) {
        console_clear();
        draw_box_double(L, T, R, B, 13); // Borda magenta
        draw_title_art(cx, T + 2);
        menuNeedsRedraw = 0;
    }

    static int lastIndex = -1;
    if (lastIndex != menuIndex) {
        // IMPORTANTE: O loop agora vai até 5
        for (int i = 0; i < 5; i++) { 
            int oy = T + 14 + (i * 3); // Ajustei o 'oy' para começar um pouco mais para cima
            
            if (i == menuIndex) {
                // Item Selecionado
                console_write_at(cx - 22, oy, 11, ">>"); // Cor Ciano
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "%s  %-20s", icons[i], labels[i]);
                console_write_at(cx - 15, oy, 15, buffer); // Texto Branco brilhante
            } else {
                // Itens não selecionados
                console_write_at(cx - 22, oy, 0, "  "); // Apaga a seta
                console_write_at(cx - 15, oy, 9, labels[i]); // Azul escuro/cinza
            }
        }
        lastIndex = menuIndex;
    }
    
    // O cursor piscante no início
    screen_cursor_blink(cx - 25, T + 14 + menuIndex * 3, 1);
}

void screen_render_ranking(void) {
    if (!rankingNeedsRedraw) return;
    console_clear();
    int L = 12, R = 67, T = 2, B = 28;
    draw_box_double(L, T, R, B, C_SCORE);
    draw_centered_text(L, R, T + 1, C_SCORE, "HALL OF FAME GALACTICO");
    int count = 0;
    ranking_load(&count);
    for (int i = 0; i < count; i++) {
        char line[64];
        snprintf(line, sizeof(line), "%d. %-16s %06d", i+1, ranking[i].name, ranking[i].score);
        draw_centered_text(L, R, T + 6 + i*2, 15, line);
    }
    rankingNeedsRedraw = 0;
}

void screen_render_gameover(void) {
    int L = 10, R = 69, T = 1, B = 30;
    int cx = (L + R) / 2;
    static int lastGOIndex    = -1;
    static int forceOptRedraw =  1;

    if (gameoverNeedsRedraw) {
        console_clear();
        draw_box_double(L, T, R, B, C_GAMEOVER);
        forceOptRedraw = 1;
        /* Titulo em ASCII art */
        console_write_at(cx - 18, T + 3,  12, " ██████╗  █████╗ ███╗   ███╗███████╗");
        console_write_at(cx - 18, T + 4,  12, "██╔════╝ ██╔══██╗████╗ ████║██╔════╝");
        console_write_at(cx - 18, T + 5,  12, "██║  ███╗███████║██╔████╔██║█████╗  ");
        console_write_at(cx - 18, T + 6,  12, "██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  ");
        console_write_at(cx - 18, T + 7,  12, "╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗");
        console_write_at(cx - 18, T + 8,  12, " ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝");
        console_write_at(cx - 18, T + 9,   9, "          O V E R");

        draw_centered_text(L, R, T + 11, 8, "~ CONEXAO PERDIDA ~");

        /* Pontuacao final */
        char scoreStr[64];
        snprintf(scoreStr, sizeof(scoreStr), "PONTUACAO FINAL:  %d  pts", score);
        draw_centered_text(L, R, T + 14, C_TITLE, scoreStr);

        char levelStr[64];
        snprintf(levelStr, sizeof(levelStr), "NIVEL ALCANCADO:  %d", level);
        draw_centered_text(L, R, T + 16, 11, levelStr);

        /* Linha separadora */
        draw_centered_text(L, R, T + 18, 8, "- - - - - - - - - - - - - - -");

        /* Instrucoes */
        draw_centered_text(L, R, T + 27, 8, "[ W/S ou SETAS para navegar  |  ENTER para confirmar ]");

        gameoverNeedsRedraw = 0;
    }

    /* Opcoes navegaveis (redesenha sempre para refletir selecao) */
    if (lastGOIndex != gameOverIndex || forceOptRedraw) {
        forceOptRedraw = 0;
        int cx2 = (L + R) / 2;
        const char *opts[2]   = { "TENTAR NOVAMENTE", "VOLTAR AO MENU" };
        const char *icons2[2] = { "🔄", "🏠" };
        for (int i = 0; i < 2; i++) {
            int oy = T + 21 + i * 3;
            if (i == gameOverIndex) {
                console_write_at(cx2 - 18, oy, 12, ">>");
                char buf[64];
                snprintf(buf, sizeof(buf), "%s  %-20s", icons2[i], opts[i]);
                console_write_at(cx2 - 11, oy, 15, buf);
            } else {
                console_write_at(cx2 - 18, oy, 0,  "  ");
                console_write_at(cx2 - 11, oy, 8, opts[i]);
            }
        }
        lastGOIndex = gameOverIndex;
    }
    screen_cursor_blink((L + R) / 2 - 21, T + 21 + gameOverIndex * 3, 1);
}