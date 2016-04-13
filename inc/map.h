/**
 * \file liste.h
 * \brief Structure de la MAP
 *
 */

#ifndef MAP_H
#define MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "StringL.h"
#include "api_test.h"

/**
 * \struct field
 * \brief Maillon de la MAP du parseur
 */
typedef struct field field;
struct field {
    StringL field_name;     /*!< Type de header */
    StringL header_field;   /*!< Contenu de ce header */
    field* suivant;         /*!< Pointeur sur le prochain élément */
};

/**
 * \struct mapStruct
 * \brief Structure de la MAP
 */
typedef struct mapStruct mapStruct;
struct mapStruct {
    StringL methode;            /**< Pointeur vers le méthode de la requête */
    StringL request_target;     /**< Pointeur vers la cible de la requête */
    StringL http_version;       /**< Pointeur vers la version de http de la requête */
    field* field;               /**< Pointeur sur la liste chainée contenant les header-fields */
    StringL message_body;
} ;

mapStruct* init_map (StringL methode, StringL target, StringL version);
void add_field (mapStruct* map, StringL field_name, StringL header_field);
int search_map (mapStruct* map, char* search, void (*callback)(char* found, unsigned int len));
void free_map (mapStruct* map);

#endif

