#ifndef INPUT_H
#define INPUT_H

#include "types.h"

/*
 * Cada funcao processa teclas para o estado correspondente e,
 * se necessario, altera *state para transicionar de tela.
 */

void input_menu(GameState *state);
void input_ranking(GameState *state);
void input_game(GameState *state);
void input_gameover(GameState *state);

#endif /* INPUT_H */
