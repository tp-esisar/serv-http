#ifndef PARSER_H
#define PARSER_H

#include "StringL.h"
#include "reader.h"
#include "map.h"

typedef enum {
    PARSE_FAIL,
    PARSE_SUCC,
} parse_state;

typedef struct {
    parse_state state;
    mapStruct* map;
	StringL buff;
} parse_return;

parse_return parse_HTTP_message(StringL* buff);

#endif
