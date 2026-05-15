#pragma once
#ifndef Hangmangame
#define Hangmangame
#include "SaveGame.h"

//here in this function if ressume=false then all other parameters having resume is ignored
void hangmangame(const char filename[], float difficultytime, bool resuming = false,
    const char resumeSecret[] = "", const char resumePlace[] = "", int resumeWordlen = 0,
    float resumeTimeleft = 0, int resumeWrongCount = 0, const char resumeWrongLetters[] = "",
    bool resumeHintword = false, const char resumeCategory[] = "");

//level selection handler
void hangmanlevels(int level);

//load save game and used when player choose resume from menu
void hangmangameResume();

//Resume from save (this is on win window)
void hangmangameResumeProgress();

#endif