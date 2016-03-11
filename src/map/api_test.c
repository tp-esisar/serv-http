
#include "api_test.h"
#include "test_unitaire.h"

int parser (char *buf, unsigned int len, char *search, void (*callback)(char* found, unsigned int len))
{
    mapStruct* map = NULL;
    test_chaine (map, buf, search, callback);
    return -1;
}

void convert (StringL field, void (*callback)(char* found, unsigned int len))
{
    callback(field.s, field.len);
}

void affiche (char *found, unsigned len)
{
    int i;
    for (i=0; i<len; i++)
        printf("%c", found[i]);
    printf("\n");
}
