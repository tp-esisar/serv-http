
#include "mini-map.h"
#include <stdlib.h>
#include <stdio.h>


Pair* addPair(Pair* list, StringL key, StringL value) {
	Pair* i;
	
	if ((i = malloc(sizeof(Pair))) == NULL) {
		perror("erreur malloc mini-map.c");
		exit(0);
	}
	i->key = key;
	i->value = value;
	i->next = list;
	return i;
}

StringL getValue(Pair* list, StringL key) {
	Pair* i;
	if (list == NULL) {
		return (StringL){NULL,0};
	}
	for(i=list;i!=NULL;i=i->next) {
		if(stringLEq(i->key,key) == 1) {
			return i->value;
		}
	}
	return (StringL){NULL,0};
}

void free_map(Pair* list) {
	if(list != NULL) {
		free_map(list->next);
		free(list);
	}
}
