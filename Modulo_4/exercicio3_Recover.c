// RECOVER

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define QUANTIDADE_BYTES_JPEG 512
#define TAMANHO_NOME_ARQUIVO_JPEG 8

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Filtra os possiveis erros de input
    if (argc != 2)
    {
        printf("Uso: recover [arquivo.raw]");
        return 1;
    }

    // Leitura do arquivo a ser recuperado os dados
    FILE *arquivoRecuperar = fopen(argv[1], "r");
    if (arquivoRecuperar == NULL)
    {
        printf("Nao foi possivel abrir o arquivo");
        return 2;
    }

    // Definicao das variaveis
    BYTE bufferJPEG[QUANTIDADE_BYTES_JPEG];
    char nomeArquivoJPEG[TAMANHO_NOME_ARQUIVO_JPEG];
    int contagemJPEGAchados = -1;
    FILE *JPEGRecuperado;

    // Loop que le cada bloco do arquivo
    while(fread(bufferJPEG, sizeof(BYTE), QUANTIDADE_BYTES_JPEG, arquivoRecuperar))
    {
        if (bufferJPEG[0] == 0xff && bufferJPEG[1] == 0xd8 && bufferJPEG[2] == 0xff && (bufferJPEG[3] & 0xf0) == 0xe0)
        {
            // Se achou um novo JPEG cria um arquivo em branco e abre ele em append
            if (contagemJPEGAchados > -1)
            {
                fclose(JPEGRecuperado);
            }
            contagemJPEGAchados++;
            sprintf(nomeArquivoJPEG, "%03i.jpg", contagemJPEGAchados);
            JPEGRecuperado = fopen(nomeArquivoJPEG, "w");
            fwrite(bufferJPEG, sizeof(BYTE), QUANTIDADE_BYTES_JPEG, JPEGRecuperado);
            fclose(JPEGRecuperado);
            JPEGRecuperado = fopen(nomeArquivoJPEG, "a");
        }
        else if (contagemJPEGAchados > -1)
        {
            // Copia os blocos que não são inicios de um JPEG para o arquivo criado
            fwrite(bufferJPEG, sizeof(BYTE), QUANTIDADE_BYTES_JPEG, JPEGRecuperado);
        }
    }
    fclose(JPEGRecuperado);
    fclose(arquivoRecuperar);
    return 0;
}
