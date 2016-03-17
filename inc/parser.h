
#ifndef PARSEUR_H
#define PARSEUR_H

#include "StringL.h"
#include "reader.h"

int parser (char *buf,unsigned int len,char *search, void (*callback)()) ;

typedef enum {
    FAIL,
    SUCC,
} parse_state;

typedef struct {
    parse_state state;
    mapStruct* map;
} parse_return;

parse_return parse_HTTP_message(StringL* buff);




#endif
