#ifndef _WORD_H_
#define _WORD_H_ 1

#include <stddef.h>

typedef struct Word Word;

Word *word_new(const char *);
void word_destroy(Word *);
const char *word_toString(const Word *);
size_t word_length(const Word *);

#endif
