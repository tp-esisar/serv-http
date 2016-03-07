#ifndef READER_H
#define READER_H

#include "stringL.h"
#include <stdlib.h>

typedef enum {
    FAIL,
    SUCC,
} read_state;

typedef struct {
    read_state state;
    StringL string;
} read_return;

//type generique de reader:
typedef read_return (*reader)(void);

//symboles syntaxiques simples
typedef enum {
    SP,
    HTAB,
    OWS
} syntaxe_elem;

//foncteur de lecture conditionnel (retourne une fonction qui lit un segment)
reader read(syntaxe_elem se, StringL* buff);


#endif