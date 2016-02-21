#include <stdlib.h>
#include <stdio.h>
#include "liste.h"

field* add_element (field** map, header_name header, StringL element)
{
    field* bloc = NULL;
    //field_content* content = NULL;

    if ((bloc = recherche_map (*map, header)) == NULL)
    {
        bloc = add_map(map, header);
        add_champ (&(bloc->champ), element);
    }

    else
        add_champ (&(bloc->champ), element);

    return bloc;

}


static field* add_map (field** map, header_name header)
{
    field* bloc = NULL;
    field* champ;

    bloc = malloc(sizeof(field));
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

field* recherche_map (field* map, header_name header)
{
    while ((map != NULL)&&(map->header_name != header))
        map = map->suivant;

    return map;
}

static field_content* add_champ (field_content** field, StringL string)
{
    field_content* bloc = NULL;
    field_content* champ;

    bloc = malloc(sizeof(field_content));
    if (bloc == NULL)
        return NULL;
    bloc->content = string;
    bloc->suivant = NULL;

    if (*field == NULL)
        *field = bloc;
    else
    {
        champ = *field;
        while (champ->suivant != NULL)
            champ = champ->suivant;
        champ->suivant = bloc;
    }

    return bloc;

}
