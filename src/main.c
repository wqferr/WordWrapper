#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#define INF (INT_MAX >> 1)

typedef struct Word {
    char *str;
    size_t len;
} Word;

char *getWord(size_t *n) {
    size_t b = 20 * sizeof(char);
    char *w = malloc(b);
    *n = getline(&w, &b, stdin);
    w[*n] = '\0';
    return w;
}

int main(void) {
    Word *words;
    int n, l;
    int i, j;
    int **cost;

    scanf("%d\n%d\n", &l, &n);
    words = malloc(n * sizeof(*words));

    for (i = 0; i < n; i++)
        words[i].str = getWord(&words[i].len);

    cost = malloc(n * sizeof(*cost));
    for (i = 0; i <= l; i++) {
        cost[i] = malloc(n * sizeof(**cost));

        cost[i][i] = -1;
        for (j = i+1; j <= l; j++)
            cost[i][j] = cost[i][j-1] + 1 + words[i].len;
        for (j = i+1; j <= l; j++) {
            if (cost[i][j] > l)
                cost[i][j] = INF;
            else
                cost[i][j] = l - cost[i][j];
        }
        cost[i][i] = 0;
    }

    for (i = 0; i < n; i++)
        free(words[i].str);
    free(words);
    return 0;
}
