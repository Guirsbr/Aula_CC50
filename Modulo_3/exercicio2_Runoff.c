// RUNOFF

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define QUANTIDADE_MAXIMA_CANDIDATOS 9
#define QUANTIDADE_MAXIMA_VOTANTES 100
#define TAMANHO_NOME_CANDIDATO 40

int acharMinimo(void);
bool compararDuasStrings(char *string1, size_t lenString1, char *string2, size_t lenString2);
void eliminar(int menorQuantidadeVotos);
bool empate(int menorQuantidadeVotos);
bool imprimirGanhador(void);
int solicitarQuantidadeVotantes(void);
void tabular(void);
bool votar(int votante, int rank, char *nome);

typedef struct
{
    char nome[TAMANHO_NOME_CANDIDATO];
    int quantidadeVotos;
    bool eliminado;
} candidato;

candidato listaCandidatos[QUANTIDADE_MAXIMA_CANDIDATOS - 1];
int preferencias[QUANTIDADE_MAXIMA_VOTANTES][QUANTIDADE_MAXIMA_CANDIDATOS];
int contagemVotantes;
int contagemCandidatos;

int main(int argc, char *argv[])
{
    // Filtra os possiveis erros de input
    if (argc < 2)
    {
        printf("Uso: runoff [candidato ...]\n");
        return 1;
    }

    // Coloca os canditatos em uma lista e define os campos da lista
    contagemCandidatos = argc - 1;
    if (contagemCandidatos > QUANTIDADE_MAXIMA_CANDIDATOS)
    {
        printf("Quantidade maxima de canditatos e %i\n", QUANTIDADE_MAXIMA_CANDIDATOS);
        return 2;
    }
    for (int i = 0; i < contagemCandidatos; i++)
    {
        for (int j = 0; j < strlen(argv[i + 1]); j++)
        {
            listaCandidatos[i].nome[j] = argv[i + 1][j];
        }
        listaCandidatos[i].quantidadeVotos = 0;
        listaCandidatos[i].eliminado = false;
    }

    // Solicita a quantidade de votantes
    contagemVotantes = solicitarQuantidadeVotantes();
    if (contagemVotantes > QUANTIDADE_MAXIMA_VOTANTES)
    {
        printf("Quantidade maxima de votantes e %i\n", QUANTIDADE_MAXIMA_VOTANTES);
        return 3;
    }

    // Solicita o voto para cada eleitor
    getchar();
    for (int i = 1; i < contagemVotantes + 1; i++)
    {
        for (int j = 1; j < contagemCandidatos + 1; j++)
        {
            char candidatoVoto[TAMANHO_NOME_CANDIDATO];
            printf("Votante %d, rank %d: ", i, j);
            fgets(candidatoVoto, sizeof(candidatoVoto), stdin);
            if (!votar(i - 1, j - 1, candidatoVoto))
            {
                printf("Voto invalido.\n");
                return 4;
            }
        }
        printf("\n");
    }

    // chama as funções do runoff até que haja(m) vencedor(es)
    while (true)
    {
        tabular();

        bool alguemGanhou = imprimirGanhador();
        if (alguemGanhou)
        {
            return 0;
        }

        int menorQuantidadeVotos = acharMinimo();
        bool foiEmpate = empate(menorQuantidadeVotos);

        if (foiEmpate)
        {
            for (int i = 0; i < contagemCandidatos; i++)
            {
                if (!listaCandidatos[i].eliminado)
                {
                    for (int j = 0; j < strlen(listaCandidatos[i].nome); j++)
                    {
                        printf("%c", listaCandidatos[i].nome[j]);
                    }
                    printf("\n");
                }
            }
            return 0;
        }

        eliminar(menorQuantidadeVotos);

        for (int i = 0; i < contagemCandidatos; i++)
        {
            listaCandidatos[i].quantidadeVotos = 0;
        }
    }
}

int acharMinimo(void)
{
    int menorQuantidadeVotos;
    bool primeiroDadoObtido = false;
    for (int i = 0; i < contagemCandidatos; i++)
    {
        if (!listaCandidatos[i].eliminado)
        {
            if (!primeiroDadoObtido)
            {
                menorQuantidadeVotos = listaCandidatos[i].quantidadeVotos;
                primeiroDadoObtido = true;
            }
            else if (listaCandidatos[i].quantidadeVotos < menorQuantidadeVotos)
            {
                menorQuantidadeVotos = listaCandidatos[i].quantidadeVotos;
            }
        }
    }
    return menorQuantidadeVotos;
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

void eliminar(int menorQuantidadeVotos)
{
    for (int i = 0; i < contagemCandidatos; i++)
    {
        if (listaCandidatos[i].quantidadeVotos == menorQuantidadeVotos)
        {
            listaCandidatos[i].eliminado = true;
        }
    }
}

bool empate(int menorQuantidadeVotos)
{
    for (int i = 0; i < contagemCandidatos; i++)
    {
        if (!listaCandidatos[i].eliminado)
        {
            if (listaCandidatos[i].quantidadeVotos > menorQuantidadeVotos)
            {
                return false;
            }
        }
    }
    return true;
}

bool imprimirGanhador(void)
{
    for (int i = 0; i < contagemCandidatos; i++)
    {
        if (listaCandidatos[i].quantidadeVotos > contagemVotantes / 2)
        {
            for (int j = 0; j < strlen(listaCandidatos[i].nome); j++)
            {
                printf("%c", listaCandidatos[i].nome[j]);
            }
            printf("\n");
            return true;
        }
    }
    return false;
}

int solicitarQuantidadeVotantes(void)
{
    int votantes = 1;
    int votantesLido = 0;
    do
    {
        printf("Numero de eleitores: ");
        votantesLido = scanf("%d", &votantes);
        while (votantesLido != 1)
        {
            scanf("%*[^\n]");
            printf("Numero de eleitores: ");
            votantesLido = scanf("%d", &votantes);
        }
    }
    while (votantes < 1 || votantes > QUANTIDADE_MAXIMA_VOTANTES);
    return votantes;
}

void tabular(void)
{
    for (int i = 0; i < contagemVotantes; i++)
    {
        for (int j = 0; j < contagemCandidatos; j++)
        {
            if (!listaCandidatos[preferencias[i][j]].eliminado)
            {
                listaCandidatos[preferencias[i][j]].quantidadeVotos++;
                break;
            }
        }
    }
}

bool votar(int votante, int rank, char *nome)
{
    for (int i = 0; i < contagemCandidatos; i++)
    {
        if (compararDuasStrings(listaCandidatos[i].nome,
            strlen(listaCandidatos[i].nome), nome, strlen(nome) - 1))
        {
            preferencias[votante][rank] = i;
            return true;
        }
    }
    return false;
}
