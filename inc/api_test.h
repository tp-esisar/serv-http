#ifndef API_TEST_H
#define API_TEST_H

#include <stdio.h>
#include <stdlib.h>
#include "map.h"

int parser (char *buf, unsigned int len, char *search, void (*callback)(char* found, unsigned int len));
void extract_stringL (StringL field, void (*callback)(char* found, unsigned int len));

#endif // API_TEST_H
