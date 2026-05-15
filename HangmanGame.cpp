#include "HangmanGame.h"
#include "Gameconst.h"
#include "Leaderboard.h"
#include "WordManager.h"
#include "Windows.h"
#include "Audio.h"
#include "SaveGame.h"  
#include <SFML/Graphics.hpp>
#include <cstring>

// Main hangman game function( handles both new games and resumed games)
void hangmangame(const char filename[], float difficultytime, bool resuming,
    const char resumeSecret[], const char resumePlace[], int resumeWordlen,
    float resumeTimeleft, int resumeWrongCount, const char resumeWrongLetters[],
    bool resumeHintword, const char resumeCategory[], const char resumePlayerName[] = "")
{
    // Maximum stages before game over
    const int Maxstages = 6;

    // Arrays to store all words and their categories from the file
    
    //Array to store the category of each word 
    char categories[maxtotalwords][maxwordlen];

    //Array to store the actual words
    char words[maxtotalwords][maxwordlen];
   
    //track which word has already used
    bool used[maxtotalwords] = { false };  // Track which words have been used
   
    //load all words and their category return total number of words loaded
    int totalWords = loadAllWords(filename, categories, words);

    // If no words loaded, exit the game
    if (totalWords == 0)
    {
        return;
    }

    //Show "Word 1" popup but Only if starting new game 
    if (!resuming)
    {
        wordindx(currentwind, totalWords);
    }


    //if resumed then window name is hangman resumed else hangman
    sf::RenderWindow window(sf::VideoMode({ 1200, 800 }),
        resuming ? "Hangman - Resumed" : "Hangman",
        sf::Style::Close | sf::Style::Titlebar);

    // Load all required fonts for different text styles
    sf::Font font;
    if (!font.openFromFile("impact.ttf")) return;

    sf::Font timeNewRoman;
    if (!timeNewRoman.openFromFile("times.ttf")) return;

    sf::Font arialBold;
    if (!arialBold.openFromFile("arialbd.ttf")) return;

    // Load and setup background image
    sf::Texture bgTex;
    if (!bgTex.loadFromFile("Color.jpg")) return;
    sf::Sprite bgSprite(bgTex);

    // Game state variables
    const int Maxwordlen = 30;
    char category[maxwordlen];      // Category of the current word
    char secret[maxwordlen];        // The secret word to guess
    char place[Maxwordlen];         // Current state of guessed letters (with dashes)
    char hinttext[100];             // Hint text to display
    int Wordlen = 0;                // Length of the secret word
    bool hintshown = false;         // Whether hint is currently displayed
    bool hintword = false;          // Whether hint has been used

    // Timer setup - counts down from difficultytime
    const int totalsegments = 30;   // Number of segments in timer bar
    float timersec = difficultytime;
    float timeleft = timersec;

    sf::Clock clock;//time start
    clock.start();

    // Load all hangman stage images
    sf::Texture hangTex[Maxstages + 1];
    hangTex[0].loadFromFile("h1.png");
    hangTex[1].loadFromFile("h2.png");
    hangTex[2].loadFromFile("h3.png");
    hangTex[3].loadFromFile("h4.png");
    hangTex[4].loadFromFile("h5.png");
    hangTex[5].loadFromFile("h6.png");
    hangTex[6].loadFromFile("h7.png");

    // Setup hangman sprite with initial position
    sf::Sprite hangSprite(hangTex[hangmanstage]);
    hangSprite.setPosition({ 0.f, -70.f });

    // Store positions for each hangman stage
    sf::Vector2f hangPositions[Maxstages + 1];
    for (int i = 0; i <= Maxstages; i++)
    {
        hangPositions[i] = { 0.f, -70.f };
    }

    // Setup hint button (bulb/lamp icon)
    sf::Texture bulbTex;
    bulbTex.loadFromFile("lampfinal.png");
    sf::Sprite hintlamp(bulbTex);

    const float hoverbulb = 1.07f, normalbulb = 1.0f;  // Scale factors for hover effect
    hintlamp.setPosition({ -40.f, 0.f });

    // Setup pause button+lamp
    sf::Sprite quitlamp(bulbTex);
    quitlamp.setPosition({ 980.f, 0.f });

    sf::Text quitText(arialBold, "Pause", 30);
    quitText.setFillColor(sf::Color(101, 67, 33));
    quitText.setStyle(sf::Text::Bold);
    quitText.setPosition({ 1080.f, 20.f });

    // Setup save button+lamp
    sf::Sprite savelamp(bulbTex);
    savelamp.setPosition({ 1000.f, 700.f });

    sf::Text saveText(arialBold, "Save", 30);
    saveText.setFillColor(sf::Color(101, 67, 33));
    saveText.setStyle(sf::Text::Bold);
    saveText.setPosition({ 1100.f, 720.f });

    // Text display variables
    char display[Maxwordlen * 2];   // Display string with spaces between letters
    char input[2] = { '\0' };       // temporarily stores the letter the player typed

    // Setup text elements for word display
    sf::Text wordtext(timeNewRoman, "", 40);// apple
    wordtext.setStyle(sf::Text::Bold);
    wordtext.setFillColor(sf::Color::White);
    wordtext.setPosition({ 50.f, 350.f });

    // Setup input display a b c 
    sf::Text inputtext(font, "", 30);
    inputtext.setFillColor(sf::Color::Yellow);
    inputtext.setPosition({ 50.f, 500.f });

    // Setup message display (for feedback like "Correct!" or "Wrong!")
    sf::Text msgText(font, "", 28);
    msgText.setFillColor(sf::Color::Green);
    msgText.setPosition({ 50.f, 450.f });

    // Setup wrong letters display
    sf::Text wrongText(timeNewRoman, "Wrong: ", 25);
    wrongText.setStyle(sf::Text::Bold);
    wrongText.setFillColor(sf::Color::Red);
    wrongText.setPosition({ 550.f, 600.f });

    char wrongLetters[30];  // Array to store wrong letter guesses
    int wrongCount = 0;     // Count of wrong guesses

    // Setup hint header text
    sf::Text hinthead(arialBold, "Hint", 20);
    hinthead.setFillColor(sf::Color(101, 67, 33));
    hinthead.setStyle(sf::Text::Bold);
    hinthead.setPosition({ 60.f, 30.f });

    // Setup hint display text
    sf::Text hintDisplay(arialBold, "", 18);
    hintDisplay.setFillColor(sf::Color(54, 24, 0));
    hintDisplay.setPosition({ 60.f, 60.f });

    bool gameOver = false;  // Flag to track if game has ended
    bool paused = false;    // Flag to track if game is paused

    // Initialize game state based on whether we're resuming or starting fresh
    if (resuming)
    {
        // if resume then Load all saved game state

        // Copy secret word from resume parameters
        int i = 0;
        while (resumeSecret[i] != '\0')
        {
            secret[i] = resumeSecret[i];
            i++;
        }
        secret[i] = '\0';

        // Copy current guessed state (with dashes and revealed letters)
        i = 0;
        while (resumePlace[i] != '\0')
        {
            place[i] = resumePlace[i];
            i++;
        }
        place[i] = '\0';

        // Copy category/hint text
        i = 0;
        while (resumeCategory[i] != '\0')
        {
            category[i] = resumeCategory[i];
            hinttext[i] = resumeCategory[i];
            i++;
        }
        category[i] = '\0';
        hinttext[i] = '\0';

        // Restore other game state variables
        Wordlen = resumeWordlen;
        timeleft = resumeTimeleft;
        wrongCount = resumeWrongCount;

        // Copy wrong letters list
        i = 0;
        while (resumeWrongLetters[i] != '\0')
        {
            wrongLetters[i] = resumeWrongLetters[i];
            i++;
        }
        wrongLetters[i] = '\0';

        // Restore hint state
        hintword = resumeHintword;
        hintshown = resumeHintword;  // If hint was used before, show it now

        // Update hangman sprite to correct stage
        hangSprite.setTexture(hangTex[hangmanstage]);
        hangSprite.setPosition(hangPositions[hangmanstage]);
    }
    else
    {
        // NEW GAME: Initialize everything from start

        // Pick a random unused word from the word list
        pickrandunused(categories, words, used, totalWords, category, secret, Wordlen);

        // Copy category text to hint text
        int hnt = 0;
        while (category[hnt] != '\0')
        {
            hinttext[hnt] = category[hnt];
            hnt++;
        }
        hinttext[hnt] = '\0';

        // Initialize place array with dashes (all letters hidden)
        for (int i = 0; i < Wordlen; i++)
        {
            place[i] = '-';
        }
        place[Wordlen] = '\0';

        // Initialize wrong letters as empty
        wrongLetters[0] = '\0';
        wrongCount = 0;
    }

    // Set hint display text
    hintDisplay.setString(hinttext);

    // Prepare display string with spaces between letters for better readability
    int k = 0;
    for (int i = 0; i < Wordlen; i++)
    {
        display[k++] = place[i];
        if (i < Wordlen - 1)
        {
            display[k++] = ' ';  // Add space between letters
        }
    }
    display[k] = '\0';
    wordtext.setString(display);

   // This is for games that were resumed, showing letters already guessed wrong
    if (wrongCount > 0)
    {
        char wrongDisplay[80] = "Wrong: ";
        // Add each wrong letter to the display string
        for (int i = 0; i < wrongCount; i++)
        {
            // Find the current length of the string
            int len = (int)strlen(wrongDisplay);

            // Add the next wrong letter at the end
            wrongDisplay[len] = wrongLetters[i];

            //add space after each charcter
            wrongDisplay[len + 1] = ' ';

            //null terminator at end
            wrongDisplay[len + 2] = '\0';
        }
        wrongText.setString(wrongDisplay);
    }

    while (window.isOpen())
    {
        float deltaTime = 0.f;

        // Update timer only if game is not paused and not over
        if (!paused)
        {
            sf::Time elapsed = clock.restart();
            deltaTime = elapsed.asSeconds();
            if (!gameOver)
            {
                timeleft -= deltaTime;  // Countdown the timer
            }
        }

        // Check if time ran out
        if (timeleft <= 0.f)
        {
            gameOver = true;
            timeleft = 0.f;
        }

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
           
            if (event->is<sf::Event::Closed>()) window.close();

            // Only process game events if not game over and not paused
            if (!gameOver && !paused)
            {
                if (event->is<sf::Event::MouseButtonPressed>())
                {
                    auto mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();

                    if (mouseEvent->button == sf::Mouse::Button::Left)
                    {
                        sf::Vector2i pos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePosF((float)pos.x, (float)pos.y);

                        // Check if hint button clicked
                        if (hintlamp.getGlobalBounds().contains(mousePosF) && !hintword)
                        {
                            playClickSound();
                            hintshown = true;
                            hintword = true;

                            // Using hint costs one wrong attempt
                            hangmanstage++;

                            if (hangmanstage > Maxstages)
                            {
                                hangmanstage = Maxstages;
                            }
                            hangSprite.setTexture(hangTex[hangmanstage]);
                            hangSprite.setPosition(hangPositions[hangmanstage]);
                        }

                        // Check if pause button clicked
                        if (quitText.getGlobalBounds().contains(mousePosF) ||
                            quitlamp.getGlobalBounds().contains(mousePosF))
                        {
                            playClickSound();
                            paused = true;
                            clock.stop();

                            // Show quit window and get user choice
                            bool resume = quitwindow();

                            if (!resume)
                            {
                                // User chose restart - close current game and restart from Word 1
                                window.close();

                                // Delete save files
                                deleteSaveFile();

                                // Reset game state
                                hangmanstage = 0;
                                currentwind = 1;
                                correctwords = 0;

                                // Restart the same level from Word 1
                                if (currentlvl == 1)
                                {
                                    hangmangame("Easy.txt", 60.f, false, "", "", 0, 0, 0, "", false, "", currentPlayerName);
                                }
                                else if (currentlvl == 2)
                                {
                                    hangmangame("Medium.txt", 50.f, false, "", "", 0, 0, 0, "", false, "", currentPlayerName);
                                }
                                else if (currentlvl == 3)
                                {
                                    hangmangame("Hard.txt", 40.f, false, "", "", 0, 0, 0, "", false, "", currentPlayerName);
                                }
                                return;
                            }

                            // Resume - restart clock
                            clock.reset();
                            clock.start();
                            paused = false;
                        }
                        //// Check if save button clicked
                        if (saveText.getGlobalBounds().contains(mousePosF) ||
                            savelamp.getGlobalBounds().contains(mousePosF))
                        {
                            playClickSound();

                            // save while guessing the word ( like a__le save) 
                            bool hasUnguessedLetters = false;

                            for (int i = 0; i < Wordlen; i++)
                            {
                                if (place[i] == '-')
                                {
                                    hasUnguessedLetters = true;
                                    break;
                                }
                            }

                            if (hasUnguessedLetters)
                            {
                                // Save current game state to file
                                saveGameState(filename, difficultytime, secret, place,
                                    Wordlen, timeleft, wrongCount, wrongLetters, hintword, category, currentPlayerName);

                                msgText.setString("Game Saved!");
                                msgText.setFillColor(sf::Color::Black);
                            }
                        }
                    }
                }

                // Handle keyboard input
                if (event->is<sf::Event::KeyPressed>())
                {
                    auto keyEvent = event->getIf<sf::Event::KeyPressed>();

                    char c = '\0';

                    // Convert letter keys to lowercase characters
                    if (keyEvent->code >= sf::Keyboard::Key::A && keyEvent->code <= sf::Keyboard::Key::Z)
                    {
                        c = 'a' + (static_cast<int>(keyEvent->code) - static_cast<int>(sf::Keyboard::Key::A));
                    }
                    bool enterPressed = (keyEvent->code == sf::Keyboard::Key::Enter);

                    // Handle backspace - clear current input
                    if (keyEvent->code == sf::Keyboard::Key::Backspace)
                    {
                        input[0] = '\0';
                    }

                    // Store the pressed letter
                    if (c != '\0')  // ( a b c) 
                    {
                        input[0] = c;
                        input[1] = '\0';
                    }

                    // Process guess when Enter is pressed
                    if (enterPressed && input[0] != '\0')
                    {
                        char ch = input[0];
                        bool found = false;

                        // Check if letter exists in secret word
                        for (int i = 0; i < Wordlen; i++)
                        {
                            if (secret[i] == ch && place[i] == '-')
                            {
                                place[i] = ch;  // Reveal the letter
                                found = true;
                            }
                        }

                        if (!found)
                        {
                            // Letter not in word - it's a wrong guess

                            // Check if this letter was already guessed wrong before
                            bool already = false;
                            for (int wi = 0; wi < wrongCount; wi++)
                            {
                                if (wrongLetters[wi] == ch)
                                {
                                    already = true;
                                    break;
                                }
                            }

                            // Add to wrong letters list if not already there
                            if (!already && wrongCount < 29)
                            {
                                wrongLetters[wrongCount++] = ch;
                                wrongLetters[wrongCount] = '\0';
                            }

                            // Update wrong letters display
                            char wrongDisplay[80] = "Wrong: ";
                            for (int i = 0; i < wrongCount; i++)
                            {
                                int len = (int)strlen(wrongDisplay);
                                wrongDisplay[len] = wrongLetters[i];
                                wrongDisplay[len + 1] = ' ';
                                wrongDisplay[len + 2] = '\0';
                            }
                            wrongText.setString(wrongDisplay);

                            // Progress hangman stage
                            hangmanstage++;
                            if (hangmanstage <= Maxstages)
                            {
                                hangSprite.setTexture(hangTex[hangmanstage]);
                                hangSprite.setPosition(hangPositions[hangmanstage]);
                            }

                            // Display wrong attempt message with count
                            char msg[30] = "Wrong Attempt! : ";
                            int len = 18;
                            if (hangmanstage < 10)
                            {
                                msg[len++] = '0' + hangmanstage;
                            }
                            else
                            {
                                int tens = hangmanstage / 10;
                                int ones = hangmanstage % 10;
                                msg[len++] = '0' + tens;
                                msg[len++] = '0' + ones;
                            }
                            msg[len] = '\0';
                            msgText.setString(msg);
                        }
                        else
                        {
                            // Letter found in word - correct guess!
                            msgText.setString("Correct!");
                        }

                        // Update word display with spaces
                        k = 0;
                        for (int i = 0; i < Wordlen; i++)
                        {
                            display[k++] = place[i];
                            if (i < Wordlen - 1)
                            {
                                display[k++] = ' ';
                            }
                        }
                        display[k] = '\0';
                        wordtext.setString(display);
                        input[0] = '\0';  // Clear input

                        // Check if all letters revealed (win condition)
                        bool allRevealed = true;
                        for (int i = 0; i < Wordlen; i++)
                        {
                            if (place[i] == '-')
                            {
                                allRevealed = false;
                            }
                        }
                        if (allRevealed)
                        {
                            gameOver = true;
                            hintword = false;  // Reset hint flag for next game

                            // Delete save file since word is completed
                            deleteSaveFile();
                        }

                        // Check if reached max wrong attempts (lose condition)
                        if (hangmanstage >= Maxstages)
                        {
                            gameOver = true;
                        }
                    }
                }
            }
        }

        // Update input display
        char inpdisplay[25] = "Enter a letter: ";
        inpdisplay[15] = input[0] ? input[0] : ' ';
        inpdisplay[16] = '\0';
        inputtext.setString(inpdisplay);

        // Get current mouse position for hover effects
        sf::Vector2i mouseNow = sf::Mouse::getPosition(window);
        sf::Vector2f mouseNowF((float)mouseNow.x, (float)mouseNow.y);

        // Apply hover effects on buttons (only when not paused)
        if (!paused)
        {
            if (hintlamp.getGlobalBounds().contains(mouseNowF))
            {
                hintlamp.setScale({ hoverbulb, 1.07f });
            }
            else
            {
                hintlamp.setScale({ normalbulb, 1.0f });
            }
            if (quitlamp.getGlobalBounds().contains(mouseNowF))
            {
                quitlamp.setScale({ hoverbulb, 1.07f });
            }
            else
            {
                quitlamp.setScale({ normalbulb, 1.0f });
            }
            if (savelamp.getGlobalBounds().contains(mouseNowF))
            {
                savelamp.setScale({ hoverbulb, 1.07f });
            }
            else
            {
                savelamp.setScale({ normalbulb, 1.0f });
            }
        }
        else
        {
            // Reset all scales when paused
            hintlamp.setScale({ normalbulb, 1.0f });
            quitlamp.setScale({ normalbulb, 1.0f });
            savelamp.setScale({ normalbulb, 1.0f });
        }

        // ===== RENDERING =====
        window.clear();
        window.draw(bgSprite);
        window.draw(hangSprite);
        window.draw(wordtext);
        window.draw(inputtext);
        window.draw(msgText);
        window.draw(hintlamp);
        window.draw(quitlamp);
        window.draw(savelamp);
        window.draw(saveText);
        window.draw(hinthead);

        // Only draw hint text if it's been revealed
        if (hintshown)
        {
            window.draw(hintDisplay);
        }

        window.draw(wrongText);
        window.draw(quitText);

        // Draw timer bar (segmented progress bar)
        sf::RectangleShape segment;
        float segWidth = 600.f / totalsegments;
        float segHeight = 20.f;
        float startX = 260.f, startY = 730.f;

        for (int i = 0; i < totalsegments; i++)
        {
            float ratio = (float)(i + 1) / totalsegments;

            // Color coding: Green > Yellow > Red as time runs out
            sf::Color color;
            if (ratio > 0.66f)
            {
                color = sf::Color::Green;
            }
            else if (ratio > 0.33f)
            {
                color = sf::Color::Yellow;
            }
            else
            {
                color = sf::Color::Red;
            }

            segment.setSize({ segWidth - 2.f, segHeight });
            segment.setPosition({ startX + i * segWidth, startY });

            // Darken segments that are past the current time
            if (i >= (int)(totalsegments * (timeleft / timersec)))
            {
                segment.setFillColor(sf::Color(50, 50, 50, 100));
            }
            else
            {
                segment.setFillColor(color);
            }
            window.draw(segment);
        }

        window.display();

        // Game over
        if (gameOver)
        {
            window.close();
            bool restart;

            // Check if player lost (hangman complete OR time ran out)
            if (hangmanstage >= Maxstages || timeleft <= 0.f)
            {
                // Player lost: Update leaderboard with current score before deleting saves
                updateLeaderboard(currentPlayerName, currentlvl, correctwords);

                // Delete ALL save files
                deleteSaveFile();  // Remove mid-game save
                clearSavedGame();  // Remove both mid-game and progress saves

                restart = losewindow();
                if (restart)
                {
                    // Reset all global counters for fresh start
                    hangmanstage = 0;
                    currentwind = 1;
                    correctwords = 0;

                    // Start completely new game (no resume)
                    hangmangame(filename, difficultytime, false, "", "", 0, 0, 0, "", false, "", currentPlayerName);
                }
            }
            else
            {
                // Player win: Update score and continue
               correctwords++;
               currentwind++;

                // Update leaderboard with new score
                updateLeaderboard(currentPlayerName, currentlvl, correctwords);

                restart = winwindow(filename, difficultytime);
                if (restart)
                {
                    // Continue with next word
                    hangmangame(filename, difficultytime, false, "", "", 0, 0, 0, "", false, "", currentPlayerName);
                }
            }

        }
    }
}

// Resume game from saved state
void hangmangameResume()
{
    // Load all saved game parameters
    char filename[100];
    float difficultytime;
    char secret[maxwordlen];
    char place[maxwordlen];
    int Wordlen;
    float timeleft;
    int wrongCount;
    char wrongLetters[30];
    bool hintword;
    char category[maxwordlen];
    char playerName[maxnamelen];

    // Try to load saved game state from file
    if (!loadGameState(filename, difficultytime, secret, place,
        Wordlen, timeleft, wrongCount, wrongLetters, hintword, category, playerName))
    {
        // No saved game found or error loading - exit
        return;
    }

    // Call main game function with resume flag set to true
    hangmangame(filename, difficultytime, true, secret, place, Wordlen,
        timeleft, wrongCount, wrongLetters, hintword, category, playerName);
}
// Resume game from progress save (after winning words)
void hangmangameResumeProgress()
{
    char filename[100];
    float difficultytime;
    char playerName[maxnamelen];

    // Load progress state
    if (!loadProgressState(filename, difficultytime, playerName))
    {
        return;
    }
    int i = 0;
    while (playerName[i] != '\0')
    {
        currentPlayerName[i] = playerName[i];
        i++;
    }
    currentPlayerName[i] = '\0';
    // Start fresh game with loaded progress
    hangmangame(filename, difficultytime, false, "", "", 0, 0, 0, "", false, "", currentPlayerName);
}

// Start a new game at specified difficulty level
void hangmanlevels(int level)
{
    // Reset all game counters for fresh start
    currentwind = 1;
    currentlvl = level;
    hangmanstage = 0;
    correctwords = 0;

    // Start game with difficulty-specific settings
    if (level == 1)
    {
        // Easy mode: 60 seconds per word
        hangmangame("Easy.txt", 60.f, false, "", "", 0, 0, 0, "", false, "", currentPlayerName);
    }
    else if (level == 2)
    {
        // Medium mode: 50 seconds per word
        hangmangame("Medium.txt", 50.f, false, "", "", 0, 0, 0, "", false, "", currentPlayerName);
    }
    else if (level == 3)
    {
        // Hard mode: 40 seconds per word
        hangmangame("Hard.txt", 40.f, false, "", "", 0, 0, 0, "", false, "", currentPlayerName);
    }
}