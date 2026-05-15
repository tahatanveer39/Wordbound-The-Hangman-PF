#include "WordManager.h"
#include <fstream>

void wordandcat(char line[], char category[], char words[][maxwordlen], int& wordCount)
{
    int i = 0, k = 0;

    // Extract category before ':' character
    while (line[i] != ':' && line[i] != '\0')
    {
        category[k++] = line[i++];
    }
    category[k] = '\0';

    if (line[i] == ':')
    {
        i++;
    }

    wordCount = 0;
    k = 0;
    int w = 0;

    // Extract words separated by commas
    while (line[i] != '\0')
    {
        if (line[i] == ',')
        {
            words[w][k] = '\0';
            w++;     // move to next word
            k = 0;     // reset character index for next word
        }
        else
        {
            words[w][k++] = line[i];    // add character to current word
        }
        i++;
    }

    words[w][k] = '\0';
    wordCount = w + 1;       // total words found
}

int loadAllWords(const char filename[], char categories[][maxwordlen], char words[][maxwordlen])
{
    std::fstream file(filename, std::ios::in);

    if (!file.is_open())
    {
        return 0;
    }

    int totalWords = 0;
    char line[maxcat];

    while (file.getline(line, maxcat))
    {
        char category[maxwordlen];
        char lineWords[maxwords][maxwordlen];
        int wordCount = 0;

        // Split line into category and words
        wordandcat(line, category, lineWords, wordCount);

        // Copy words and category into output arrays
        for (int i = 0; i < wordCount; i++)
        {
            int j = 0;
            while (category[j] != '\0')   // Copy category to category array
            {
                categories[totalWords][j] = category[j];
                j++;
            }

            categories[totalWords][j] = '\0';
            
            j = 0;
            while (lineWords[i][j] != '\0')    // Copy word to words array
            {
                words[totalWords][j] = lineWords[i][j];
                j++;
            }
            words[totalWords][j] = '\0';
            totalWords++;

            if (totalWords >= maxtotalwords)
            {
                break;
            }
        }
        if (totalWords >= maxtotalwords)
        {
            break;
        }
    }
    file.close();

    return totalWords;  // return total words loaded
}

void trimandsetwordlen(char secret[], int& Wordlen)
{
    int start = 0;
    // Skip leading spaces
    while (secret[start] == ' ')
    {
        start++;
    }
    int end = 0;
    while (secret[end] != '\0')  // find string end
    {
        end++;
    }
    while (end > start && secret[end - 1] == ' ')
    {
        end--;
    }
    int i = 0;
    for (int j = start; j < end; ++j)
    {
        secret[i++] = secret[j];
    }
    secret[i] = '\0';
    Wordlen = i;     // set actual word length
}

void pickrandunused(char categories[][maxwordlen], char words[][maxwordlen],
    bool used[], int totalWords, char categoryOut[], char wordOut[], int& Wordlen)
{
    int unusedCount = 0;

    // Count number of unused words
    for (int i = 0; i < totalWords; i++)
    {
        if (!used[i])
        {
            unusedCount++;
        }
    }

    // If all words used, reset usage
    if (unusedCount == 0)
    {
        for (int i = 0; i < totalWords; i++)
        {
            used[i] = false;
        }
    }

    int r;

    do
    {
        r = rand() % totalWords;
    } while (used[r]);
    used[r] = true;

    // Copy category of selected word
    int i = 0;
    while (categories[r][i] != '\0')
    {
        categoryOut[i] = categories[r][i];
        i++;
    }
    categoryOut[i] = '\0';

    i = 0;
    while (words[r][i] != '\0')
    {
        wordOut[i] = words[r][i];
        i++;
    }
    wordOut[i] = '\0';

    trimandsetwordlen(wordOut, Wordlen);  // Trim spaces and set word length
}