#include "Leaderboard.h"
#include "Audio.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <cstring>

char playerNames[maxplayers][maxnamelen];  //store top 5 player names
int playerEasyScores[maxplayers];
int playerMediumScores[maxplayers];
int playerHardScores[maxplayers];
char currentPlayerName[maxnamelen] = "";  //the player who is currently playing

void loadLeaderboard()
{
    std::fstream file("leaderboard.txt", std::ios::in);

    // Initialize all entries as empty
    for (int i = 0; i < maxplayers; i++)
    {
        //Setting the first character of player name to \0 means the string is empty like " " jb tk player apna naam nai dala ga sari slots khali
        playerNames[i][0] = '\0'; 
        playerEasyScores[i] = 0;
        playerMediumScores[i] = 0;
        playerHardScores[i] = 0;
    }

    //stop if file not open
    if (!file.is_open())
    {
        return;
    }

    int count = 0;     // Counter for how many players have been read
    
    // Loop until we reach maxplayers or until the file ends
    while (count < maxplayers && file.getline(playerNames[count], maxnamelen)) //file.getline reads up to 30 chars into playerNames[count]
    {
        //After reading the name on current line, read the three scores
        file >> playerEasyScores[count] >> playerMediumScores[count] >> playerHardScores[count];
       
        //Skip the newline character at end of line 
        file.ignore();

        //stop reading if file end 
        if (!file || playerNames[count][0] == '\0')
        {
            break;
        }
        count++;  //move to next player
    }

    file.close();
}

void saveLeaderboard()
{
    //open txt and is on writing mode
    std::fstream file("leaderboard.txt", std::ios::out);
    if (!file.is_open())
    {
        return;
    }
    for (int i = 0; i < maxplayers; i++)
    {
        //check if playername is not empty
        if (playerNames[i][0] != '\0')
        {
            // Write the player's name to the file followed by a newline
            file << playerNames[i] << "\n";

            // Write the player's scores (easy, medium, hard) in a single line separated by spaces
            // Then add a newline at the end of this line
            file << playerEasyScores[i] << " "
                << playerMediumScores[i] << " "
                << playerHardScores[i] << "\n";
        }
    }

    file.close();
}

int getPlayerIndex(const char playerName[])
{
    for (int i = 0; i < maxplayers; i++)
    {

        //Compare two strings character by character return 0 if they match else non zero
        if (strcmp(playerNames[i], playerName) == 0)
        {
            return i;
        }
    }
    return -1;
}

void updateLeaderboard(const char playerName[], int level, int score)
{
    int playerIdx = getPlayerIndex(playerName);
    //Find which slot this player is in (0-4), or -1 if not found 

    if (playerIdx != -1)
    {
        // Player exists - update their score if it's higher
        if (level == 1 && score > playerEasyScores[playerIdx])
        {
            //If playing on Easy AND new score is higher than their current easy score update the score
            playerEasyScores[playerIdx] = score;
        }
        else if (level == 2 && score > playerMediumScores[playerIdx])
        {
            playerMediumScores[playerIdx] = score;
        }
        else if (level == 3 && score > playerHardScores[playerIdx])
        {
            playerHardScores[playerIdx] = score;
        }
    }
    else
    {
        //player name not found in current existing list so
        // New player - find empty slot or replace lowest total score
      
        int emptySlot = -1;//store index of first empty slot

        //check all sorts to find an emty one
        for (int i = 0; i < maxplayers; i++)
        {
            if (playerNames[i][0] == '\0')  //if name string is empty " " 
            {
                emptySlot = i;    //empty slot found
                break;
            }
        }

        //Case 1: if empty slot exist
        if (emptySlot != -1)
        {
            // Add player name to empty slot
            int j = 0;
            while (playerName[j] != '\0')   //copy untill null terminate
            {
                playerNames[emptySlot][j] = playerName[j];
                j++;
            }
            playerNames[emptySlot][j] = '\0';

            //assign score on the based of level they played (if level 1 then store score in easy else 0)
            playerEasyScores[emptySlot] = (level == 1) ? score : 0;
            playerMediumScores[emptySlot] = (level == 2) ? score : 0;
            playerHardScores[emptySlot] = (level == 3) ? score : 0;
        }

        //Casse 2: No empty slot --> Replace lowest total sscore
        else
        {
            // Start by assuming the first player has the lowest total score
            int minTotal = playerEasyScores[0] + playerMediumScores[0] + playerHardScores[0];
            int minIdx = 0;   //indx with minimum score

            // Check all players to find who has the smallest total score
            for (int i = 1; i < maxplayers; i++)
            {
                int total = playerEasyScores[i] + playerMediumScores[i] + playerHardScores[i];
                if (total < minTotal)    // If a new minimum is found
                {
                    minTotal = total;   // Update lowest total
                    minIdx = i;     //store index of lowest score player player
                }
            }

            // Replace if new player's score is higher
            if (score > minTotal)
            {
                int j = 0;
                while (playerName[j] != '\0')    //Copy the new player name
                {
                    playerNames[minIdx][j] = playerName[j];
                    j++;
                }
                playerNames[minIdx][j] = '\0';

                //add score which level they played
                playerEasyScores[minIdx] = (level == 1) ? score : 0;
                playerMediumScores[minIdx] = (level == 2) ? score : 0;
                playerHardScores[minIdx] = (level == 3) ? score : 0;
            }
        }
    }

    saveLeaderboard();
}

bool getPlayerNameWindow()
{
    sf::RenderWindow nameWin(sf::VideoMode({ 1200, 800 }), "Enter Your Name",
        sf::Style::Close | sf::Style::Titlebar);

    // Load background image
    sf::Texture bgTex;
    if (!bgTex.loadFromFile("finaly.png"))  // Your 1200x800 image
    {
        return false;
    }
    sf::Sprite bgSpr(bgTex);
    bgSpr.setPosition({ 0.f, 0.f });

    sf::Font arialBold;
    if (!arialBold.openFromFile("arialbd.ttf"))
    {
        return false;
    }

    sf::Text promptText(arialBold, "Enter Your Name:", 35);
    promptText.setFillColor(sf::Color::White);
    promptText.setStyle(sf::Text::Bold);
    promptText.setPosition({ 570.f, 170.f });

    char inputName[maxnamelen] = "";
    int nameLen = 0;

    sf::Text nameText(arialBold, "", 30);
    nameText.setFillColor(sf::Color(255, 215, 0));
    nameText.setPosition({ 680.f, 350.f });

    // Underscores text (shown when input is empty)
    sf::Text underscoresText(arialBold, "_ _ _", 30);
    underscoresText.setFillColor(sf::Color(255, 215, 0));
    underscoresText.setPosition({ 690.f, 350.f });

    sf::Text instructText(arialBold, "Press ENTER to confirm", 28);
    instructText.setFillColor(sf::Color::White);
    instructText.setPosition({ 590.f, 520.f });

    while (nameWin.isOpen())
    {
        while (auto event = nameWin.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                nameWin.close();
                return false;
            }

            if (event->is<sf::Event::KeyPressed>())
            {
                auto keyEvent = event->getIf<sf::Event::KeyPressed>();

                // Handle Enter key
                if (keyEvent->code == sf::Keyboard::Key::Enter && nameLen > 0)
                {
                    inputName[nameLen] = '\0';

                    // Copy the name to global currentPlayerName
                    int i = 0;
                    while (inputName[i] != '\0')
                    {
                        currentPlayerName[i] = inputName[i];
                        i++;
                    }
                    currentPlayerName[i] = '\0';

                    nameWin.close();
                    return true;
                }

                // Handle Backspace
                if (keyEvent->code == sf::Keyboard::Key::Backspace && nameLen > 0)
                {
                    nameLen--;
                    inputName[nameLen] = '\0';
                }

                // Handle letter keys (A-Z)
                if (nameLen < maxnamelen - 1)
                {
                    char c = '\0';

                    if (keyEvent->code >= sf::Keyboard::Key::A && keyEvent->code <= sf::Keyboard::Key::Z)
                    {
                        c = 'A' + (static_cast<int>(keyEvent->code) - static_cast<int>(sf::Keyboard::Key::A));

                        // Make lowercase if Shift not pressed
                        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) &&
                            !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift))
                        {
                            c = c + 32; // Convert to lowercase
                        }
                    }
                    // Handle numbers
                    else if (keyEvent->code >= sf::Keyboard::Key::Num0 && keyEvent->code <= sf::Keyboard::Key::Num9)
                    {
                        c = '0' + (static_cast<int>(keyEvent->code) - static_cast<int>(sf::Keyboard::Key::Num0));
                    }
                    // Handle space
                    else if (keyEvent->code == sf::Keyboard::Key::Space)
                    {
                        c = ' ';
                    }

                    if (c != '\0')
                    {
                        inputName[nameLen++] = c;
                        inputName[nameLen] = '\0';
                    }
                }
            }
        }

        nameText.setString(inputName);

        nameWin.clear(sf::Color::Black);
        nameWin.draw(bgSpr);  // Draw background image
        nameWin.draw(promptText);

        // Draw underscores only if name is empty
        if (nameLen == 0)
        {
            nameWin.draw(underscoresText);
        }
        else
        {
            nameWin.draw(nameText);  // Draw player's input
        }

        nameWin.draw(instructText);
        nameWin.display();
    }

    return false;
}

void showLeaderboardWindow()
{
    sf::RenderWindow win(sf::VideoMode({ 1200, 800 }), "Leaderboard - Top 5 Players",
        sf::Style::Close | sf::Style::Titlebar);

    sf::Texture tex;
    tex.loadFromFile("ledfinal.png");
    sf::Sprite spr(tex);
    spr.setPosition({ 0.f, 0.f });

    sf::Font arialBold;
    arialBold.openFromFile("arialbd.ttf");

    sf::Font timesFont;
    timesFont.openFromFile("times.ttf");

    sf::Text titleText(arialBold, "Leaderboard", 50);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition({ 425.f, 90.f });

    // Column headers
    sf::Text nameHeader(arialBold, "Player", 30);
    nameHeader.setFillColor(sf::Color::White);
    nameHeader.setStyle(sf::Text::Bold);
    nameHeader.setPosition({ 280.f, 280.f });

    sf::Text easyHeader(arialBold, "Easy", 30);
    easyHeader.setFillColor(sf::Color::White);
    easyHeader.setStyle(sf::Text::Bold);
    easyHeader.setPosition({ 590.f, 280.f });

    sf::Text mediumHeader(arialBold, "Medium", 30);
    mediumHeader.setFillColor(sf::Color::White);
    mediumHeader.setStyle(sf::Text::Bold);
    mediumHeader.setPosition({ 740.f, 280.f });

    sf::Text hardHeader(arialBold, "Hard", 30);
    hardHeader.setFillColor(sf::Color::White);
    hardHeader.setStyle(sf::Text::Bold);
    hardHeader.setPosition({ 920.f, 280.f });

    // Back button
    sf::FloatRect backBox({ 480.f, 685.f }, { 240.f, 65.f });
    sf::RectangleShape hoverBack;
    hoverBack.setFillColor(sf::Color(0, 0, 0, 120));
    hoverBack.setPosition(backBox.position);
    hoverBack.setSize(backBox.size);

    sf::Text backText(arialBold, "Back", 28);
    backText.setFillColor(sf::Color::White);
    backText.setStyle(sf::Text::Bold);
    backText.setPosition({ 565.f, 695.f });

    bool hoveredBack = false;

    while (win.isOpen())
    {
        while (auto event = win.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                win.close();
            }

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
        hoveredBack = backBox.contains(mousePos);

        win.clear(sf::Color::Black);
        if (tex.getSize().x > 0)
        {
            win.draw(spr);
        }

        win.draw(titleText);
        win.draw(nameHeader);
        win.draw(easyHeader);
        win.draw(mediumHeader);
        win.draw(hardHeader);

        // Display player data
        for (int i = 0; i < maxplayers; i++)
        {
            //Display player whose name is not empty
            if (playerNames[i][0] != '\0')
            {
                //Vertical position for each row
                float yPos = 340.f + (i * 60.f);

                // Rank numbers (1. ,2. .....)
                char rankBuf[5];
                rankBuf[0] = '0' + (i + 1);
                rankBuf[1] = '.';
                rankBuf[2] = ' ';
                rankBuf[3] = '\0';

                sf::Text rankText(arialBold, rankBuf, 25);
                rankText.setFillColor(sf::Color::Yellow);
                rankText.setPosition({ 170.f, yPos });
                win.draw(rankText);

                // Player name
                sf::Text nameText(timesFont, playerNames[i], 25);
                nameText.setFillColor(sf::Color::White);
                nameText.setPosition({ 200.f, yPos });
                win.draw(nameText);

                // Easy score
                char easyBuf[20];
                int easyScore = playerEasyScores[i];
                int idx = 0;
                if (easyScore == 0)
                {
                    easyBuf[idx++] = '0';   //if score is 0 just write 0
                }
                else
                {
                    char temp[20];   //temp buffer for reverse digit
                    int tempIdx = 0;

                    //Extract digit in reverse order like 123 as 3, 2, 1
                    while (easyScore > 0)
                    {
                        temp[tempIdx++] = '0' + (easyScore % 10);
                        easyScore /= 10;
                    }

                    //Reverse digit into easy buffer (3,2,1 as 1,2,3)
                    for (int j = tempIdx - 1; j >= 0; j--)
                    {
                        easyBuf[idx++] = temp[j];
                    }
                }
                easyBuf[idx] = '\0';

                sf::Text easyText(timesFont, easyBuf, 25);
                easyText.setFillColor(sf::Color::White);
                easyText.setPosition({ 615.f, yPos });
                win.draw(easyText);

                // Medium score
                char mediumBuf[20];
                int mediumScore = playerMediumScores[i];
                idx = 0;
                if (mediumScore == 0)
                {
                    mediumBuf[idx++] = '0';
                }
                else
                {
                    char temp[20];
                    int tempIdx = 0;
                    while (mediumScore > 0)
                    {
                        temp[tempIdx++] = '0' + (mediumScore % 10);
                        mediumScore /= 10;
                    }
                    for (int j = tempIdx - 1; j >= 0; j--)
                    {
                        mediumBuf[idx++] = temp[j];
                    }
                }
                mediumBuf[idx] = '\0';

                sf::Text mediumText(timesFont, mediumBuf, 25);
                mediumText.setFillColor(sf::Color::White);
                mediumText.setPosition({ 790.f, yPos });
                win.draw(mediumText);

                // Hard score
                char hardBuf[20];
                int hardScore = playerHardScores[i];
                idx = 0;
                if (hardScore == 0)
                {
                    hardBuf[idx++] = '0';
                }
                else
                {
                    char temp[20];
                    int tempIdx = 0;
                    while (hardScore > 0)
                    {
                        temp[tempIdx++] = '0' + (hardScore % 10);
                        hardScore /= 10;
                    }
                    for (int j = tempIdx - 1; j >= 0; j--)
                    {
                        hardBuf[idx++] = temp[j];
                    }
                }
                hardBuf[idx] = '\0';

                sf::Text hardText(timesFont, hardBuf, 25);
                hardText.setFillColor(sf::Color::White);
                hardText.setPosition({ 950.f, yPos });
                win.draw(hardText);
            }
        }

        if (hoveredBack)
        {
            win.draw(hoverBack);
        }
        win.draw(backText);

        win.display();
    }
}