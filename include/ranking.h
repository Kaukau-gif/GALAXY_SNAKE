#ifndef RANKING_H
#define RANKING_H

/* ========================= API DE RANKING ========================= */

void ranking_load(int *count);
void ranking_save(int count);
int  ranking_best_score(void);
void ranking_insert(const char *name, int sc);

#endif /* RANKING_H */
