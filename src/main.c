/*
 * main.c
 * Ponto de entrada: inicializa o console e roda o loop principal.
 *
 * SNAKE INTELIGENTE - BFS Edition v3.0
 * Compile: gcc src/*.c -Iinclude -o snake_bfs.exe
 * Requer : Windows com terminal moderno (Windows Terminal recomendado)
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

#include "config.h"
#include "types.h"
#include "game_state.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "ranking.h"

/* ========================= UTILITARIO ========================= */

static void trim_newline(char *s) {
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) s[--n] = '\0';
}

/* ========================= INICIALIZACAO DO CONSOLE ========================= */

static int console_init(void) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        printf("Erro ao obter o console.\n");
        return 0;
    }

    COORD bufSize = { (SHORT)CON_COLS, (SHORT)CON_ROWS };
    SetConsoleScreenBufferSize(hConsole, bufSize);

    SMALL_RECT winRect = { 0, 0, (SHORT)(CON_COLS - 1), (SHORT)(CON_ROWS - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &winRect);

    SetConsoleTitleA("SNAKE BFS - Cobra Inteligente");
    console_hide_cursor();
    return 1;
}

/* ========================= LOOP PRINCIPAL ========================= */

int main(void) {
    if (!console_init()) return 1;

    srand((unsigned int)time(NULL));

    GameState state = STATE_MENU;

    while (state != STATE_EXIT) {

        switch (state) {

            case STATE_MENU:
                screen_render_menu();
                input_menu(&state);
                Sleep(30);
                break;

            case STATE_RANKING:
                screen_render_ranking();
                input_ranking(&state);
                Sleep(50);
                break;

            case STATE_PLAYING:
                input_game(&state);
                if (state != STATE_PLAYING) break;

                if (!paused) {
                    game_update();

                    if (gameOver) {
                        effect_anim_death();

                        char name[32];
                        console_show_cursor();

                        console_goto(0, HEIGHT + 2);
                        console_color(C_TITLE);
                        printf("  Digite seu nome: ");
                        fflush(stdout);
                        console_color(C_PANEL_VAL);

                        if (fgets(name, sizeof(name), stdin) != NULL)
                            trim_newline(name);
                        else
                            strcpy(name, "Jogador");

                        if (strlen(name) == 0) strcpy(name, "Jogador");

                        ranking_insert(name, score);
                        console_hide_cursor();

                        gameOverIndex       = 0;
                        gameoverNeedsRedraw = 1;
                        state = STATE_GAMEOVER;

                    } else {
                        int sp = autoMode
                                 ? (speedMs > 80 ? 80 : speedMs)
                                 : speedMs;
                        Sleep(sp);
                    }
                } else {
                    Sleep(50);
                }
                break;

            case STATE_GAMEOVER:
                screen_render_gameover();
                input_gameover(&state);
                Sleep(50);
                break;

            default:
                break;
        }
    }

    console_show_cursor();
    console_clear();
    console_color(C_RESET);
    return 0;
}
