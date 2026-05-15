#include "Audio.h"

sf::SoundBuffer buffer;   //stores background music
sf::SoundBuffer clickBuffer;  //store click sound

// Optional Sound objects (created only if loading succeeds)
std::optional<sf::Sound> backgroundSound;
std::optional<sf::Sound> clickSound;
bool soundEnabled = true;   //Sound is ON by default 

bool initSounds()
{
    //load click sound and If loading succeeds, create a Sound object using emplace()
    if (clickBuffer.loadFromFile("click.wav"))
    {
        // Construct clickSound using the buffer
        clickSound.emplace(clickBuffer);
    }
    return true;
}

void playClickSound()
{
    if (clickSound && soundEnabled)
    {
        clickSound->play();
    }
}