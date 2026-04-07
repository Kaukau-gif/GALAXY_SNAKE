#ifndef GAME_H
#define GAME_H

void game_init(void);
void game_update(void);

int game_is_on_snake(int x, int y);
int game_is_obstacle(int x, int y);
int game_fruit_at(int x, int y);

void game_spawn_obstacles(void);
void game_spawn_fruit(int index);

/* Meteoros */
void game_init_meteors(void);
void game_update_meteors(void);
int  game_is_meteor(int x, int y);

/* Estrelas de fundo */
void game_init_stars(void);

/* Superpoder */
void game_activate_superpower(void);
void game_update_superpower(void);

#endif /* GAME_H */
