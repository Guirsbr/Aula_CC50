# DNA

import csv
import sys

def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Uso: dna.py database.csv sequence.txt")

    # TODO: Read database file into a variable
    try:
        with open(sys.argv[1], "r") as arquivo:
            dados_dnas = csv.reader(arquivo)

            dicionario_subsequencias = {}
            dicionario_nomes = {}
            first_row = True
            for row in dados_dnas:
                if first_row:
                    for i in range(1, len(row)):
                        dicionario_subsequencias[row[i]] = 0
                    first_row = False

                else:
                    dicionario_nomes[row[0]] = []
                    for i in range(1, len(row)):
                        dicionario_nomes[row[0]].append(row[i])

    except FileNotFoundError:
        sys.exit("Não foi possível abrir o banco de dados")

    # TODO: Read DNA sequence file into a variable
    try:
        with open(sys.argv[2], "r") as arquivo:
            dna = arquivo.read()

    except FileNotFoundError:
        sys.exit("Não foi possível abrir a sequência de DNA")

    # TODO: Find longest match of each STR in DNA sequence
    for subsequencia in dicionario_subsequencias:
        dicionario_subsequencias[subsequencia] = str(longest_match(dna, subsequencia))

    # TODO: Check database for matching profiles
    for nome in dicionario_nomes:
        pessoa_encontrada = True
        index = 0
        for subsequencia in dicionario_subsequencias:
            if dicionario_subsequencias[subsequencia] != dicionario_nomes[nome][index]:
                pessoa_encontrada = False
                break
            index += 1

        if pessoa_encontrada == True:
            print(nome)
            return

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()