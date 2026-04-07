/*
 * bfs.c
 * Implementacao da busca em largura (BFS) usada para guiar a cobra.
 *
 * ESTRUTURAS DE DADOS (conforme aula):
 *   - bfs_queue[]   : fila circular implicita (array + front/rear)
 *   - visited[][]   : hash espacial — evita revisitar celulas
 *   - parent[][]    : lista encadeada implicita — reconstroi o caminho
 *   - dx[] / dy[]   : adjacencia implicita — 4 direcoes
 */

#include <string.h>
#include "bfs.h"
#include "game_state.h"
#include "game.h"

/* ========================= ESTADO INTERNO ========================= */

static BfsNode bfs_queue[BFS_MAX];
static int     bfs_front = 0;
static int     bfs_rear  = 0;

static int     bfs_visited_grid[WIDTH + 1][HEIGHT + 1];
static BfsNode bfs_parent[WIDTH + 1][HEIGHT + 1];

/* Direcoes: cima, direita, baixo, esquerda */
static const int dx[4] = { 0,  1,  0, -1 };
static const int dy[4] = { -1, 0,  1,  0 };

/* ========================= ESTADO PUBLICO ========================= */

BfsNode bfs_path[BFS_MAX];
int     bfs_path_len = 0;

BfsNode bfs_visited_cells[BFS_MAX];
int     bfs_visited_count = 0;

/* ========================= FILA ========================= */

static void queue_enqueue(int x, int y) {
    bfs_queue[bfs_rear].x = x;
    bfs_queue[bfs_rear].y = y;
    bfs_rear++;
}

static BfsNode queue_dequeue(void) {
    return bfs_queue[bfs_front++];
}

static int queue_is_empty(void) {
    return bfs_front == bfs_rear;
}

/* ========================= UTILITARIO ========================= */

static int cell_is_walkable(int x, int y) {
    if (x < 1 || x > WIDTH || y <= TOP || y >= HEIGHT) return 0;
    if (game_is_obstacle(x, y)) return 0;
    /* Corpo bloqueia (indice > 0); cabeca e' o ponto de partida */
    for (int i = 1; i < snakeLen; i++)
        if (snake[i].x == x && snake[i].y == y) return 0;
    return 1;
}

static void record_visited(int x, int y) {
    if (bfs_visited_count < BFS_MAX) {
        bfs_visited_cells[bfs_visited_count].x = x;
        bfs_visited_cells[bfs_visited_count].y = y;
        bfs_visited_count++;
    }
}

/* ========================= BFS CORE ========================= */

int bfs_run(int sx, int sy, int fx, int fy) {
    /* Reinicia estruturas */
    bfs_front = 0;
    bfs_rear  = 0;
    bfs_path_len      = 0;
    bfs_visited_count = 0;
    memset(bfs_visited_grid, 0, sizeof(bfs_visited_grid));

    /* Enfileira origem */
    queue_enqueue(sx, sy);
    bfs_visited_grid[sx][sy] = 1;
    record_visited(sx, sy);

    while (!queue_is_empty()) {
        BfsNode cur = queue_dequeue();

        if (cur.x == fx && cur.y == fy) {
            /* Reconstroi caminho de volta via parent[][] */
            BfsNode temp[BFS_MAX];
            int     temp_len = 0;
            BfsNode node = { fx, fy };

            while (!(node.x == sx && node.y == sy)) {
                temp[temp_len++] = node;
                node = bfs_parent[node.x][node.y];
            }

            /* Inverte: deixa o caminho na ordem cobra -> fruta */
            bfs_path_len = 0;
            for (int i = temp_len - 1; i >= 0; i--)
                bfs_path[bfs_path_len++] = temp[i];

            return 1;
        }

        /* Expande vizinhos */
        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dx[i];
            int ny = cur.y + dy[i];

            if (!bfs_visited_grid[nx < 0 ? 0 : (nx > WIDTH ? WIDTH : nx)]
                                 [ny < 0 ? 0 : (ny > HEIGHT ? HEIGHT : ny)]
                && cell_is_walkable(nx, ny)) {

                queue_enqueue(nx, ny);
                bfs_visited_grid[nx][ny] = 1;
                bfs_parent[nx][ny] = cur;
                record_visited(nx, ny);
            }
        }
    }

    return 0; /* sem caminho */
}

int bfs_find_nearest_fruit(void) {
    int best_len   = 999999;
    int found      = 0;

    BfsNode best_path[BFS_MAX];
    int     best_path_len = 0;

    BfsNode best_visited[BFS_MAX];
    int     best_visited_count = 0;

    for (int i = 0; i < FRUIT_COUNT; i++) {
        if (bfs_run(snake[0].x, snake[0].y, fruits[i].x, fruits[i].y)) {
            if (bfs_path_len < best_len) {
                best_len         = bfs_path_len;
                best_path_len    = bfs_path_len;
                memcpy(best_path, bfs_path, sizeof(BfsNode) * bfs_path_len);

                best_visited_count = bfs_visited_count;
                memcpy(best_visited, bfs_visited_cells,
                       sizeof(BfsNode) * bfs_visited_count);
                found = 1;
            }
        }
    }

    if (found) {
        bfs_path_len = best_path_len;
        memcpy(bfs_path, best_path, sizeof(BfsNode) * best_path_len);

        bfs_visited_count = best_visited_count;
        memcpy(bfs_visited_cells, best_visited,
               sizeof(BfsNode) * best_visited_count);
    } else {
        bfs_path_len      = 0;
        bfs_visited_count = 0;
    }

    return found;
}

/* ========================= CONSULTAS ========================= */

int bfs_is_on_path(int x, int y) {
    for (int i = 0; i < bfs_path_len; i++)
        if (bfs_path[i].x == x && bfs_path[i].y == y) return 1;
    return 0;
}

int bfs_is_visited(int x, int y) {
    for (int i = 0; i < bfs_visited_count; i++)
        if (bfs_visited_cells[i].x == x && bfs_visited_cells[i].y == y) return 1;
    return 0;
}
