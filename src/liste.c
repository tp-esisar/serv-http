#include <stdlib.h>
#include <stdio.h>
#include "liste.h"

elm_map* add_element (elm_map** map, header_name header, StringL element)
{
    elm_map* bloc = NULL;
    if ((bloc = recherche_map (*map, header)) == NULL)
        bloc = add_map(map, header);
        // Ajouter StringL dans le bloc
    else
        ;// Ajouter StringL dans le bloc

    return bloc;

}


static elm_map* add_map (elm_map** map, header_name header)
{
    elm_map* bloc = NULL;
    elm_map* champ;

    bloc = malloc(sizeof(elm_map));
    if (bloc == NULL)
        return NULL;
    bloc->header_name = header;
    bloc->champ = NULL;
    bloc->suivant = NULL;

    if (*map == NULL)
        *map = bloc;
    else
    {
        champ = *map;
        while (champ->suivant != NULL)
            champ = champ->suivant;
        champ->suivant = bloc;
    }

    return bloc;

}

elm_map* recherche_map (elm_map* map, header_name header)
{
    while ((map != NULL)&&(map->header_name != header))
        map = map->suivant;

    return map;
}
