#ifndef _WORD_H_
#define _WORD_H_ 1

#include <stddef.h>

typedef struct Word Word;
typedef struct Sentence Sentence;

Word *word_new(const char *);
void word_destroy(Word *);
const char *word_toString(const Word *);
size_t word_length(const Word *);

Sentence *sentence_new(Word **, size_t);
Sentence *sentence_fromWordList(List *);
void sentence_destroy(Sentence *);
void sentence_destroyAll(Sentence *);
char *sentence_toString(const Sentence *);
size_t sentence_length(const Sentence *);
size_t sentence_nWords(const Sentence *);

Word *sentence_removeNextWord(Sentence *);

#endif
