#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api_test.h"

void affiche_string (char *found, unsigned len)
{
    int i;
    for (i=0; i<len; i++)
        printf("%c", found[i]);
    printf("\n");
}

int main (int argc, char *argv[])
{
    FILE* file;
    char* txt;
    int i =0, retour;
    int silence = 0;

    if (argc < 3) { printf("Usage %s file search [silence]\n", argv[0]); exit(EXIT_FAILURE); }
    if ((argc >= 4) && (strcmp(argv[3],"silence") == 0)) {
        silence = 1;
    }

    file = fopen(argv[1], "r");
    if (file == NULL) { perror("Impossible d'ouvrir le fichier ! "); exit(EXIT_FAILURE); }

    while (fgetc(file) != EOF)
        i++;

    txt = malloc((i+1)*sizeof(char));
    if (txt == NULL)
    { perror("Erreur d'allocation memoire"); exit(EXIT_FAILURE); }

    i=0; rewind(file);
    do
    {
        txt[i]=fgetc(file);
    } while (txt[i++] != EOF);
    txt[i]='\0';

    retour = parser (txt, strlen(txt), argv[2], affiche_string);

    if(silence == 0) {
        if (retour == -1)
            printf("--- Message correct ---\n");
        else
            printf("Erreur syntaxique ligne %d\n", retour);
    }

    return EXIT_SUCCESS;
}
