// CAESAR

#include <ctype.h>
#include <stdio.h>
#include <string.h>

const int tamanhoTexto = 100;

int main(int argc, char *argv[])
{
    // Filtra os possiveis erros de input e define a key em uma variavel
    int chaveCriptografia = 0;

    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }

        chaveCriptografia = chaveCriptografia * 10 + (argv[1][i] - 48);
        chaveCriptografia = chaveCriptografia % 26;
    }

    // Define o texto para criptografar
    char textoCriptografar[tamanhoTexto];
    printf("plaintext: ");
    fgets(textoCriptografar, sizeof(textoCriptografar), stdin);

    // Faz as conversões de caracteres
    for (int i = 0; i < strlen(textoCriptografar); i++)
    {
        if (isalpha(textoCriptografar[i]) && isupper(textoCriptografar[i]))
        {
            textoCriptografar[i] = textoCriptografar[i] + chaveCriptografia;
            if (textoCriptografar[i] > 90)
            {
                textoCriptografar[i] -= 26;
            }
        }
        
        if (isalpha(textoCriptografar[i]) && islower(textoCriptografar[i]))
        {
            textoCriptografar[i] -= 26;
            textoCriptografar[i] = textoCriptografar[i] + chaveCriptografia;
            if (textoCriptografar[i] > 96)
            {
                textoCriptografar[i] -= 26;
            }
            textoCriptografar[i] += 26;
        }
    }

    // Mostra o texto criptografado
    printf("ciphertext: %s", textoCriptografar);
    return 0;
}
