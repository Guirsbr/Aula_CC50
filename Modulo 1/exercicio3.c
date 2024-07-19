#include <math.h>
#include <stdio.h>

float solicitar_valor_troco(void);

int main(void)
{
    // Define o valor do troco
    float valorTrocoDollar = solicitar_valor_troco();
    int valorTrocoCent = round(valorTrocoDollar * 100);

    // Calcula a quantidade de moedas
    int quantidadeMoedas = 0;
    if ((valorTrocoCent / 25) != 0)
    {
        quantidadeMoedas += (valorTrocoCent / 25);
        valorTrocoCent -= (25 * (valorTrocoCent / 25));
    }
    if ((valorTrocoCent / 10) != 0)
    {
        quantidadeMoedas += (valorTrocoCent / 10);
        valorTrocoCent -= (10 * (valorTrocoCent / 10));
    }
    if ((valorTrocoCent / 5) != 0)
    {
        quantidadeMoedas += (valorTrocoCent / 5);
        valorTrocoCent -= (5 * (valorTrocoCent / 5));
    }
    if ((valorTrocoCent) != 0)
    {
        quantidadeMoedas += valorTrocoCent;
    }

    // Mostra na tela a quantidade de moedas
    printf("%d\n", quantidadeMoedas);
}

float solicitar_valor_troco(void)
{
    float valor = 1;
    float valorLido = 0;
    do
    {
        printf("Digite o valor do troco: ");
        valorLido = scanf("%f", &valor);
        while (valorLido != 1)
        {
            scanf("%*[^\n]");
            printf("Digite o valor do troco: ");
            valorLido = scanf("%f", &valor);
        }  
    }
    while (valor < 0);
    return valor;
}