#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

void affiche(StringL bloc)
{
    int i;
    for (i=0; i<bloc.len; i++)
        printf("%c", bloc.s[i]);
    printf("\n");
}


int main (void)
{
    mapStruct* map = NULL;
    char txt[] = "GET /test HTTP/10  Host: www.google.com  Transfer-Encoding :gzip ";
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

    search_map(map, "start-line", &affiche);
    search_map(map,"request-line", &affiche);
    search_map(map,"methode", &affiche);
    search_map(map,"request-target", &affiche);
    search_map(map,"HTTP-version", &affiche);
    search_map(map,"Host", &affiche);
    search_map(map,"Host-header", &affiche);
    search_map(map,"host", &affiche);
    search_map(map,"test", &affiche);
    search_map(map,"Transfer-Encoding-header", &affiche);
    search_map(map,"Transfer-Encoding-Header", &affiche);
    search_map(map,"Transfer-Encoding", &affiche);

    free_map(map);

    return EXIT_SUCCESS;
}
