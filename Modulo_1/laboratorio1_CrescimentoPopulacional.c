// CRESCIMENTO POPULACIONAL

#include <stdio.h>

int solicitar_populacao_inicial(void);
int solicitar_populacao_final(int populacaoIni);

int main(void)
{
    // Define os valores iniciais
    int populacaoInicial = solicitar_populacao_inicial();
    int populacaoFinal = solicitar_populacao_final(populacaoInicial);
    int anosCorridos = 0;

    // Faz os calculos necessarios e armazena nas variaveis
    while (populacaoInicial < populacaoFinal)
    {
        int populacaoInicialAuxiliar = populacaoInicial;
        populacaoInicial += (populacaoInicialAuxiliar / 3);
        populacaoInicial -= (populacaoInicialAuxiliar / 4);
        anosCorridos++;
    }

    // Mostra o resultado
    printf("Years: %d\n", anosCorridos);
}

int solicitar_populacao_inicial(void)
{
    int populacao = 1;
    int populacaoLido = 0;
    do
    {
        printf("Digite a populacao inicial: ");
        populacaoLido = scanf("%d", &populacao);
        while (populacaoLido != 1)
        {
            scanf("%*[^\n]");
            printf("Digite a populacao inicial: ");
            populacaoLido = scanf("%d", &populacao);
        }
    }
    while (populacao < 9);
    return populacao;
}

int solicitar_populacao_final(int populacaoIni)
{
    int populacaoFin = 1;
    int populacaoFinLido = 0;
    do
    {
        printf("Digite a populacao final: ");
        populacaoFinLido = scanf("%d", &populacaoFin);
        while (populacaoFinLido != 1)
        {
            scanf("%*[^\n]");
            printf("Digite a populacao final: ");
            populacaoFinLido = scanf("%d", &populacaoFin);
        }
    }
    while (populacaoFin < populacaoIni);
    return populacaoFin;
}
