#ifndef FICHIER_H
#define FICHIER_H

#include <stdlib.h>
#include <stdio.h>
#include "reponse.h"
#include "header-getter.h"
#include "StringL.h"
#include <string.h>
#include <stdint.h>

char* loadFile(FILE* file);
void accessFile (Sreponse* reponse, char *chemin, Authorization_HS* Authorization, mapStruct* map);

#endif // FICHIER_H
