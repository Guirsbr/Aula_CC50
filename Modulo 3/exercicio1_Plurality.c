// PLURALITY

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define quantidadeMaximaCandidatos 9
#define tamanhoNomeCandidato 40

void imprimir_ganhador(void);
int solicitar_quantidade_votos(void);
bool votar(char *name);

typedef struct
{
    char nome[40];
    int quantidadeVotos;
} candidato;

candidato listaCandidatos[quantidadeMaximaCandidatos - 1];

int contagemCandidatos;

int main(int argc, char *argv[])
{
    // Filtra os possiveis erros de input
    if (argc < 2 || argc > 10)
    {
        printf("Usage: ./plurality name1 name2 ...(max 9)\n");
        return 1;
    }

    // Coloca os canditatos em uma lista
    contagemCandidatos = argc - 1;
    for (int i = 0; i < contagemCandidatos; i++)
    {
        for (int j = 0; j < strlen(argv[i + 1]); j++)
        {
            listaCandidatos[i].nome[j] = argv[i + 1][j];
        }
        listaCandidatos[i].quantidadeVotos = 0;
    }

    // Solicita a quantidade de eleitores/votos
    int quantidadeVotos = solicitar_quantidade_votos();

    // Solicita o voto para cada eleitor
    getchar();
    for (int i = 1; i < quantidadeVotos + 1; i++)
    {
        char candidatoVoto[tamanhoNomeCandidato];
        printf("Voto %d: ", i);
        fgets(candidatoVoto, sizeof(candidatoVoto), stdin);
        if (!votar(candidatoVoto))
        {
            printf("Voto invalido.\n");
        }
    }

    // Mostra na tela o(s) ganhador(es)
    imprimir_ganhador();
    return 0;
}

void imprimir_ganhador(void)
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

int solicitar_quantidade_votos(void)
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
    while (votos < 1);
    return votos;
}

bool votar(char *nome)
{
    bool achouCandidato = false;

    for (int i = 0; i < contagemCandidatos; i++)
    {
        bool nomesIguais = true;
        for (int j = 0; j < strlen(nome) - 1; j++)
        {
            if (nome[j] != listaCandidatos[i].nome[j])
            {
                nomesIguais = false;
            }
        }
        if (nomesIguais == true)
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
