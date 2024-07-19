#include <stdio.h>

long long solicitar_numero_cartao(void);

int main(void)
{
    // Define o numero do cartao
    long long numeroCartao;
    numeroCartao = solicitar_numero_cartao();

    // Variaveis auxiliares
    int primNumCartao;
    int soma1 = 0;
    int soma2 = 0;
    int contaAuxiliar;
    int contadorDigitos = 0;
    int alternador = 1;

    // Faz os calculos necessarios e armazena nas variaveis
    while (numeroCartao > 0)
    {
        if (alternador == 1)
        {
            soma1 += numeroCartao % 10;
            numeroCartao /= 10;
            alternador = 0;
        }
        else if (alternador == 0)
        {
            contaAuxiliar = (numeroCartao % 10) * 2;
            if (contaAuxiliar > 9)
            {
                soma2 += (contaAuxiliar % 10);
                contaAuxiliar /= 10;
                soma2 += contaAuxiliar;
            }
            else
            {
                soma2 += contaAuxiliar;
            }
            numeroCartao /= 10;
            alternador = 1;
        }
        if (numeroCartao < 100 && numeroCartao > 9)
        {
            primNumCartao = numeroCartao;
        }
        contadorDigitos++;
    }

    // Verifica as variaveis e mostra a resposta
    contaAuxiliar = (soma1 + soma2) % 10;
    if (contaAuxiliar == 0)
    {
        if ((primNumCartao == 34 || primNumCartao == 37) && contadorDigitos == 15)
        {
            printf("AMEX\n");
        }
        else if ((primNumCartao == 51 || primNumCartao == 52 || primNumCartao == 53 ||
                 primNumCartao == 54 || primNumCartao == 55) && contadorDigitos == 16)
        {
            printf("MASTERCARD\n");
        }
        else if ((primNumCartao / 10) == 4 && (contadorDigitos == 13 || contadorDigitos == 16))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}

long long solicitar_numero_cartao(void)
{
    long long numero = 1;
    long long numeroLido = 0;
    do
    {
        printf("Digite o numero do cartao: ");
        numeroLido = scanf("%lli", &numero);
        while (numeroLido != 1)
        {
            scanf("%*[^\n]");
            printf("Digite o numero do cartao: ");
            numeroLido = scanf("%lli", &numero);
        }
    }
    while (numero < 0);
    return numero;
}