#ifndef BFS_H
#define BFS_H

#include "types.h"
#include "config.h"

/* ========================= ESTADO PUBLICO DO BFS ========================= */

extern BfsNode bfs_path[BFS_MAX];
extern int     bfs_path_len;

extern BfsNode bfs_visited_cells[BFS_MAX];
extern int     bfs_visited_count;

/* ========================= API PUBLICA ========================= */

/*
 * Executa BFS de (sx,sy) ate (fx,fy).
 * Retorna 1 se encontrou caminho; 0 caso contrario.
 * O caminho fica em bfs_path[] / bfs_path_len.
 */
int  bfs_run(int sx, int sy, int fx, int fy);

/*
 * Executa BFS para todas as frutas e mantém o caminho
 * ate a mais proxima.
 * Retorna 1 se ao menos uma fruta foi alcançada.
 */
int  bfs_find_nearest_fruit(void);

/* Consultas sobre o ultimo resultado */
int  bfs_is_on_path(int x, int y);
int  bfs_is_visited(int x, int y);

#endif /* BFS_H */
