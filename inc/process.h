#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "request.h"
#include "StringL.h"
#include "map.h"
#include "reponse.h"
#include "fichier.h"
//#include "cJSON.h"
#include "mini-map.h"
#include "header-getter.h"
#include "reader.h"

typedef struct {
    StringL protocol;
    StringL host;
    StringL query;
    StringL fragment;
} URI_Info;

int processing(parse_state state, mapStruct* map, Sreponse* reponse);

URI_Info extractInfoFromURI(StringL uri);


#endif
