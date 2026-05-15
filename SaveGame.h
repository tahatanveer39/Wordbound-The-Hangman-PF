#pragma once
#ifndef SaveGame
#define SaveGame

bool hasSavedGame();//game saved or not

void saveGameState(const char filename[], float difficultytime,
    const char secret[], const char place[],
    int Wordlen, float timeleft, int wrongCount,
    const char wrongLetters[], bool hintword, const char category[],
    const char playerName[]);  

bool loadGameState(char filename[], float& difficultytime,
    char secret[], char place[],
    int& Wordlen, float& timeleft, int& wrongCount,
    char wrongLetters[], bool& hintword, char category[],
    char playerName[]); //we use here refrence var (&) to write back loaded data

// Saves ONLY difficulty time and player name
void saveProgressState(const char filename[], float difficultytime,
    const char playerName[]);


// Used if player wants to continue same difficulty next time
bool loadProgressState(char filename[], float& difficultytime,
    char playerName[]);

//used when player restart game (reset save memory)
//Used when you want to start a fresh game in the same session, but still keep the save file
void clearSavedGame();

//player want to resume or not
bool resumeGameWindow();

//Player starts a new game and chooses not to resume
void deleteSaveFile();

#endif