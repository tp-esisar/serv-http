#ifndef FICHIER_H
#define FICHIER_H

#include <stdlib.h>
#include <stdio.h>
#include "reponse.h"
#include "header-getter.h"
#include "StringL.h"
#include <string.h>
#include <stdint.h>
#include "cJSON.h"
#include "process.h"

char* loadFile(FILE* file);
void accessFile (Sreponse* reponse, char *chemin, Authorization_HS* Authorization, mapStruct* map, cJSON* config_php_const, URI_Info uri_info);
int php_request (Sreponse* reponse, char *chemin, mapStruct* map, cJSON* config_php, StringL stdinbuf, URI_Info uri_info);

#endif // FICHIER_H 
