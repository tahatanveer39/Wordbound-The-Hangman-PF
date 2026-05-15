#pragma once
#ifndef Audio
#define Audio

#include <SFML/Audio.hpp>
//extern variables = declare here so other files can use them
extern sf::SoundBuffer buffer;
extern sf::SoundBuffer clickBuffer;

extern bool soundEnabled;    //ON OFF 

bool initSounds(); //Load Sound Files in Memory(Buf and clickbuf)
void playClickSound();

#endif