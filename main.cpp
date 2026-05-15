#include "Audio.h"
#include "Windows.h"
#include "Leaderboard.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>
#include <iostream>

// Define global variables
int correctwords = 0;
int currentwind = 1;
int currentlvl = 0;
int hangmanstage = 0;

int main()
{
    srand((unsigned int)time(0));

    loadLeaderboard();  // Load leaderboard data

    if (!initSounds())
    {
        return -1;
    }

    sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "Image + Hover Boxes", sf::Style::Close | sf::Style::Titlebar);


    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("hangs.wav"))
    {
        std::cerr << "Failed to load hangs.wav\n";
    }

    sf::Sound gameSound(soundBuffer);
    bool isSoundOn = true;
    if (soundBuffer.getSampleCount() > 0)
    {
        gameSound.play();
        gameSound.setLooping(true);
    }

    sf::Texture soundon;
    sf::Texture soundoff;
    if (!soundon.loadFromFile("picfi.png"))
    {
        std::cerr << "Failed to load sound_on.png\n";
    }

    if (!soundoff.loadFromFile("mutefi.png"))
    {
        std::cerr << "Failed to load sound_off.png\n";
    }

    sf::Sprite soundSprite((soundon.getSize().x > 0) ? soundon : soundoff);

    auto soundiconposition = [&]()
        {
            sf::Vector2u winSize = window.getSize();
            sf::FloatRect bounds = soundSprite.getGlobalBounds();

            float margin = 10.f;
            float soundwid = bounds.size.x;
            float soundheight = bounds.size.y;

            soundSprite.setPosition({
                static_cast<float>(winSize.x) - soundwid - margin,
                static_cast<float>(winSize.y) - soundheight - margin
                });
        };

    soundiconposition();
    sf::Texture texture;
    if (!texture.loadFromFile("start1.png"))
    {
        return -1;
    }

    sf::Sprite sprite(texture);
    sprite.setScale({ 0.5f, 0.5f });

    sf::FloatRect box1({ 405.f, 675.f }, { 280.f, 50.f }); // openWindow1 (levels)
    sf::FloatRect box2({ 130.f, 675.f }, { 250.f, 50.f }); // openWindow2 (instructions)
    sf::FloatRect box3({ 712.f, 675.f }, { 290.f, 50.f }); // exit

    sf::RectangleShape hoverShade;
    hoverShade.setFillColor(sf::Color(0, 0, 0, 100));

    sf::Vector2i mouse;
    sf::Vector2f mousePos;
    int hovered = 0;

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    sf::Vector2i pos = sf::Mouse::getPosition(window);
                    sf::Vector2f p((float)pos.x, (float)pos.y);
                    if (soundSprite.getGlobalBounds().contains(p))
                    {
                        if (isSoundOn)
                        {
                            gameSound.pause();
                            isSoundOn = false;
                            if (soundoff.getSize().x > 0)
                            {

                                playClickSound();
                                soundSprite.setTexture(soundoff);
                            }
                        }
                        else
                        {
                            gameSound.play();
                            isSoundOn = true;
                            if (soundon.getSize().x > 0)
                            {
                                playClickSound();
                                soundSprite.setTexture(soundon);
                            }
                        }
                        continue;
                    }

                    if (box1.contains(p))
                    {
                        playClickSound();
                        openWindow1(false);
                    }
                    else if (box2.contains(p))
                    {
                        playClickSound();
                        instruction();
                    }
                    else if (box3.contains(p))
                    {
                        playClickSound();
                        window.close();
                    }
                }
            }
        }

        mouse = sf::Mouse::getPosition(window);
        mousePos = { (float)mouse.x, (float)mouse.y };
        hovered = 0;

        if (box1.contains(mousePos))
        {
            hovered = 1;
        }
        else if (box2.contains(mousePos))
        {
            hovered = 2;
        }
        else if (box3.contains(mousePos))
        {
            hovered = 3;
        }
        window.clear(sf::Color::White);
        window.draw(sprite);

        if (hovered == 1)
        {
            hoverShade.setPosition(box1.position);
            hoverShade.setSize(box1.size);
            window.draw(hoverShade);
        }
        else if (hovered == 2)
        {
            hoverShade.setPosition(box2.position);
            hoverShade.setSize(box2.size);
            window.draw(hoverShade);
        }
        else if (hovered == 3)
        {
            hoverShade.setPosition(box3.position);
            hoverShade.setSize(box3.size);
            window.draw(hoverShade);
        }

        window.draw(soundSprite);
        window.display();
    }

    return 0;
}