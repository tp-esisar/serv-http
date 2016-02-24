/**
 * \file liste.c
 * \brief API pour la gestion de la MAP
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "liste.h"

/** \brief Fonction qui permet de créer de nouveau bloc dans la MAP
 *
 * \param map Pointeur sur la MAP
 * \param header Type de header qu'on l'on souhaite créer
 * \return Adresse bloc Si le bloc a été correctement crée
 * \return NULL Sinon
 *
 */
static field* add_map (field** map, header_name header)
{
    field* bloc = NULL;
    field* current;

    bloc = malloc(sizeof(field));
    if (bloc == NULL)
        return NULL;
    bloc->header_name = header;
    bloc->champ = NULL;
    bloc->suivant = NULL;

    if (*map == NULL) /**< Si c'est le premier bloc à être crée, on début la liste chainée */
        *map = bloc;
    else /**< Sinon on ajoute le bloc à la suite */
    {
        current = *map;
        while (current->suivant != NULL)
            current = current->suivant;
        current->suivant = bloc;
    }

    return bloc;
}

/** \brief Fonction qui permet de rechercher un header-field particulier dans une MAP
 *
 * \param map MAP dans lequel on fait la recherche
 * \param header header-field à rechercher
 * \return Pointeur sur le bloc qui contient le header recherché
 * \return NULL s'il n'existe pas
 *
 */
static field* recherche_map (field* map, header_name header)
{
    while ((map != NULL)&&(map->header_name != header))
        map = map->suivant;

    return map;
}

/** \brief Fonction qui ajoute le contenu d'un header-field dans la MAP
 *
 * \param field Pointeur sur le bloc de la MAP dans lequel il faut ajouter le contenu
 * \param string Le contenu à ajouter
 * \return EXIT_SUCCESS Si l'élément a bien été ajouté
 * \return EXIT_FAILURE Si l'élément n'a pas pu être ajouté
 *
 */
static int add_champ (field_content** field, StringL string)
{
    field_content* bloc = NULL;
    field_content* champ;

    bloc = malloc(sizeof(field_content));
    if (bloc == NULL)
        return EXIT_FAILURE;
    bloc->content = string;
    bloc->suivant = NULL;

    if (*field == NULL) /**< Si c'est le premier élément du bloc, on crée une nouvelle liste chainée */
        *field = bloc;
    else /**< Sinon on le met à la suite */
    {
        champ = *field;
        while (champ->suivant != NULL)
            champ = champ->suivant;
        champ->suivant = bloc;
    }

    return EXIT_SUCCESS;
}

/** \brief Fonction qui permet d'ajouter un header-field dans la MAP
 *
 * \param map Pointeur sur la MAP
 * \param header Type de header qu'on l'on souhaite ajouter
 * \param element L'élément à ajouter
 * \return EXIT_SUCCESS Si l'élément a bien été ajouté
 * \return EXIT_FAILURE Si l'élément n'a pas pu être ajouté
 */
int add_element (field** map, header_name header, StringL element)
{
    field* bloc = NULL;

    if ((bloc = recherche_map (*map, header)) == NULL) /**< Si le bloc n'existe pas, on le crée */
    {
        bloc = add_map(map, header);
        return add_champ (&(bloc->champ), element);
    }

    else  /**< Sinon on ajoute l'information dans le bloc */
        return add_champ (&(bloc->champ), element);
}
