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
    (*n)--;
    w[*n] = '\0';
    return w;
}

int main(void) {
    Word *words;
    int n, l;
    int i, j;
    int c;
    int **cost;
    int *opt;

    scanf("%d\n%d\n", &l, &n);
    words = malloc((n+1) * sizeof(*words));

    for (i = 0; i < n; i++)
        words[i].str = getWord(&words[i].len);

    cost = malloc((n+1) * sizeof(*cost));
    for (i = 0; i <= n; i++) {
        cost[i] = malloc((n+1) * sizeof(**cost));

        cost[i][i] = -1;
        for (j = i+1; j <= n; j++)
            cost[i][j] = cost[i][j-1] + 1 + words[j-1].len;
        for (j = i+1; j <= n; j++) {
            if (cost[i][j] > l)
                cost[i][j] = INF;
            else
                cost[i][j] = l - cost[i][j];
        }
        cost[i][i] = 0;
    }

    for (i = 0; i <= n; i++) {
        for (j = 0; j <= n; j++)
            printf("%d ", cost[i][j]);
        printf("\n");
    }

    opt = malloc((n+1) * sizeof(opt));
    opt[0] = 0;

    for (j = 1; j <= n; j++) {
        opt[j] = INF;
        for (i = 0; i < j; i++) {
            c = opt[i] + cost[i][j];
            if (c < opt[j])
                opt[j] = c;
        }
    }

    printf("%d\n", opt[n]);

    free(opt);

    for (i = 0; i <= n; i++)
        free(cost[i]);
    free(cost);

    for (i = 0; i < n; i++)
        free(words[i].str);
    free(words);
    return 0;
}
