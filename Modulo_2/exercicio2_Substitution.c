// SUBSTITUTION

#include <ctype.h>
#include <stdio.h>
#include <string.h>

const int tamanhoTexto = 100;

int main(int argc, char *argv[])
{
    // Filtra os possiveis erros de input
    char alfabeto[27];

    if (argc != 2 || strlen(argv[1]) != 26)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    for (int i = 0; i < strlen(argv[1]); i++)
    {
        char letraAtual = argv[1][i];

        if (!isalpha(letraAtual))
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }

        if (islower(letraAtual))
        {
            letraAtual = toupper(letraAtual);
            argv[1][i] = toupper(argv[1][i]);
        }

        if (letraAtual == alfabeto[letraAtual - 65])
        {
            printf("Usage: ./substitution key\n");
            return 1;
        }

        alfabeto[letraAtual - 65] = letraAtual;
    }

    // Define o texto para criptografar
    char textoCriptografar[tamanhoTexto];
    printf("plaintext: ");
    fgets(textoCriptografar, sizeof(textoCriptografar), stdin);

    // Faz as conversões de caracteres
    for (int i = 0; i < strlen(textoCriptografar); i++)
    {
        if (isupper(textoCriptografar[i]))
        {
            textoCriptografar[i] = argv[1][textoCriptografar[i] - 65];
        }
        else if (islower(textoCriptografar[i]))
        {
            textoCriptografar[i] = tolower(argv[1][textoCriptografar[i] - 97]);
        }
    }

    // Mostra o texto criptografado
    printf("ciphertext: %s", textoCriptografar);
    return 0;
}
