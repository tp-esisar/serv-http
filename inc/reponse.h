#ifndef REPONSE_H
#define REPONSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "request.h"

typedef struct {
    char* startline;
    char* headerfield;
	char* messagebody;
} Sreponse;

Sreponse* init_Sreponse (void);
char* startline (char* num, char* detail);
Sreponse* addHeaderfield(Sreponse* reponse, char* ajout);
message* SreponseToMessage (Sreponse* Sreponse);
void error (Sreponse* message, char* num, char* detail);

#endif 