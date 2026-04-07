/*
 * input.c - SNAKE GALAXY EDITION
 * Atualizado com suporte a Multiplayer (Dupla Galáctica) e Auto-limpeza do Robô
 */

#include <conio.h>
#include <windows.h>
#include <time.h>
#include "input.h"
#include "game_state.h"
#include "game.h"
#include "renderer.h"
#include "bfs.h"

/* ========================= INPUT DO MENU ========================= */
void input_menu(GameState *state) {
    if (!_kbhit()) return;
    int k = _getch();

    // Navegação por Setas ou Teclas Especiais
    if (k == 0 || k == 224) {
        k = _getch();
        if      (k == 72) { menuIndex--; if (menuIndex < 0) menuIndex = 4; }
        else if (k == 80) { menuIndex++; if (menuIndex > 4) menuIndex = 0; }
        return;
    }

    // Atalhos por Letras ou Números
    if      (k == 'w' || k == 'W') { menuIndex--; if (menuIndex < 0) menuIndex = 4; }
    else if (k == 's' || k == 'S') { menuIndex++; if (menuIndex > 4) menuIndex = 0; }
    else if (k == '1') { isMultiplayer = 0; autoMode = 0; game_init(); *state = STATE_PLAYING; }
    else if (k == '2') { isMultiplayer = 1; autoMode = 0; game_init(); *state = STATE_PLAYING; }
    else if (k == '3') { isMultiplayer = 0; autoMode = 1; game_init(); *state = STATE_PLAYING; }
    else if (k == '4') { rankingNeedsRedraw = 1; *state = STATE_RANKING; }
    else if (k == '5' || k == 27) { *state = STATE_EXIT; }
    
    else if (k == 13) {
        menuNeedsRedraw = 1;
        switch (menuIndex) {
            case 0: isMultiplayer = 0; autoMode = 0; game_init(); *state = STATE_PLAYING; break;
            case 1: isMultiplayer = 1; autoMode = 0; game_init(); *state = STATE_PLAYING; break; 
            case 2: isMultiplayer = 0; autoMode = 1; game_init(); *state = STATE_PLAYING; break;
            case 3: rankingNeedsRedraw = 1; *state = STATE_RANKING; break;
            default: *state = STATE_EXIT; break;
        }
    }
}

/* ========================= INPUT DURANTE O JOGO ========================= */
void input_game(GameState *state) {
    if (!_kbhit()) return;
    int k = _getch();

    // --- CONTROLES ESPECIAIS (SETAS) ---
    if (k == 0 || k == 224) {
        k = _getch();
        if (isMultiplayer) {
            if      (k == 72 && dir2 != DIR_DOWN)  nextDir2 = DIR_UP;
            else if (k == 77 && dir2 != DIR_LEFT)  nextDir2 = DIR_RIGHT;
            else if (k == 80 && dir2 != DIR_UP)    nextDir2 = DIR_DOWN;
            else if (k == 75 && dir2 != DIR_RIGHT) nextDir2 = DIR_LEFT;
        } else if (!autoMode) {
            if      (k == 72 && dir != DIR_DOWN)  nextDir = DIR_UP;
            else if (k == 77 && dir != DIR_LEFT)  nextDir = DIR_RIGHT;
            else if (k == 80 && dir != DIR_UP)    nextDir = DIR_DOWN;
            else if (k == 75 && dir != DIR_RIGHT) nextDir = DIR_LEFT;
        }
        return;
    }

    // --- CONTROLES WASD (PLAYER 1) ---
    if (!autoMode) {
        if      ((k == 'w' || k == 'W') && dir != DIR_DOWN)  nextDir = DIR_UP;
        else if ((k == 'd' || k == 'D') && dir != DIR_LEFT)  nextDir = DIR_RIGHT;
        else if ((k == 's' || k == 'S') && dir != DIR_UP)    nextDir = DIR_DOWN;
        else if ((k == 'a' || k == 'A') && dir != DIR_RIGHT) nextDir = DIR_LEFT;
    }

    // --- TECLAS DE FUNÇÃO ---
    if (k == 'q' || k == 'Q') {
        if (!superUsed && !superActive) {
            game_activate_superpower();
        }
    }
    else if (k == 'p' || k == 'P') {
        if (!paused) {
            paused = 1;
            pauseStart = time(NULL);
            update_side_panel();
            draw_pause_overlay();
        } else {
            paused = 0;
            pausedSeconds += (int)(time(NULL) - pauseStart);
            clear_pause_overlay();
            update_side_panel();
        }
    }
    else if (k == 'v' || k == 'V') {
        showBfsPath = !showBfsPath;
        if (!showBfsPath) clear_bfs_overlay(); 
        else draw_bfs_path_overlay();
    }
    else if (k == 'g' || k == 'G') {
        showBfsVisit = !showBfsVisit;
        if (!showBfsVisit) clear_bfs_overlay();
        else draw_bfs_visited_overlay();
    }
    
    // T - Trocar Modo de Pilotagem (Manual/Auto)
    // CORREÇÃO: Limpa a Cobra 2 imediatamente se o robô for ativado
    else if (k == 't' || k == 'T') {
        autoMode = !autoMode;
        
        if (autoMode) {
            // Se o modo robô ligou, mata a existência da cobra 2 na tela e na memória
            if (isMultiplayer) {
                isMultiplayer = 0; 
                for (int i = 0; i < snakeLen2; i++) {
                    erase_cell(snake2[i].x, snake2[i].y);
                }
                snakeLen2 = 0; 
            }
        }
        update_side_panel();
    }
}

/* ========================= INPUT RANKING / GAMEOVER ========================= */
void input_ranking(GameState *state) {
    if (!_kbhit()) return;
    int k = _getch();
    if (k == 13 || k == 27 || k == 'm' || k == 'M') {
        menuNeedsRedraw = 1;
        *state = STATE_MENU;
    }
}

void input_gameover(GameState *state) {
    if (!_kbhit()) return;
    int k = _getch();

    if (k == 0 || k == 224) {
        k = _getch();
        if (k == 72) gameOverIndex = 0; 
        else if (k == 80) gameOverIndex = 1; 
        return;
    }

    if (k == 'w' || k == 'W') gameOverIndex = 0;
    else if (k == 's' || k == 'S') gameOverIndex = 1;
    else if (k == 13) {
        survivalMode = 0;
        if (gameOverIndex == 0) {
            game_init(); 
            *state = STATE_PLAYING;
        } else {
            menuNeedsRedraw = 1; 
            *state = STATE_MENU;
        }
    }
}