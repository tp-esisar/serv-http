#include <stdio.h>
#include <stdlib.h>
#include "api_test.h"

int main (int argc, char *argv[])
{
    FILE* file;
    char* txt;
    int i =0, retour;

    if (argc != 3)
    {
        printf("Usage %s file search\n", argv[0]);
        exit(01);
    }

    file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Impossible d'ouvrir le fichier ! ");
        exit(01);
    }

    while (fgetc(file) != EOF)
        i++;
    txt = malloc((i+1)*sizeof(char));
    if (txt == NULL)
    {
        perror("Erreur d'allocation memoire");
        exit(1);
    }
    i=0; rewind(file);
    do
    {
        txt[i]=fgetc(file);
    } while (txt[i++] != EOF);
    txt[i]='\0';

    retour = parser (txt, strlen(txt), argv[2], &affiche);
    if (retour == -1)
        printf("--- Message correct ---\n");
    else
        printf("Erreur syntaxique ligne %d", retour);

    return EXIT_SUCCESS;
}
