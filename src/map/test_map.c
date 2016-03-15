
#include "api_test.h"
#include "StringL.h"
#include "map.h"



void test_chaine (mapStruct* map, char* txt, char* search, void (*callback)())
{
    StringL header1 = {txt,3};
    StringL header2 = {txt+4, 5};
    StringL header3 = {txt+10, 7};
    StringL header4a = {txt+19, 4};
    StringL header5a = {txt+41, 17};
    StringL header4 = {txt+19, 20};
    StringL header5 = {txt+41, 23};

    map = init_map(header1,header2 ,header3 );
    add_field(map, header4a, header4);
    add_field(map, header5a, header5);

    search_map(map,"start-line", callback);
    search_map(map,"request-line", callback);
    search_map(map,"methode", callback);
    search_map(map,"request-target", callback);
    search_map(map,"HTTP-version", callback);
    search_map(map,"Host", callback);
    search_map(map,"Host-header", callback);
    search_map(map,"host", callback);
    search_map(map,"test", callback);
    search_map(map,"Transfer-Encoding-header", callback);
    search_map(map,"Transfer-Encoding-Header", callback);
    search_map(map,"Transfer-Encoding", callback);

    free_map(map);
}

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

    retour = parser (txt, strlen(txt), argv[2], affiche_string);
    if (retour == -1)
        printf("--- Message correct ---\n");
    else
        printf("Erreur syntaxique ligne %d", retour);

    return EXIT_SUCCESS;
}
