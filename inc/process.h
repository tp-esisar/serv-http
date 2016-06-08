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
#include "cJSON.h"
#include "mini-map.h"
#include "header-getter.h"
#include "reader.h"
#include "fcgi.h"

typedef struct {
    StringL scheme;
    StringL host;
    StringL query;
    StringL path;
    StringL userinfo;
    StringL port;
} URI_Info;

int processing(parse_state state, mapStruct* map, Sreponse* reponse, cJSON* config, cJSON* config_php);
StringL normalisation (StringL url);
URI_Info extractInfoFromURI(StringL uri);
char* get_final_file_path(URI_Info info, cJSON* jsonDB, StringL headerHost);


#endif
