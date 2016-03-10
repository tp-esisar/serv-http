#ifndef API_TEST_H
#define API_TEST_H


#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

int parser (char *buf, unsigned int len, char *search, void (*callback)(char* found, unsigned int len));
void convert (StringL field, void (*callback)(char* found, unsigned int len));
void affiche (char *found, unsigned len);

#endif // API_TEST_H
