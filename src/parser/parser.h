
#ifndef PARSER_H
#define PARSER_H

#include "stringL.h"

int parser (char *buf,unsigned int len,char *search, void (*callback)()) ;

#define stepch(a) (pstepch(&a))

typedef struct {
    StringL method;
    StringL request_target;
    StringL HTTP_version;
} request_line;

typedef request_line start_line;



#endif
