#pragma once
#ifndef Windows
#define windows
#include "SaveGame.h"

//the w shows Word 1, Word 2...for short time then auto close
void wordindx(int wordindx, int totalwords);

//the w wich appears on pressing finish on win window it shows how many are guessed correctly and have menu button
bool finishwordwindow();

//the w appear on clicking pause ask to resume or restart
bool quitwindow();

//win window
bool winwindow(const char filename[], float difficultytime);

//lose window
bool losewindow();

//instruction
void instruction();

//Easy/med/hard/leaderboard that window
void openWindow1();
void openWindow1(bool skipNamePrompt = false);

#endif