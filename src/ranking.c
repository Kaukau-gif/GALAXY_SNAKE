/*
 * ranking.c
 * Persistencia e manipulacao do ranking de pontuacoes.
 */

#include <stdio.h>
#include <string.h>
#include "ranking.h"
#include "game_state.h"
#include "config.h"

void ranking_load(int *count) {
    FILE *f = fopen(RANKING_FILE, "r");
    *count = 0;
    if (!f) return;

    while (*count < MAX_RANKING &&
           fscanf(f, "%31[^;];%d\n",
                  ranking[*count].name,
                  &ranking[*count].score) == 2) {
        (*count)++;
    }
    fclose(f);
}

void ranking_save(int count) {
    FILE *f = fopen(RANKING_FILE, "w");
    if (!f) return;

    for (int i = 0; i < count; i++)
        fprintf(f, "%s;%d\n", ranking[i].name, ranking[i].score);

    fclose(f);
}

int ranking_best_score(void) {
    int count = 0;
    ranking_load(&count);
    return count ? ranking[0].score : 0;
}

void ranking_insert(const char *name, int sc) {
    int count = 0;
    ranking_load(&count);

    if (count < MAX_RANKING) {
        strncpy(ranking[count].name, name, 31);
        ranking[count].name[31] = '\0';
        ranking[count].score    = sc;
        count++;
    } else if (sc > ranking[count - 1].score) {
        strncpy(ranking[count - 1].name, name, 31);
        ranking[count - 1].name[31] = '\0';
        ranking[count - 1].score    = sc;
    } else {
        return; /* pontuacao nao entra no ranking */
    }

    /* Ordena por pontuacao decrescente (insertion sort — tamanho fixo pequeno) */
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (ranking[j].score > ranking[i].score) {
                RankEntry tmp = ranking[i];
                ranking[i]   = ranking[j];
                ranking[j]   = tmp;
            }
        }
    }

    if (count > MAX_RANKING) count = MAX_RANKING;
    ranking_save(count);
}
