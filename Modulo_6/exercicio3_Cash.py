# CASH

while True:
    try:
        valor_troco = int(100 * (float(input("Digite o troco: "))))  
        break
    except:
        continue

quantidade_moedas = 0
moedas = [25, 10, 5, 1]

for i in range(len(moedas)):
    verificacao = int(valor_troco / moedas[i])
    if verificacao != 0:
        quantidade_moedas += verificacao
        valor_troco -= verificacao * moedas[i]

print(quantidade_moedas)
