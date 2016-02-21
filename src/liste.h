typedef enum header_name header_name;
enum header_name {
    a,z,e,r,t,y,u,i,o,p
} ;


typedef struct {
    char* s;
    unsigned int len;
} StringL;

typedef struct elm_champ elm_champ;
struct elm_champ {
    StringL* content;
    elm_champ* suivant;
} ;

typedef struct elm_map elm_map;
struct elm_map {
    header_name header_name;
    elm_champ* champ;
    elm_map* suivant;
};


static elm_map* add_map (elm_map** map, header_name header);
elm_map* recherche_map (elm_map* map, header_name header);
elm_map* add_element (elm_map** map, header_name header, StringL element);
