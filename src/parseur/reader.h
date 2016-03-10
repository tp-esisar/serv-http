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

//type de closure de lecture (emulation du comportement d'une closure)
struct mult_free { 
    void* ctxt; //contexte d'execution de la closure
    int freed;
};
typedef struct {
    struct mult_free context_MF;
    read_return (*fun)(void*); //code de la closure
    void (*free)(struct mult_free*);
} reader;

//#define CALL_CLOSURE(NAME) NAME ## .closure(( ## NAME ## _context) ## NAME ## .context)
#define CALL_CLOSURE(NAME) NAME.fun(NAME.context_MF.ctxt)
#define FREE_CLOSURE(NAME) NAME->free(&NAME->context_MF)

//symboles syntaxiques simples
typedef enum {
    SP,
    HTAB,
    OWS,
    DIGIT,
    day
} syntaxe_elem;

//foncteur de lecture conditionnel (retourne une fonction qui lit un segment)
reader read(syntaxe_elem se, StringL* buff);


#endif