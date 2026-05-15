#pragma once
#ifndef Leaderboard
#define Leaderboard

const int maxplayers = 5;    //Leaderboard shows top 5 player only
const int maxnamelen = 30;  //Each Player name has max 30 characters

// Arrays to store top 5 players
extern char playerNames[maxplayers][maxnamelen];
extern int playerEasyScores[maxplayers];      //Stores the easy level score for top player (max 5)
extern int playerMediumScores[maxplayers];
extern int playerHardScores[maxplayers];
extern char currentPlayerName[maxnamelen];   //Store name of Player who is currently Playing

//Read leaderboard data from leaderboard.txt file 
void loadLeaderboard(); 

//Saves updated leaderboard back to "leaderboard.txt"
void saveLeaderboard();

//Add or Update Player Score (Name+Levels(1/2/3)+their scores)
void updateLeaderboard(const char playerName[], int level, int score);

//searches for a player name return(0-4 ya -1 if player name not exist)---does we need to update or to insert new
int getPlayerIndex(const char playerName[]);

//open window to take player name return 1 if player enter or 0 if he close the window
bool getPlayerNameWindow();  
void showLeaderboardWindow();

#endif