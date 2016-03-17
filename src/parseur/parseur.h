
#ifndef PARSEUR_H
#define PARSEUR_H

#include "stringL.h"
#include "reader.h"

typedef enum {
    FAIL,
    SUCC,
} parse_state;

typedef struct {
    parse_state state;
    mapStruct* map;
	StringL buff;
} parse_return;

parse_return parse_HTTP_message(StringL* buff);




#endif
