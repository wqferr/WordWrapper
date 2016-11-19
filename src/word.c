#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "word.h"
#include "list.h"

#define whitespace(c) (c == ' ' || c == '\n' || c == '\t')

struct Word {
    char *str;
    size_t len;
};

struct Sentence {
    List *words;
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

Sentence *sentence_new(Word **words, size_t n) {
    size_t i;
    Sentence *s = malloc(sizeof(*s));

    s->words = list_new();
    s->len = 0;

    if (n > 0) {
        for (i = 0; i < n; i++) {
            list_append(s->words, words[i]);
            s->len += word_length(words[i]);
        }
        s->len += list_length(s->words) - 1;
    }

    return s;
}

Sentence *sentence_fromWordList(List *l) {
    ListIterator *iter = list_iterator(l);
    Sentence *s = malloc(sizeof(*s));

    s->words = list_new();
    s->len = 0;

    if (list_length(l) > 0) {
        while (listiter_hasNext(iter)) {
            list_append(s->words, listiter_next(iter));
            s->len += word_length(listiter_curr(iter));
        }
        s->len += list_length(s->words) - 1;
    }

    return s;
}

Sentence *sentence_clone(const Sentence *o) {
    ListIterator *iter = list_iterator(o->words);
    Sentence *s = malloc(sizeof(*s));

    s->words = list_new();
    s->len = 0;

    if (list_length(o->words) > 0) {
        while (listiter_hasNext(iter)) {
            list_append(s->words, listiter_next(iter));
            s->len += word_length(listiter_curr(iter));
        }
        s->len += list_length(s->words) - 1;
    }

    return s;
}

void sentence_destroy(Sentence *s) {
    list_destroy(s->words);
    free(s);
}

void sentence_destroyAll(Sentence *s) {
    ListIterator *iter = list_iterator(s->words);
    while (listiter_hasNext(iter))
        word_destroy(listiter_next(iter));
    listiter_destroy(iter);

    sentence_destroy(s);
}

char *sentence_toString(const Sentence *s) {
    char *str = malloc(s->len + 2);
    ListIterator *iter = list_iterator(s->words);
    Word *w;

    str[0] = '\0';
    while (listiter_hasNext(iter)) {
        w = listiter_next(iter);
        strcat(str, w->str);
        strcat(str, " ");
    }
    str[s->len] = '\0'; // Remove last whitespace

    listiter_destroy(iter);
    return str;
}

size_t sentence_length(const Sentence *s) {
    return s->len;
}

size_t sentence_nWords(const Sentence *s) {
    return list_length(s->words);
}

Word *sentence_removeNextWord(Sentence *s) {
    Word *w = list_removeFirst(s->words);

    if (w != NULL) {
        s->len -= w->len;
        if (list_length(s->words) > 0)
            s->len--;
    }

    return w;
}

void sentence_append(Sentence *s, Word *w) {
    if (list_length(s->words) > 0)
        s->len++;
    list_append(s->words, w);
    s->len += w->len;
}

Sentence **sentence_wrap(const Sentence *s, size_t n) {
    // TODO
}
