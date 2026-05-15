#include "Windows.h"
#include "Gameconst.h"
#include "Audio.h"
#include "HangmanGame.h"
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cstring>
#include "SaveGame.h"
#include <fstream>
#include "Leaderboard.h"

// Window that shows word number
void wordindx(int wordindx, int totalwords)
{
    sf::RenderWindow w(sf::VideoMode({ 1200, 800 }), "Word Info", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture tex;
    tex.loadFromFile("num.png");

    sf::Sprite spr(tex);
    spr.setPosition({ 0.f, 0.f });

    sf::Font font;
    font.openFromFile("arialbd.ttf");

    char buf[64];
    std::snprintf(buf, sizeof(buf), "Word %d", wordindx);

    sf::Text text(font, buf, 44);
    text.setFillColor(sf::Color(214, 158, 66, 200));
    text.setStyle(sf::Text::Bold);
    text.setPosition({ 490.f, 370.f });

    sf::Clock clock;

    while (w.isOpen())
    {
        while (auto event = w.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                w.close();
            }
        }

        if (clock.getElapsedTime().asSeconds() > 0.75f)
        {
            w.close();
        }

        w.clear(sf::Color::Black);

        if (tex.getSize().x > 0)
        {
            w.draw(spr);
        }
        w.draw(text);
        w.display();
    }
}

// the window which open on pressing on finish in win window

bool finishwordwindow()
{
    sf::RenderWindow win(sf::VideoMode({ 1200, 800 }), "Words Guessed", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture bgTex;
    bgTex.loadFromFile("words.png");

    sf::Sprite bgSpr(bgTex);
    bgSpr.setPosition({ 0.f, 0.f });

    sf::Font arialBold;
    arialBold.openFromFile("arialbd.ttf");

    char buf[64];
    std::snprintf(buf, sizeof(buf), " %d Words", correctwords);

    sf::Text text(arialBold, buf, 40);
    text.setFillColor(sf::Color(214, 158, 66, 200));
    text.setStyle(sf::Text::Bold);
    text.setPosition({ 640.f, 350.f });

    // Back to start button hover shade
    sf::FloatRect backBox({ 495.f, 480.f }, { 510.f, 120.f });
    sf::RectangleShape hoverBack;
    hoverBack.setFillColor(sf::Color(0, 0, 0, 120));
    hoverBack.setPosition(backBox.position);
    hoverBack.setSize(backBox.size);

    bool hoveredBack = false;

    while (win.isOpen())
    {
        while (auto event = win.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {

                win.close();
                return false;
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {

                playClickSound();
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {

                    sf::Vector2i pos = sf::Mouse::getPosition(win);
                    sf::Vector2f mousePos((float)pos.x, (float)pos.y);

                    if (backBox.contains(mousePos))
                    {
                        playClickSound();
                        win.close();
                        return true;
                    }
                }
            }
        }

        sf::Vector2i mouse = sf::Mouse::getPosition(win);
        sf::Vector2f mousePos((float)mouse.x, (float)mouse.y);
        hoveredBack = backBox.contains(mousePos);

        win.clear(sf::Color::Black);
        win.draw(bgSpr);
        win.draw(text);

        if (hoveredBack)
        {
            win.draw(hoverBack);
        }

        win.display();
    }
    return false;
}

// Quit (Resume and Restart)
bool quitwindow()
{
    sf::RenderWindow quitWin(sf::VideoMode({ 800, 448 }), "Pause", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture tex;
    tex.loadFromFile("new.png");
    sf::Sprite spr(tex);
    spr.setPosition({ 0.f, 0.f });

    sf::Font arialBold;
    arialBold.openFromFile("arialbd.ttf");

    // Resume and Restart buttons
    sf::FloatRect resumeBox({ 275.f, 168.f }, { 200.f, 60.f });
    sf::FloatRect restartBox({ 280.f, 260.f }, { 193.f, 60.f });

    sf::RectangleShape hoverResume;
    hoverResume.setFillColor(sf::Color(0, 0, 0, 120));
    hoverResume.setPosition(resumeBox.position);
    hoverResume.setSize(resumeBox.size);

    sf::RectangleShape hoverRestart;
    hoverRestart.setFillColor(sf::Color(0, 0, 0, 120));
    hoverRestart.setPosition(restartBox.position);
    hoverRestart.setSize(restartBox.size);

    sf::Text resumeText(arialBold, "Resume", 30);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setStyle(sf::Text::Bold);
    resumeText.setPosition({ 320.f,180.f });

    sf::Text restartText(arialBold, "Restart", 30);
    restartText.setFillColor(sf::Color::White);
    restartText.setStyle(sf::Text::Bold);
    restartText.setPosition({ 310.f, 265.f });

    bool hoveredResume = false;
    bool hoveredRestart = false;

    bool resumeGame = false;
    bool restartGame = false;

    while (quitWin.isOpen())
    {
        while (auto event = quitWin.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                quitWin.close();
                resumeGame = true;
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    sf::Vector2i pos = sf::Mouse::getPosition(quitWin);
                    sf::Vector2f mousePos((float)pos.x, (float)pos.y);

                    if (resumeBox.contains(mousePos))
                    {
                        playClickSound();
                        quitWin.close();
                        resumeGame = true;
                    }
                    else if (restartBox.contains(mousePos))
                    {
                        playClickSound();
                        quitWin.close();
                        restartGame = true;
                    }
                }
            }
        }

        sf::Vector2i mouse = sf::Mouse::getPosition(quitWin);
        sf::Vector2f mousePos((float)mouse.x, (float)mouse.y);

        hoveredResume = resumeBox.contains(mousePos);
        hoveredRestart = restartBox.contains(mousePos);

        quitWin.clear(sf::Color::Black);
        if (tex.getSize().x > 0)
        {
            quitWin.draw(spr);
        }

        if (hoveredResume) quitWin.draw(hoverResume);
        if (hoveredRestart) quitWin.draw(hoverRestart);

        quitWin.draw(resumeText);
        quitWin.draw(restartText);

        quitWin.display();
    }

    if (restartGame)
    {
        return false; //to restart the level
    }
    return true; // Resume game
}

// Win window
bool winwindow(const char filename[], float difficultytime)
{
    sf::RenderWindow win(sf::VideoMode({ 1195, 800 }), "YOU WIN!", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture winTex;
    winTex.loadFromFile("youwin.png");
    sf::Sprite winSpr(winTex);
    winSpr.setPosition({ 0.f, 0.f });

    sf::Font arialBold;
    arialBold.openFromFile("arialbd.ttf");

    char buf[32];
    std::snprintf(buf, sizeof(buf), "%d Word", correctwords);

    sf::Text wordsText(arialBold, buf, 40);
    wordsText.setFillColor(sf::Color(214, 158, 66, 200));
    wordsText.setStyle(sf::Text::Bold);
    wordsText.setPosition({ 510.f, 505.f });

    // Save button lamp and text
    sf::Texture saveLampTex;
    saveLampTex.loadFromFile("lampfinal.png");
    sf::Sprite saveLamp(saveLampTex);
    saveLamp.setPosition({ -20.f, 10.f });

    sf::Text saveButtonText(arialBold, "Save", 28);
    saveButtonText.setFillColor(sf::Color(101, 67, 33));
    saveButtonText.setStyle(sf::Text::Bold);
    saveButtonText.setPosition({ 80.f, 35.f });

    // Save button hover
    sf::FloatRect saveBox({ 1000.f, 650.f }, { 200.f, 80.f });
    sf::RectangleShape hoverSave;
    hoverSave.setFillColor(sf::Color(0, 0, 0, 120));
    hoverSave.setPosition(saveBox.position);
    hoverSave.setSize(saveBox.size);

    sf::FloatRect restartBox({ 220.f, 616.f }, { 350.f, 65.f });
    sf::RectangleShape hoverRestart;
    hoverRestart.setFillColor(sf::Color(0, 0, 0, 120));
    hoverRestart.setPosition(restartBox.position);
    hoverRestart.setSize(restartBox.size);

    sf::FloatRect wordsBox({ 630.f, 613.f }, { 350.f, 65.f });
    sf::RectangleShape hoverWords;
    hoverWords.setFillColor(sf::Color(0, 0, 0, 120));
    hoverWords.setPosition(wordsBox.position);
    hoverWords.setSize(wordsBox.size);

    // Back to start button shade
    sf::FloatRect backBox({ 440.f, 710.f }, { 320.f, 60.f });
    sf::RectangleShape hoverBack;
    hoverBack.setFillColor(sf::Color(0, 0, 0, 120));
    hoverBack.setPosition(backBox.position);
    hoverBack.setSize(backBox.size);

    bool hoverrestart = false;
    bool hoverwords = false;
    bool hoverback = false;

    const float hoverbulb = 1.07f, normalbulb = 1.0f;

    while (win.isOpen())
    {
        while (auto event = win.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                win.close();
                return false;
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2i pos = sf::Mouse::getPosition(win);
                sf::Vector2f mousePos((float)pos.x, (float)pos.y);

                if (restartBox.contains(mousePos))
                {
                    playClickSound();
                    win.close();
                    return true;
                }

                if (wordsBox.contains(mousePos))
                {
                    bool goToStart = finishwordwindow();

                    if (goToStart)
                    {
                        playClickSound();
                        win.close();
                        //  return false to go back to main menu
                        return false;
                    }
                }

                if (backBox.contains(mousePos))
                {
                    playClickSound();
                    win.close();
                    // return false to go back to main menu
                    return false;
                }

                // Save button click handler
                if (saveBox.contains(mousePos) || saveLamp.getGlobalBounds().contains(mousePos))
                {
                    playClickSound();
                    saveProgressState(filename, difficultytime, currentPlayerName);
                }
            }
        }

        sf::Vector2i mouse = sf::Mouse::getPosition(win);
        sf::Vector2f mousePosF((float)mouse.x, (float)mouse.y);
        hoverrestart = restartBox.contains(mousePosF);
        hoverwords = wordsBox.contains(mousePosF);
        hoverback = backBox.contains(mousePosF);

        win.clear(sf::Color::Green);
        win.draw(winSpr);

        if (hoverrestart)
        {
            win.draw(hoverRestart);
        }
        if (hoverwords)
        {
            win.draw(hoverWords);
        }
        if (hoverback)
        {
            win.draw(hoverBack);
        }

        win.draw(saveLamp);
        win.draw(saveButtonText);
        win.draw(wordsText);
        win.display();
    }
    return false;
}

// Lose window
bool losewindow()
{
    sf::RenderWindow lose(sf::VideoMode({ 1200, 800 }), "YOU LOSE!", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture loseTex;
    loseTex.loadFromFile("loos.png");
    sf::Sprite loseSpr(loseTex);
    loseSpr.setPosition({ 0.f, 0.f });

    sf::Font arialBold;
    arialBold.openFromFile("arialbd.ttf");

    char buf[32];
    std::snprintf(buf, sizeof(buf), "%d Word", correctwords);
    sf::Text wordsText(arialBold, buf, 40);
    wordsText.setFillColor(sf::Color(214, 158, 66, 200));
    wordsText.setStyle(sf::Text::Bold);
    wordsText.setPosition({ 515.f, 625.f });

    sf::FloatRect restartBox({ 284.f, 510.f }, { 630.f, 85.f });
    sf::RectangleShape hoverRestart;
    hoverRestart.setFillColor(sf::Color(0, 0, 0, 120));
    hoverRestart.setPosition(restartBox.position);
    hoverRestart.setSize(restartBox.size);

    // Back to start button
    sf::FloatRect backBox({ 456.f, 705.f }, { 295.f, 55.f });
    sf::RectangleShape hoverBack;
    hoverBack.setFillColor(sf::Color(0, 0, 0, 120));
    hoverBack.setPosition(backBox.position);
    hoverBack.setSize(backBox.size);

    bool hoveredRestart = false;
    bool hoveredBack = false;

    while (lose.isOpen())
    {
        while (auto event = lose.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                lose.close();
                return false;
            }

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2i pos = sf::Mouse::getPosition(lose);
                sf::Vector2f mousePos((float)pos.x, (float)pos.y);
                if (restartBox.contains(mousePos))
                {
                    playClickSound();
                    lose.close();
                    return true;
                }

                if (backBox.contains(mousePos))
                {
                    playClickSound();
                    lose.close();
                    return false;
                }
            }
        }

        sf::Vector2i mouse = sf::Mouse::getPosition(lose);
        sf::Vector2f mousePosF((float)mouse.x, (float)mouse.y);
        hoveredRestart = restartBox.contains(mousePosF);
        hoveredBack = backBox.contains(mousePosF);

        lose.clear(sf::Color::Black);
        lose.draw(loseSpr);

        if (hoveredRestart)
        {
            lose.draw(hoverRestart);
        }
        if (hoveredBack)
        {
            lose.draw(hoverBack);
        }

        lose.draw(wordsText);

        lose.display();
    }
    return false;
}

// instructions window
void instruction()
{
    sf::FloatRect backBox({ 400.f, 675.f }, { 330.f, 60.f });

    sf::RenderWindow win(sf::VideoMode({ 1200, 800 }), "Window 2", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture tex;
    tex.loadFromFile("instt.png");
    sf::Sprite spr(tex);

    sf::RectangleShape hoverShade;
    hoverShade.setFillColor(sf::Color(0, 0, 0, 120));
    hoverShade.setPosition(backBox.position);
    hoverShade.setSize(backBox.size);

    bool hovered = false;

    while (win.isOpen())
    {
        while (auto event = win.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                win.close();

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    sf::Vector2i pos = sf::Mouse::getPosition(win);
                    sf::Vector2f mousePos((float)pos.x, (float)pos.y);

                    if (backBox.contains(mousePos))
                    {
                        playClickSound();
                        win.close();
                    }
                }
            }
        }

        sf::Vector2i mouse = sf::Mouse::getPosition(win);
        sf::Vector2f mousePos((float)mouse.x, (float)mouse.y);
        hovered = backBox.contains(mousePos);

        win.clear(sf::Color::Black);

        if (tex.getSize().x > 0)
        {
            win.draw(spr);
        }
        if (hovered)
        {
            win.draw(hoverShade);
        }
        win.display();
    }
}

void openWindow1(bool skipNamePrompt)
{
    // Only ask for name if not skipping and no saved game exists
    if (!hasSavedGame() && !skipNamePrompt)
    {
        // Check if player name is already set
        if (currentPlayerName[0] == '\0')
        {
            if (!getPlayerNameWindow())
            {
                return;  // Exit if player closes name window
            }
        }
    }

    if (hasSavedGame())
    {
        // Check which type of save exists
        std::fstream midGameFile("savegame.txt", std::ios::in);
        bool hasMidGameSave = midGameFile.is_open();
        if (hasMidGameSave) midGameFile.close();

        std::fstream progressFile("saveprogress.txt", std::ios::in);
        bool hasProgressSave = progressFile.is_open();
        if (hasProgressSave) progressFile.close();

        bool resume = resumeGameWindow();
        if (resume)
        {
            if (hasMidGameSave)
            {
                hangmangameResume();  // Resume mid game(win window save)
            }
            else if (hasProgressSave)
            {
                hangmangameResumeProgress();  // Resume from progress
            }
            return;
        }
        else
        {
            // User chose restart , clear all saves
            clearSavedGame();

            // Only ask for player name if we don't have 
            if (currentPlayerName[0] == '\0')
            {
                if (!getPlayerNameWindow())
                {
                    return;  // Exit if player closes name window
                }
            }
        }
    }

    sf::RenderWindow win(sf::VideoMode({ 1200, 800 }), "Select Level", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture tex;
    tex.loadFromFile("levels.png");
    sf::Sprite spr(tex);

    sf::FloatRect box1({ 200.f, 522.f }, { 215.f, 67.f }); // Easy
    sf::FloatRect box2({ 460.f, 522.f }, { 245.f, 67.f }); // Medium
    sf::FloatRect box3({ 744.f, 522.f }, { 215.f, 67.f }); // Hard
    sf::FloatRect backBox({ 606.f, 660.f }, { 317.f, 60.f }); // back
    sf::FloatRect box5({ 230.f, 665.f }, { 320.f, 55.f }); // Leaderboard

    sf::RectangleShape hoverShade;
    hoverShade.setFillColor(sf::Color(0, 0, 0, 120));

    int hovered = 0;

    while (win.isOpen())
    {
        while (auto event = win.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                win.close();

            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    sf::Vector2i posI = sf::Mouse::getPosition(win);
                    sf::Vector2f posF((float)posI.x, (float)posI.y);

                    if (box1.contains(posF))
                    {
                        playClickSound();
                        win.close();
                        hangmanlevels(1);
                    }
                    else if (box2.contains(posF))
                    {
                        playClickSound();
                        win.close();
                        hangmanlevels(2);
                    }
                    else if (box3.contains(posF))
                    {
                        playClickSound();
                        win.close();
                        hangmanlevels(3);
                    }
                    else if (backBox.contains(posF))
                    {
                        playClickSound();
                        win.close();
                    }
                    else if (box5.contains(posF))
                    {
                        playClickSound();
                        showLeaderboardWindow();
                    }
                }
            }
        }

        sf::Vector2i mouse = sf::Mouse::getPosition(win);
        sf::Vector2f mousePos((float)mouse.x, (float)mouse.y);
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
        else if (backBox.contains(mousePos))
        {
            hovered = 4;
        }
        else if (box5.contains(mousePos))
        {
            hovered = 6;
        }

        win.clear(sf::Color::Black);
        if (tex.getSize().x > 0)
        {
            win.draw(spr);
        }

        switch (hovered)
        {
        case 1: hoverShade.setPosition(box1.position); hoverShade.setSize(box1.size); win.draw(hoverShade); break;
        case 2: hoverShade.setPosition(box2.position); hoverShade.setSize(box2.size); win.draw(hoverShade); break;
        case 3: hoverShade.setPosition(box3.position); hoverShade.setSize(box3.size); win.draw(hoverShade); break;
        case 4: hoverShade.setPosition(backBox.position); hoverShade.setSize(backBox.size); win.draw(hoverShade); break;
        case 6: hoverShade.setPosition(box5.position); hoverShade.setSize(box5.size); win.draw(hoverShade); break;
        }

        win.display();
    }
}