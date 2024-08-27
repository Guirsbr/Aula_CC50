// SPELLER

// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

bool colocarPalavraNaHash(const char *palavraSemPontuacao, const char *palavraCompleta);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N][N][N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    char palavraSemPontuacao[LENGTH + 1];
    int quantidadePontuacao = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        if (isalpha(word[i]) == 0)
        {
            quantidadePontuacao++;
        }
        else
        {
            palavraSemPontuacao[i - quantidadePontuacao] = word[i];
        }
    }
    palavraSemPontuacao[strlen(word) - quantidadePontuacao] = '\0';

    node *palavraAtual = NULL;
    if (strlen(palavraSemPontuacao) == 1)
    {
        palavraAtual = table[toupper(palavraSemPontuacao[0]) - 'A'][0][0];
    }
    else if (strlen(palavraSemPontuacao) == 2)
    {
        palavraAtual =
            table[toupper(palavraSemPontuacao[0]) - 'A'][toupper(palavraSemPontuacao[1]) - 'A'][0];
    }
    else
    {
        palavraAtual =
            table[toupper(palavraSemPontuacao[0]) - 'A'][toupper(palavraSemPontuacao[1]) - 'A']
                 [toupper(palavraSemPontuacao[2]) - 'A'];
    }

    char palavraChecar[LENGTH + 1];
    for (int i = 0; i < strlen(word); i++)
    {
        palavraChecar[i] = toupper(word[i]);
    }
    palavraChecar[strlen(word)] = '\0';

    while (true)
    {
        if (palavraAtual->next == NULL)
        {
            return false;
        }
        if (strcasecmp(palavraChecar, palavraAtual->word) == 0)
        {
            return true;
        }
        palavraAtual = palavraAtual->next;
    }
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *dicionario = fopen(dictionary, "r");
    if (dicionario == NULL)
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                table[i][j][k] = malloc(sizeof(node));
                if (table[i][j][k] == NULL)
                {
                    fclose(dicionario);
                    unload();
                    return false;
                }
                table[i][j][k]->next = NULL;
            }
        }
    }

    char bufferDicionario;
    char palavraDicionario[LENGTH + 1];
    char palavraSemPontuacao[LENGTH + 1];
    int quantidadePontuacao = 0;
    int indicePalavra = 0;

    while (fread(&bufferDicionario, sizeof(char), 1, dicionario))
    {
        palavraDicionario[indicePalavra] = bufferDicionario;
        if (isalpha(bufferDicionario) == 0 && bufferDicionario != '\n')
        {
            quantidadePontuacao++;
        }
        else
        {
            palavraSemPontuacao[indicePalavra - quantidadePontuacao] = bufferDicionario;
        }

        indicePalavra++;
        if (bufferDicionario == '\n')
        {
            palavraDicionario[indicePalavra - 1] = '\0';
            palavraSemPontuacao[indicePalavra - quantidadePontuacao - 1] = '\0';
            if (!colocarPalavraNaHash(palavraSemPontuacao, palavraDicionario))
            {
                fclose(dicionario);
                unload();
                return false;
            }
            indicePalavra = 0;
            quantidadePontuacao = 0;
        }
    }

    fclose(dicionario);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int contagemPalavras = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                node *nodeAtual = table[i][j][k];
                while (nodeAtual->next != NULL)
                {
                    contagemPalavras++;
                    nodeAtual = nodeAtual->next;
                }
            }
        }
    }

    return contagemPalavras;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *variavelAuxiliar1;
    node *variavelAuxiliar2;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                variavelAuxiliar1 = table[i][j][k];
                bool terminouDeLiberar = false;

                while (terminouDeLiberar == false)
                {
                    if (variavelAuxiliar1->next == NULL)
                    {
                        free(variavelAuxiliar1);
                        terminouDeLiberar = true;
                    }
                    else
                    {
                        variavelAuxiliar2 = variavelAuxiliar1->next;
                        free(variavelAuxiliar1);
                        variavelAuxiliar1 = variavelAuxiliar2;
                    }
                }
            }
        }
    }

    return true;
}

bool colocarPalavraNaHash(const char *palavraSemPontuacao, const char *palavraCompleta)
{
    node *nodeAtual = NULL;
    if (strlen(palavraSemPontuacao) == 1)
    {
        nodeAtual = table[toupper(palavraSemPontuacao[0]) - 'A'][0][0];
    }
    else if (strlen(palavraSemPontuacao) == 2)
    {
        nodeAtual =
            table[toupper(palavraSemPontuacao[0]) - 'A'][toupper(palavraSemPontuacao[1]) - 'A'][0];
    }
    else
    {
        nodeAtual =
            table[toupper(palavraSemPontuacao[0]) - 'A'][toupper(palavraSemPontuacao[1]) - 'A']
                 [toupper(palavraSemPontuacao[2]) - 'A'];
    }

    while (nodeAtual->next != NULL)
    {
        nodeAtual = nodeAtual->next;
    }

    for (int i = 0; i < strlen(palavraCompleta); i++)
    {
        nodeAtual->word[i] = toupper(palavraCompleta[i]);
    }
    nodeAtual->word[strlen(palavraCompleta)] = '\0';

    nodeAtual->next = malloc(sizeof(node));
    if (nodeAtual->next == NULL)
    {
        return false;
    }
    nodeAtual->next->next = NULL;

    return true;
}
