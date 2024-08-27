// MARIO

#include <stdio.h>

int solicitar_tamanho_piramide(void);

int main(void)
{
    // Define o tamanho da piramide
    int tamanhoPiramide = solicitar_tamanho_piramide();

    // Imprime a piramide
    int progressaoDaPiramide = 1;
    int controladorDeAlinhamento = tamanhoPiramide;
    for (int i = 0; i < tamanhoPiramide; i++)
    {
        printf("%*s", controladorDeAlinhamento, "#");
        for (int j = 1; j < progressaoDaPiramide; j++)
        {
            printf("#");
        }
        controladorDeAlinhamento--;
        printf("\n");
        progressaoDaPiramide++;
    }
}

int solicitar_tamanho_piramide(void)
{
    int tamanho = 1;
    int tamanhoLido = 0;
    do
    {
        printf("Digite o tamanho entre 1 e 8: ");
        tamanhoLido = scanf("%d", &tamanho);
        while (tamanhoLido != 1)
        {
            scanf("%*[^\n]");
            printf("Digite o tamanho entre 1 e 8: ");
            tamanhoLido = scanf("%d", &tamanho);
        }  
    }
    while (tamanho < 1 || tamanho > 8);
    return tamanho;
}