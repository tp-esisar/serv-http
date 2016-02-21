typedef enum header_name header_name;
enum header_name {
    a,z,e,r,t,y,u,i,o,p
} ;


typedef struct {
    char* s;
    unsigned int len;
} StringL;

typedef struct field_content field_content;
struct field_content {
    StringL content;
    field_content* suivant;
} ;

typedef struct field field;
struct field {
    header_name header_name;
    field_content* champ;
    field* suivant;
};


static field* add_map (field** map, header_name header);
field* recherche_map (field* map, header_name header);
field* add_element (field** map, header_name header, StringL element);
static field_content* add_champ (field_content** field, StringL string);
