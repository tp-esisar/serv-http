#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

int main (void)
{
    field* map = NULL;
    StringL test;

    printf("Programme de test pour la MAP\n");
    printf("%d\t ", add_element (&map, a, test));
    printf("%d\t ", add_element (&map, z, test));
    printf("%d\t ", add_element (&map, e, test));
    printf("%d\t ", add_element (&map, r, test));
    printf("%d\t ", add_element (&map, t, test));
    printf("%d\t ", add_element (&map, u, test));

    return EXIT_SUCCESS;
}
