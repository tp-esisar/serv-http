/**
 * \file liste.c
 * \brief API pour la gestion de la MAP
 *
 */
#include "map.h"
#include "api_test.h"

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
    {
        perror("Erreur d'allocation memoire");
        exit(1);
    }

    /**< Remplissage des champs de StartLine */
    map->methode = methode;
    map->request_target = target;
    map->http_version = version;
    map->field = NULL;
    return map;
}

/** \brief Fonction qui ajoute un header-field à la MAP
 *
 * \param   map     Pointeur sur la MAP
 * \param   header  Field-name du header-field à ajouter
 * \param   content Field-value du header-field à ajouter
 * \return  EXIT_SUCCESS    Si le header-field a bien été ajouté
 * \return  EXIT_FAILURE    Si le header-field n'a pas pu être ajouté car déjà présent
 *
 * A des fins d'optimisation, même si un parseur devrait collecter la totalité des header-field (même ceux en double),
 *  nous savons qu'une requête avec un champ en double ne pourra être traité, ici la fonction n'ajoute donc pas la
 *  nouvelle entrée et retourne plutôt un code d'erreur.
 */
int add_field (mapStruct* map, StringL header, StringL content)
{
    field* bloc = NULL;
    field* chaine = map->field;

    /**< On crée un nouveau bloc que l'on rempli avec les informations données en paramètre */
    bloc = malloc(sizeof(field));
    if (bloc == NULL)
    {
        perror("Erreur d'allocation memoire");
        exit(1);
    }
    bloc->field_name = header;
    bloc->header_field = content;
    bloc->suivant = NULL;

    /**< Si la MAP est vide, on insére le bloc en le premier élément */
    if (map->field == NULL)
        map->field = bloc;

    /**< Sinon on parcourt toute la liste chaine, on regardant si l'élément n'est pas déjà présent dans la liste
    * Si c'est le cas, on retourne une erreur
    * Sinon on ajoute l'élement à la fin de la liste */
    else
    {
        while(chaine->suivant != NULL)
        {
            if (stringLEq(header, chaine->field_name))
            {
                free(bloc);
                return EXIT_FAILURE;
            }
            chaine = chaine->suivant;
        }
        chaine->suivant = bloc;
    }
    return EXIT_SUCCESS;
}

/** \brief Fonction qui recherche un header-field dans la MAP
 *
 * \param   map         Pointeur sur la MAP
 * \param   search      field-name a rechercher
 * \param   *callback   Pointeur sur la fonction a appeler lorsque l'on a trouvé le field
 */
void search_map (mapStruct* map, char* search, void (*callback)(char* found, unsigned int len))
{
    int mode = 1; /**< Mode permet de définir si ce qui est demandé est un header complet (2), ou juste le champ (1) */
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
        temp.len = (map->http_version.s)-(map->methode.s)+(map->http_version.len);
        extract_stringL(temp, callback);
    }
    else if(strcmp("methode", search) == 0)
        extract_stringL(map->methode, callback);
    else if(strcmp("request-target", search) == 0)
        extract_stringL(map->request_target, callback);
    else if(strcmp("HTTP-version", search) == 0)
        extract_stringL(map->http_version, callback);

    /**< Tout ce qui concerne les requêtes qui liste des champs  */
    else if(strcmp("header-field", search) == 0)
    {
        while (bloc != NULL)
            extract_stringL(bloc->header_field, callback);
    }
    else if(strcmp("field-name", search) == 0)
    {
        while (bloc != NULL)
            extract_stringL(bloc->field_name, callback);
    }
    else if(strcmp("field-value", search) == 0)
    {
        while (bloc != NULL)
            extract_stringL(extract_fieldValue(bloc->header_field), callback);
    }

    /**< Pour finir, les formes classique : header-field... */
    else
    {
        if (strstr(search, "-header")) /**< Si on veut un header complet, on passe en mode 2 */
        {
            searchS.len -= 7;
            mode =2;
        }
        else if(!strcmp("cookie-string", search)) /**< Cas spécial du Cookie qui n'a pas la même syntaxe que les autres et que l'on normalise */
            searchS.len = 6;

        /**< On cherche le champ dans la liste chainée */
        while ((bloc != NULL) && (!stringLEq(searchS, bloc->field_name)))
            bloc = bloc->suivant;
        /**< Si on l'a trouvé et suivant dans quel mode on est, on envoie à callback le StringL contenant le champ recherché */
        if (bloc != NULL)
        {
            if (mode == 1)  /**< Si on veut juste le content, il faut l'extraire de la ligne */
                extract_stringL(extract_fieldValue(bloc->header_field), callback);
            else            /**< Sinon on renvoie toute la ligne */
                extract_stringL(bloc->header_field, callback);
        }

    }
}

/** \brief Fonction qui libère un bloc de la MAP et tout ces précédents (récursivement)
 *
 * \param bloc Pointeur sur le bloc à supprimer ainsi que ces précédents
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
