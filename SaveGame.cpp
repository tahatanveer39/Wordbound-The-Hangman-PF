#include "SaveGame.h"
#include "Gameconst.h"
#include "Audio.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include "HangmanGame.h"
#include "Leaderboard.h"

bool hasSavedGame()
{
    // Check for mid-game save first (hangman sprites added window)
    std::fstream file("savegame.txt", std::ios::in);
    if (file.is_open())
    {
        file.close();
        return true;
    }

    // Check for progress save(save on win window)
    std::fstream progFile("saveprogress.txt", std::ios::in);
    if (progFile.is_open())
    {
        progFile.close();
        return true;
    }

    return false;
}

void saveGameState(const char filename[], float difficultytime,
    const char secret[], const char place[],
    int Wordlen, float timeleft, int wrongCount,
    const char wrongLetters[], bool hintword, const char category[], const char playerName[])

{
    std::fstream file("savegame.txt", std::ios::out);
    if (!file.is_open()) return;

    // Save all game state
    file << filename << "\n";
    file << difficultytime << "\n";
    file << currentwind << "\n";
    file << correctwords << "\n";
    file << currentlvl << "\n";
    file << hangmanstage << "\n";
    file << secret << "\n";
    file << place << "\n";
    file << Wordlen << "\n";
    file << timeleft << "\n";
    file << wrongCount << "\n";
    file << wrongLetters << "\n";
    file << (hintword ? 1 : 0) << "\n";
    file << category << "\n";
    file << playerName << "\n";

    file.close();
}

//save game and restore (Reverse of save game state)
bool loadGameState(char filename[], float& difficultytime,
    char secret[], char place[],
    int& Wordlen, float& timeleft, int& wrongCount,
    char wrongLetters[], bool& hintword, char category[], char playerName[])
{ 
    std::fstream file("savegame.txt", std::ios::in);
    if (!file.is_open())
    {
        return false;
    }

    file.getline(filename, 100);   //Read one line
    //read single digit
    file >> difficultytime;
    file >> currentwind;
    file >> correctwords;
    file >> currentlvl;
    file >> hangmanstage;
    file.ignore();  //ignore null terminator or any other leftover
    file.getline(secret, maxwordlen);
    file.getline(place, maxwordlen);
    file >> Wordlen;
    file >> timeleft;
    file >> wrongCount;
    file.ignore();
    file.getline(wrongLetters, 30);
    int hintInt;
    file >> hintInt;
    hintword = (hintInt == 1);
    file.ignore();

    //Read category and player name
    file.getline(category, maxwordlen);
    file.ignore();
    file.getline(playerName, maxnamelen);


    file.close();
    return true;
}

void clearSavedGame()
{
    //Delete both saved games ( win window save and playing save)
    std::remove("savegame.txt");
    std::remove("saveprogress.txt");
}

void deleteSaveFile()
{
    //Remove only winwindow save
    remove("savegame.txt");
}

bool resumeGameWindow()
{
    sf::RenderWindow win(sf::VideoMode({ 800, 448 }), "Resume Game",
        sf::Style::Close | sf::Style::Titlebar);

    sf::Texture tex;
    tex.loadFromFile("new.png"); // Use same texture as quit window
    sf::Sprite spr(tex);
    spr.setPosition({ 0.f, 0.f });

    sf::Font arialBold;
    arialBold.openFromFile("arialbd.ttf");

    sf::FloatRect resumeBox({ 277.f, 168.f }, { 191.f, 60.f });
    sf::FloatRect quitBox({ 280.f, 260.f }, { 193.f, 60.f });

    sf::RectangleShape hoverResume;
    hoverResume.setFillColor(sf::Color(0, 0, 0, 120));
    hoverResume.setPosition(resumeBox.position);
    hoverResume.setSize(resumeBox.size);

    sf::RectangleShape hoverQuit;
    hoverQuit.setFillColor(sf::Color(0, 0, 0, 120));
    hoverQuit.setPosition(quitBox.position);
    hoverQuit.setSize(quitBox.size);

    sf::Text resumeText(arialBold, "Resume", 30);
    resumeText.setFillColor(sf::Color::White);
    resumeText.setStyle(sf::Text::Bold);
    resumeText.setPosition({ 320.f, 180.f });

    sf::Text quitText(arialBold, "Restart", 30);
    quitText.setFillColor(sf::Color::White);
    quitText.setStyle(sf::Text::Bold);
    quitText.setPosition({ 300.f, 265.f });

    bool hoveredResume = false;
    bool hoveredQuit = false;

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
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    sf::Vector2i pos = sf::Mouse::getPosition(win);
                    sf::Vector2f mousePos((float)pos.x, (float)pos.y);

                    if (resumeBox.contains(mousePos))
                    {
                        playClickSound();
                        win.close();
                        return true; // Resume saved game
                    }
                    else if (quitBox.contains(mousePos))
                    {
                        playClickSound();
                        clearSavedGame();
                        win.close();
                        return false; // Start new game
                    }
                }
            }
        }

        sf::Vector2i mouse = sf::Mouse::getPosition(win);
        sf::Vector2f mousePos((float)mouse.x, (float)mouse.y);
        hoveredResume = resumeBox.contains(mousePos);
        hoveredQuit = quitBox.contains(mousePos);

        win.clear(sf::Color::Black);
        if (tex.getSize().x > 0)
        {
            win.draw(spr);
        }

        if (hoveredResume) win.draw(hoverResume);
        if (hoveredQuit) win.draw(hoverQuit);

        win.draw(resumeText);
        win.draw(quitText);

        win.display();
    }

    return false;
}

// Save only progress (for win window saves)
void saveProgressState(const char filename[], float difficultytime, const char playerName[])
{
    std::fstream file("saveprogress.txt", std::ios::out);
    if (!file.is_open()) return;

    // Save level info and progress
    file << filename << "\n";
    file << difficultytime << "\n";
    file << currentwind << "\n";
    file << correctwords << "\n";
    file << currentlvl << "\n";
    file << hangmanstage << "\n";
    file << playerName << "\n";

    file.close();
}

// Load progress state
bool loadProgressState(char filename[], float& difficultytime, char playerName[])
{
    std::fstream file("saveprogress.txt", std::ios::in);
    if (!file.is_open()) return false;

    file.getline(filename, 100);
    file >> difficultytime;
    file >> currentwind;
    file >> correctwords;
    file >> currentlvl;
    file >> hangmanstage;
    file.ignore();
    file.getline(playerName, maxnamelen);

    file.close();
    return true;
}
