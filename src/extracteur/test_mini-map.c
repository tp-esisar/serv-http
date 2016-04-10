#include <stdio.h>
#include <stdlib.h>
#include "mini-map.h"
#include "StringL.h"


//printf("\x1b[31mThis is red text\x1b[0m\n");
//printf("\x1b[32mThis is green text\x1b[0m\n");

int main()
{
	 Pair* map = NULL;
	 int ok = 0;
	 StringL key1 = fromRegularString("key1");
	 StringL key2 = fromRegularString("key2");
	 StringL key3 = fromRegularString("key3");
	 StringL badkey = fromRegularString("badkey");
	 
	 StringL value1 = fromRegularString("value1");
	 StringL value2 = fromRegularString("value2");
	 StringL value3 = fromRegularString("value3");
	 
	 map = addPair(map,key1,value1);
	 map = addPair(map,key2,value2);
	 map = addPair(map,key3,value3);
	 
	 if(stringLEq(value1,getValue(map,key1)) != 1) ok++;
	 if(stringLEq(value2,getValue(map,key2)) != 1) ok++;
	 if(stringLEq(value3,getValue(map,key3)) != 1) ok++;
	 if(getValue(map,badkey).s != NULL) ok++;
	 
	 free(key1.s);
	 free(key2.s);
	 free(key3.s);
	 free(value1.s);
	 free(value2.s);
	 free(value3.s);
	 free_map(map);
	 if(ok != 0) {
	 	fprintf(stderr,"\x1b[31mtest mini-map FAILD\x1b[0m, %d tests failed\n",ok);
	 	return(1);
	 }
	 else {
	 	printf("\x1b[32mtest mini-map OK\x1b[0m\n");
	 	return(0);
	 }
	
}
