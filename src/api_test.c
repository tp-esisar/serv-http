
#include "../inc/api_test.h"

int parser (char *buf, unsigned int len, char *search, void (*callback)(char* found, unsigned int len))
{

    return -1;
}

void extract_stringL (StringL field, void (*callback)(char* found, unsigned int len))
{
    callback(field.s, field.len);
}


