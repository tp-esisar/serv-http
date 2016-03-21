
#include "StringL.h"
#include "map.h"

void affiche_string (char *found, unsigned len)
{
    int i;
    for (i=0; i<len; i++)
        printf("%c", found[i]);
    printf("\n");
}

void test_chaine (mapStruct* map, char* txt, void (*callback)())
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


    printf("\n\n--- Cherche : start-line \n"); printf("Trouvé : %d occurences\n", search_map(map,"start-line", callback));
    printf("\n\n--- Cherche : request-line \n"); printf("Trouvé : %d occurences", search_map(map,"request-line", callback));
    printf("\n\n--- Cherche : methode \n"); printf("Trouvé : %d occurences", search_map(map,"methode", callback));
    printf("\n\n--- Cherche : request-target \n"); printf("Trouvé : %d occurences", search_map(map,"request-target", callback));
    printf("\n\n--- Cherche : HTTP-version \n"); printf("Trouvé : %d occurences", search_map(map,"HTTP-version", callback));
    printf("\n\n--- Cherche : Host \n"); printf("Trouvé : %d occurences", search_map(map,"Host", callback));
    printf("\n\n--- Cherche : Host-header \n"); printf("Trouvé : %d occurences", search_map(map,"Host-header", callback));
    printf("\n\n--- Cherche : host \n"); printf("Trouvé : %d occurences", search_map(map,"host", callback));
    printf("\n\n--- Cherche : test \n"); printf("Trouvé : %d occurences", search_map(map,"test", callback));
    printf("\n\n--- Cherche : Transfer-Encoding-header \n"); printf("Trouvé : %d occurences", search_map(map,"Transfer-Encoding-header", callback));
    printf("\n\n--- Cherche : Transfer-Encoding-Header \n"); printf("Trouvé : %d occurences", search_map(map,"Transfer-Encoding-Header", callback));
    printf("\n\n--- Cherche : Transfer-Encoding \n"); printf("Trouvé : %d occurences", search_map(map,"Transfer-Encoding", callback));

    free_map(map);
}

int main (int argc, char *argv[])
{
    mapStruct* map = NULL;
    char txt[] = "GET /test HTTP/10  Host: www.google.com  Transfer-Encoding :gzip ";
    test_chaine (map, txt, affiche_string);
    return 0;
}
