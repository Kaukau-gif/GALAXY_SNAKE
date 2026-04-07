#ifndef RENDERER_H
#define RENDERER_H

/* Primitivas */
void console_goto(int x, int y);
void console_color(int color);
void console_hide_cursor(void);
void console_show_cursor(void);
void console_clear(void);
void console_write_at(int x, int y, int color, const char *text);
void console_fill_spaces(int x, int y, int n);

/* Caixas */
void draw_box_single(int left, int top, int right, int bottom, int color);
void draw_box_double(int left, int top, int right, int bottom, int color);
void draw_separator_double(int left, int y, int right, int color);
void draw_centered_text(int left, int right, int y, int color, const char *text);
void clear_inside_box(int left, int top, int right, int bottom);

/* Barra */
void draw_progress_bar(int x, int y, int bar_w, int value, int maxval);

/* Arena */
int  arena_col(int logical_x);
void draw_board_border(void);
void draw_board_background(void);

/* Galaxia */
void draw_stars_background(void);
void draw_meteor(int x, int y);
void draw_superpower_hud(void);

/* BFS overlay */
void draw_bfs_path_overlay(void);
void draw_bfs_visited_overlay(void);
void clear_bfs_overlay(void);

/* Cobra */
void draw_head(int x, int y);
void draw_body(int x, int y);
void draw_tail_seg(int x, int y);
void erase_cell(int x, int y);
void draw_snake2(int x, int y, int isHead);

/* Frutas */
void draw_fruit(int index);

/* Obstaculos */
void draw_obstacle(int x, int y);
void draw_all_obstacles(void);

/* HUD */
void draw_side_panel_frame(void);
void update_side_panel(void);

/* Efeitos */
void effect_flash_eat(int rarity);
void effect_anim_death(void);
void effect_anim_level_up(void);
void effect_superpower_activate(void);

/* Pausa */
void draw_pause_overlay(void);
void clear_pause_overlay(void);

/* Telas */
void screen_render_menu(void);
void screen_render_ranking(void);
void screen_render_gameover(void);

#endif /* RENDERER_H */
