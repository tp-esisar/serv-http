/**
 * \file liste.h
 * \brief Structure de la MAP
 *
 */

typedef enum header_name header_name;
enum header_name {
    a,z,e,r,t,y,u,i,o,p
} ;

/**
 * \struct StringL
 * \brief Poiteur sur chaine de caractère
 *
 */
typedef struct {
    char* s; /*!< Pointeur sur début de a chaine */
    unsigned int len; /*!< Longueur de la chaine */
} StringL;

/**
 * \struct field_content
 * \brief Maillon de la chaine contenant les header-fields
 *
 */
typedef struct field_content field_content;
struct field_content {
    StringL content; /*!< Contient la chaine de caractère */
    field_content* suivant; /*!< Pointeur sur le prochain élément */
} ;

/**
 * \struct field
 * \brief Maillon de la MAP du parseur
 *
 */
typedef struct field field;
struct field {
    header_name header_name; /*!< Pointeur sur le type de header */
    field_content* champ; /*!< Pointeur sur le contenu de ce header */
    field* suivant; /*!< Pointeur sur le prochain élément */
};


int add_element (field** map, header_name header, StringL element);
