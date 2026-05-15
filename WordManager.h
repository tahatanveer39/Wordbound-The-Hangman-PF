#pragma once
#ifndef Wordmanage
#define Wordmanage

#include "Gameconst.h"

//take one line split into categry, words, and how many words
void wordandcat(char line[], char category[], char words[][maxwordlen], int& wordCount);

//load all words and return total num of words loaded
int loadAllWords(const char filename[], char categories[][maxwordlen], char words[][maxwordlen]);

//rmv leading spaces, set word length according to the secret word
void trimandsetwordlen(char secret[], int& Wordlen);

//pick random word and it also not use before if all used then reset ----out category and word and its length 
void pickrandunused(char categories[][maxwordlen], char words[][maxwordlen],
    bool used[], int totalWords, char categoryOut[], char wordOut[], int& Wordlen);

#endif