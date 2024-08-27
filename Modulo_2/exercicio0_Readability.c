// READABILITY

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

const int tamanhoTexto = 1000;

int main(void)
{
    // Define o texto para analisar
    char textoAnalisar[tamanhoTexto];
    printf("Texto: ");
    fgets(textoAnalisar, sizeof(textoAnalisar), stdin);

    // Faz as analises e armazena nas variaveis
    int contagemLetras = 0;
    int contagemSentenca = 0;
    int contagemPalavras = 1;
    for (int i = 0; i < strlen(textoAnalisar); i++)
    {
        if (isalpha(textoAnalisar[i]))
        {
            contagemLetras++;
        }
        if (textoAnalisar[i] == '.' || textoAnalisar[i] == '!' || textoAnalisar[i] == '?')
        {
            contagemSentenca++;
        }
        if (textoAnalisar[i] == ' ')
        {
            contagemPalavras++;
        }
    }

    // Faz o calculo e mostra o resultado
    int indice = round(0.0588 * ((contagemLetras * 100.0) / contagemPalavras) -
                       0.296 * ((contagemSentenca * 100.0) / contagemPalavras) - 15.8);
    if (indice < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (indice >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", indice);
    }
    return 0;
}
