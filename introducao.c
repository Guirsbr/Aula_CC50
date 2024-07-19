#include <stdio.h>

int main(void) {

    char userName[30];

    printf("Digite o seu nome: ");
    fgets(userName, sizeof(userName), stdin);
    
    size_t len = strlen(userName);
    if (len > 0 && userName[len-1] == '\n') {
    userName[--len] = '\0';
    }

    printf("Ola %s!", userName);
}