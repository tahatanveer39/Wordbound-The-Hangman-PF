#pragma once
#ifndef Gameconst
#define Gameconst

// Hangman Constants based on txt files
const int maxl = 200;             // for reading lines
const int maxwords = 5;          // max 5 words per category
const int maxwordlen = 20;      // max 10 letters per word
const int maxtotalwords = 50;  // total words across all categories
const int maxcat = 100;       // max category name length

// Global game state
extern int correctwords;     // Number of words correctly guessed by the player
extern int currentwind;      // which word player is on
extern int easyscore;        //These are saved to a file and loaded when game starts
extern int mediumscore;
extern int hardscore;
extern int currentlvl;       // Easy/Med/Hard
extern int hangmanstage;   //stages 0-6

#endif