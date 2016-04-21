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
#include "cJSON.h"
#include "mini-map.h"
#include "header-getter.h"

int processing(parse_state state, mapStruct* map, Sreponse* reponse);

StringL uniformURI(StringL uri);
StringL extractSiteFromUURI(StringL uuri, cJSON* sites);


#endif
