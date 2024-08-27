// PLURALITY

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define QUANTIDADE_MAXIMA_CANDIDATOS 9
#define QUANTIDADE_MAXIMA_VOTOS 100
#define TAMANHO_NOME_CANDIDATO 40


bool compararDuasStrings(char *string1, size_t lenString1, char *string2, size_t lenString2);
void imprimirGanhador(void);
int solicitarQuantidadeVotos(void);
bool votar(char *name);

typedef struct
{
    char nome[TAMANHO_NOME_CANDIDATO];
    int quantidadeVotos;
} candidato;

candidato listaCandidatos[QUANTIDADE_MAXIMA_CANDIDATOS - 1];

int contagemCandidatos;

int main(int argc, char *argv[])
{
    // Filtra os possiveis erros de input
    if (argc < 2)
    {
        printf("Uso: runoff [candidato ...]\n");
        return 1;
    }

    // Coloca os canditatos em uma lista
    contagemCandidatos = argc - 1;
    if (contagemCandidatos > QUANTIDADE_MAXIMA_CANDIDATOS)
    {
        printf("O numero maximo de canditatos e %i\n", QUANTIDADE_MAXIMA_CANDIDATOS);
        return 2;
    }
    for (int i = 0; i < contagemCandidatos; i++)
    {
        for (int j = 0; j < strlen(argv[i + 1]); j++)
        {
            listaCandidatos[i].nome[j] = argv[i + 1][j];
        }
        listaCandidatos[i].quantidadeVotos = 0;
    }

    // Solicita a quantidade de eleitores/votos
    int quantidadeVotos = solicitarQuantidadeVotos();
    if (quantidadeVotos > QUANTIDADE_MAXIMA_VOTOS)
    {
        printf("Quantidade maxima de votos e %i\n", QUANTIDADE_MAXIMA_VOTOS);
        return 3;
    }

    // Solicita o voto para cada eleitor
    getchar();
    for (int i = 1; i < quantidadeVotos + 1; i++)
    {
        char candidatoVoto[TAMANHO_NOME_CANDIDATO];
        printf("Voto %d: ", i);
        fgets(candidatoVoto, sizeof(candidatoVoto), stdin);
        if (!votar(candidatoVoto))
        {
            printf("Voto invalido.\n");
            return 4;
        }
    }

    // Mostra na tela o(s) ganhador(es)
    imprimirGanhador();
    return 0;
}

bool compararDuasStrings(char *string1, size_t lenString1, char *string2, size_t lenString2)
{
    if (lenString1 != lenString2)
    {
        return false;
    }
    for (int i = 0; i < lenString1; i++)
    {
        if (string1[i] != string2[i])
        {
            return false;
        }
    }
    return true;
}

void imprimirGanhador(void)
{
    int maiorVoto = 0;
    for (int i = 0; i < contagemCandidatos; i++)
    {
        if (listaCandidatos[i].quantidadeVotos > maiorVoto)
        {
            maiorVoto = listaCandidatos[i].quantidadeVotos;
        }
    }
    if (maiorVoto == 0)
    {
        return;
    }
    for (int i = 0; i < contagemCandidatos; i++)
    {
        if (listaCandidatos[i].quantidadeVotos == maiorVoto)
        {
            for (int j = 0; j < strlen(listaCandidatos[i].nome); j++)
            {
                printf("%c", listaCandidatos[i].nome[j]);
            }
            printf("\n");
        }
    }
}

int solicitarQuantidadeVotos(void)
{
    int votos = 1;
    int votosLido = 0;
    do
    {
        printf("Numero de eleitores: ");
        votosLido = scanf("%d", &votos);
        while (votosLido != 1)
        {
            scanf("%*[^\n]");
            printf("Numero de eleitores: ");
            votosLido = scanf("%d", &votos);
        }
    }
    while (votos < 1 || votos > QUANTIDADE_MAXIMA_VOTOS);
    return votos;
}

bool votar(char *nome)
{
    bool achouCandidato = false;

    for (int i = 0; i < contagemCandidatos; i++)
    {
        if (compararDuasStrings(listaCandidatos[i].nome,
            strlen(listaCandidatos[i].nome), nome, strlen(nome) - 1))
        {
            listaCandidatos[i].quantidadeVotos++;
            achouCandidato = true;
        }
    }
    if (achouCandidato == true)
    {
        return true;
    }
    return false;
}
