#include <stdlib.h>
#include <string.h>

#include "word.h"

#define whitespace(c) (c == ' ' || c == '\n' || c == '\t')

struct Word {
    char *str;
    size_t len;
};


Word *word_new(const char *str) {
    size_t i = 0;
    Word *w = malloc(sizeof(*w));
    while (str[i] && !whitespace(str[i]))
        i++;
    w->str = strndup(str, i-1);
    w->len = i;
    return w;
}

void word_destroy(Word *w) {
    free(w->str);
    free(w);
}

const char *word_toString(const Word *w) {
    return w->str;
}

size_t word_length(const Word *w) {
    return w->len;
}
