// TIDEMAN

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define QUANTIDADE_MAXIMA_CANDIDATOS 9
#define QUANTIDADE_MAXIMA_VOTANTES 100
#define TAMANHO_NOME_CANDIDATO 40

// preferences[i][j] is number of voters who prefer i over j
int preferencias[QUANTIDADE_MAXIMA_CANDIDATOS][QUANTIDADE_MAXIMA_CANDIDATOS];

// locked[i][j] means i is locked in over j
bool presos[QUANTIDADE_MAXIMA_CANDIDATOS][QUANTIDADE_MAXIMA_CANDIDATOS];

// Each pair has a winner, loser
typedef struct
{
    int ganhador;
    int perdedor;
} par;

// Array of candidates
char candidatos[QUANTIDADE_MAXIMA_CANDIDATOS][TAMANHO_NOME_CANDIDATO];
par pares[QUANTIDADE_MAXIMA_CANDIDATOS * (QUANTIDADE_MAXIMA_CANDIDATOS - 1) / 2];

int contagemPares = 0;
int contagemCandidatos;

// Function prototypes
void AdicionarPares(void);
bool compararDuasStrings(char *string1, size_t lenString1, char *string2, size_t lenString2);
void gravarPreferencias(int ranques[]);
void imprimirGanhador(void);
void ordenarPares(void);
void prenderPares(void);
int solicitarQuantidadeVotantes(void);
bool temCiclo(int ganhador, int perdedor);
bool votar(int ranque, char *nome, int ranques[]);

int main(int argc, char *argv[])
{
    // Filtra os possiveis erros de input
    if (argc < 2)
    {
        printf("Uso: tideman [candidate ...]\n");
        return 1;
    }

    // Coloca os canditatos em uma lista
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
            candidatos[i][j] = argv[i + 1][j];
        }
    }

    // Define falso para todos os pares
    for (int i = 0; i < contagemCandidatos; i++)
    {
        for (int j = 0; j < contagemCandidatos; j++)
        {
            presos[i][j] = false;
        }
    }
    contagemPares = 0;

    // Solicita a quantidade de votantes
    int contagemVotantes = solicitarQuantidadeVotantes();
    if (contagemVotantes > QUANTIDADE_MAXIMA_VOTANTES)
    {
        printf("Quantidade maxima de votantes e %i\n", QUANTIDADE_MAXIMA_VOTANTES);
        return 3;
    }

    // Solicita o voto para cada eleitor
    getchar();
    for (int i = 1; i < contagemVotantes + 1; i++)
    {

        int ranques[contagemCandidatos];

        for (int j = 1; j < contagemCandidatos + 1; j++)
        {
            char candidatoVoto[TAMANHO_NOME_CANDIDATO];
            printf("Votante %d, rank %d: ", i, j);
            fgets(candidatoVoto, sizeof(candidatoVoto), stdin);

            if (!votar(j - 1, candidatoVoto, ranques))
            {
                printf("Voto invalido.\n");
                return 4;
            }
        }
        gravarPreferencias(ranques);

        printf("\n");
    }

    AdicionarPares();
    ordenarPares();
    prenderPares();
    imprimirGanhador();
    return 0;
}

void AdicionarPares(void)
{
    for (int i = 0; i < contagemCandidatos; i++)
    {
        for (int j = 0; j < contagemCandidatos; j++)
        {
            if (preferencias[i][j] > preferencias[j][i])
            {
                pares[contagemPares].ganhador = i;
                pares[contagemPares].perdedor = j;
                contagemPares++;
            }
        }
    }
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

void gravarPreferencias(int ranques[])
{
    for (int i = 0; i < contagemCandidatos - 1; i++)
    {
        for (int j = i; j < contagemCandidatos - 1; j++)
        {
            preferencias[ranques[i]][ranques[j + 1]]++;
        }
    }
}

void imprimirGanhador(void)
{
    for (int i = 0; i < contagemCandidatos; i++)
    {
        bool fonteAchada = true;
        for (int j = 0; j < contagemCandidatos; j++)
        {
            if (presos[i][j] == true)
            {
                fonteAchada = false;
                break;
            }
        }
        if (fonteAchada)
        {
            for (int j = 0; j < strlen(candidatos[i]); j++)
            {
                printf("%c", candidatos[i][j]);
            }
            printf("\n");
            return;
        }
    }
}

void ordenarPares(void)
{
    bool trocou = true;
    while (trocou)
    {
        trocou = false;
        for (int i = 0; i < contagemPares - 1; i++)
        {
            int diferencaPontosPar1 = preferencias[pares[i].ganhador][pares[i].perdedor] -
                                      preferencias[pares[i].perdedor][pares[i].ganhador];
            int diferencaPontosPar2 = preferencias[pares[i+1].ganhador][pares[i+1].perdedor] -
                                      preferencias[pares[i+1].perdedor][pares[i+1].ganhador];
            if ((diferencaPontosPar1) < (diferencaPontosPar2))
            {
                int auxiliarGanhador = pares[i].ganhador;
                int auxiliarPerdedor = pares[i].perdedor;
                pares[i].ganhador = pares[i+1].ganhador;
                pares[i].perdedor = pares[i+1].perdedor;
                pares[i+1].ganhador = auxiliarGanhador;
                pares[i+1].perdedor = auxiliarPerdedor;
                trocou = true;
            }
        }
    }
}

void prenderPares(void)
{
    for (int i = 0; i < contagemPares; i++)
    {
        if (!temCiclo(pares[i].ganhador, pares[i].perdedor))
        {
            presos[pares[i].ganhador][pares[i].perdedor] = true;
        }
    }
}

bool temCiclo(int ganhador, int perdedor)
{
    if (presos[perdedor][ganhador] == true)
    {
        return true;
    }
    for (int i = 0; i < contagemCandidatos; i++)
    {
        if (presos[perdedor][i] == true && temCiclo(ganhador, i))
        {
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

bool votar(int ranque, char *nome, int ranques[])
{
    for (int i = 0; i < contagemCandidatos; i++)
    {
        if (compararDuasStrings(candidatos[i],
            strlen(candidatos[i]), nome, strlen(nome) - 1))
        {
            ranques[ranque] = i;
            return true;
        }
    }
    return false;
}
