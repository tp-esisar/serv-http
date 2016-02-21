#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

int main (void)
{
    elm_map* map = NULL;
    StringL test;

    printf("Programme de test pour la MAP\n");
    printf("%d\t ", add_element (&map, a, test));
    printf("%d\t ", add_element (&map, z, test));
    printf("%d\t ", add_element (&map, e, test));
    printf("%d\t ", add_element (&map, r, test));
    printf("%d\t ", add_element (&map, t, test));
    printf("%d\t ", add_element (&map, u, test));

    printf("\n%d\t", recherche_map (map, p));
    printf("%d\t", recherche_map (map, r));
    return EXIT_SUCCESS;
}
