
#ifndef PARSEUR_H
#define PARSEUR_H

#include "stringL.h"

int parser (char *buf,unsigned int len,char *search, void (*callback)()) ;

typedef enum {
    FAIL,
    SUCC
} parse_status;


typedef struct {
    StringL method;
    StringL request_target;
    StringL HTTP_version;
} request_line;

typedef request_line start_line; //A transformer en union si necessaire mais non à première vu

typedef struct {
    start_line start_line;
    //MAP  header_fieldS;
    StringL message_body;
} HTTP_message;

parse_error parse_HTTP_message(StringL* buff, HTTP_message* HTTP_message);




#endif
