# CREDIT (DESAFIO)

while True:
    numero_cartao = input("Digite o nome do cartão: ")
    try:
        int(numero_cartao)
        break
    except:
        continue

soma_digitos = 0
for i in range(len(numero_cartao) - 2, -1, -2):
    conta_auxiliar = str(int(numero_cartao[i]) * 2)
    for j in range(len(conta_auxiliar)):
        soma_digitos += int(conta_auxiliar[j])
for i in range(len(numero_cartao) - 1, -1, -2):
    soma_digitos += int(numero_cartao[i])

primeiros_2_digitos = numero_cartao[0] + numero_cartao[1]
if soma_digitos % 10 == 0:
    if primeiros_2_digitos in ["34", "37"] and len(numero_cartao) == 15:
        print("AMEX")
    elif primeiros_2_digitos in ["51", "52", "53", "54", "55"] and len(numero_cartao) == 16:
        print("MASTERCARD")
    elif primeiros_2_digitos[0] == "4" and len(numero_cartao) in [13, 16]:
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")