
texto_analisar = input("Digite o texto a ser analisado: ")

contagemLetras = 0
contagemSentenca = 0
contagemPalavras = 1
for i in range(len(texto_analisar)):
    if texto_analisar[i].isalpha():
        contagemLetras += 1
    elif texto_analisar[i] in ['.', '!', '?']:       
        contagemSentenca += 1
    elif texto_analisar[i] == " ":
        contagemPalavras += 1
        
indice = int(round(0.0588 * ((contagemLetras * 100) / contagemPalavras) -
                   0.296 * ((contagemSentenca * 100) / contagemPalavras) - 15.8))
if indice < 1:
    print("Before Grade 1")
elif indice >= 16:
    print("Grade 16+")
else:
    print(f"Grade {indice}")