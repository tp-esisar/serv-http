#ifndef MINIMAP_H
#define MINIMAP_H

#include "StringL.h"

struct Pair {
	StringL key;
	StringL value;
	struct Pair* next;
};
typedef struct Pair Pair;


Pair* addPair(Pair* list, StringL key, StringL value);

StringL getValue(Pair* list, StringL key);

void free_map(Pair* list);


#endif
