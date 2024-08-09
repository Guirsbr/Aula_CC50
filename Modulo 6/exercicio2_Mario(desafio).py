
while True:
    try:
        altura_piramide = int(input("Digite o Tamanho da pirâmide: "))  
    except:
        continue
    if 0 < altura_piramide < 9:
        break

numero_de_espacos = altura_piramide - 1
for i in range(1, altura_piramide + 1):
    for j in range(numero_de_espacos):
        print(" ", end="")
    for j in range(i):
        print("#", end="")
    print("  ", end="")
    for j in range(i):
        print("#", end="")
    print()
    numero_de_espacos -= 1