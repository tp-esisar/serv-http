/**
 * \file liste.c
 * \brief API pour la gestion de la MAP
 *
 */
#include "map.h"
#include <ctype.h>

/** \brief Fonction qui initialise la MAP en remplissant les champs de la StartLine
 *
 * \param   methode     Champ méthode de la requête
 * \param   target      Champ request-target de la requête
 * \param   version     Champ HTTP-version de la requête
 * \return  map         Pointeur sur la MAP crée
 */
mapStruct* init_map (StringL methode, StringL target, StringL version)
{
    mapStruct* map;
    /**< Allocation de la MAP */
    map = malloc(sizeof(mapStruct));
    if (map == NULL)
		return NULL;

    /**< Remplissage des champs de StartLine */
    map->methode = methode;
    map->request_target = target;
    map->http_version = version;
    map->field = NULL;
    map->message_body = (StringL){NULL,0};
    return map;
}

/** \brief Fonction qui ajoute un header-field à la MAP
 *
 * \param   map     Pointeur sur la MAP
 * \param   field_name  Field-name du header-field à ajouter
 * \param   header_field Field-value du header-field à ajouter
 *
 */
void add_field (mapStruct* map, StringL field_name, StringL header_field)
{
    field* bloc = NULL;

    /**< On crée un nouveau bloc que l'on rempli avec les informations données en paramètre */
    bloc = malloc(sizeof(field));
    if (bloc != NULL)
    {
        bloc->field_name = field_name;
		bloc->header_field = header_field;
		bloc->suivant = map->field;

		/**< On insére le bloc en tête de la liste (pour plus de performances) */
		map->field = bloc;
    }
    
}

/** \brief Fonction qui recherche un header-field dans la MAP
 *
 * \param   map         Pointeur sur la MAP
 * \param   search      field-name a rechercher
 * \param   *callback   Pointeur sur la fonction a appeler lorsque l'on a trouvé le field
 * \return  nombre      Nombre d'élèments trouvés qui ont été soumis à la fonctions callback
 */
int search_map (mapStruct* map, char* search, void (*callback)(char* found, unsigned int len))
{
    int mode = 1; /**< Mode permet de définir si ce qui est demandé est un header complet (2), ou juste le champ (1) */
    int compteur = 0; /**< Compteur du nombre d'occurences trouvées */
    field* bloc = map->field;
    StringL temp, searchS;

    /**< Conversion de la chaine de caractère en StringL pour pouvoir faire les comparaisons */
    searchS.s = search;
    searchS.len = strlen(search);

    /**< On compare tout d'abord les champs spéciaux (pas sous la forme header-field) */

    /**< Tout ce qui concerne la StartLine (methode, request-target, HTTP-version) */
    if ((strcmp("start-line", search) == 0)||(strcmp("request-line", search) == 0))
    {
        temp = map->methode;
        temp.len = (map->http_version.s)-(map->methode.s)+(map->http_version.len)+2;
        extract_stringL(temp, callback);
        compteur ++;
    }
    else if(strcmp("method", search) == 0)
    {
        extract_stringL(map->methode, callback);
        compteur ++;
    }
    else if(strcmp("request-target", search) == 0)
    {
        extract_stringL(map->request_target, callback);
        compteur ++;
    }
    else if(strcmp("HTTP-version", search) == 0)
    {
        extract_stringL(map->http_version, callback);
        compteur ++;
    }

    /**< Tout ce qui concerne les requêtes qui liste des champs  */
    else if(strcmp("header-field", search) == 0)
    {
        while (bloc != NULL) {
            extract_stringL(bloc->header_field, callback);
            bloc = bloc->suivant;
            compteur ++;
        }
    }
    else if(strcmp("field-name", search) == 0)
        while (bloc != NULL)
        {
            extract_stringL(bloc->field_name, callback);
            bloc = bloc->suivant;
            compteur ++;
        }

    else if(strcmp("field-value", search) == 0)
        while (bloc != NULL)
        {
            extract_stringL(extract_fieldValue(bloc->header_field), callback);
            bloc = bloc->suivant;
            compteur ++;
        }
        
    else if(strcmp("message-body", search) == 0)
    {
        extract_stringL(map->message_body, callback);
        compteur ++;
    }

    /**< Pour finir, les formes classique : header-field... */
    else
    {
        if (strstr(search, "-header")) /**< Si on veut un header complet, on passe en mode 2 */
        {
            searchS.len -= 7;
            mode =2;
        }
        else if(strcmp("cookie-string", search) == 0) /**< Cas spécial du Cookie qui n'a pas la même syntaxe que les autres et que l'on normalise */
        {
            searchS.len = 6;
            searchS.s[0] = toupper(searchS.s[0]);
        }
        if(strstr("cookie", search)!=NULL) /**< Cas spécial du Cookie qui n'a pas la même syntaxe que les autres et que l'on normalise */
        {
            searchS.s[0] = toupper(searchS.s[0]);
        }

        /**< On cherche les champs correspondants dans la liste chainée */
        while (bloc != NULL)
		{
            
			if (stringLEq(searchS, bloc->field_name)) /**< Si on l'a trouvé et suivant dans quel mode on est, on envoie à callback le StringL contenant le champ recherché */
			{
				if (mode == 1)  /**< Si on veut juste le content, il faut l'extraire de la ligne */
				{
					extract_stringL(extract_fieldValue(bloc->header_field), callback);
					compteur ++;
				}
				else            /**< Sinon on renvoie toute la ligne */
				{
					extract_stringL(bloc->header_field, callback);
					compteur ++;
				}
			}
			bloc = bloc->suivant;
		}
    }
    return compteur;
}

/** \brief Fonction qui libère un bloc de la MAP et tout ces suivants (récursivement)
 *
 * \param bloc Pointeur sur le bloc à supprimer ainsi que ces suivants
 */
static void free_bloc(field* bloc)
{
    if (bloc->suivant == NULL)
        free(bloc);
    else
    {
        free_bloc(bloc->suivant);
        free(bloc);
    }
}

/** \brief Fonction qui libère la mémoire utilisée par la MAP
 *
 * \param map Pointeur sur la MAP
 */
void free_map(mapStruct* map)
{
    free_bloc(map->field);
    free(map);
}
