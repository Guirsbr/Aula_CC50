// SCRABBLE

#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(char *word, size_t len);

int main(void)
{
    // Get input words from both players
    char word1[30];
    printf("Player 1: ");
    fgets(word1, sizeof(word1), stdin);
    char word2[30];
    printf("Player 2: ");
    fgets(word2, sizeof(word2), stdin);

    // Score both words
    int score1 = compute_score(word1, strlen(word1));
    int score2 = compute_score(word2, strlen(word2));

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!");
    }
    else
    {
        printf("Tie!");
    }
    return 0;
}

int compute_score(char *word, size_t len)
{
    // TODO: Compute and return score for string
    int scoreSum = 0;
    for (int i = 0; i < len; i++)
    {
        if (isupper(word[i]))
        {
            scoreSum += POINTS[word[i] - 65];
        }
        else if (islower(word[i]))
        {
            scoreSum += POINTS[word[i] - 97];
        }
    }
    return scoreSum;
}
