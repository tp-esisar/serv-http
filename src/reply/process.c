#include "process.h"





int processing(mapStruct* map, Sreponse* reponse) 
{
	if (map->http_version.s[5] != '1')
		error(reponse, "505", "Version de HTTP non support�e");
	else {
		//Normalisation URL
		if (stringLEq (map->methode, (StringL){"GET", 3}) == 1) {
			reponse->startline=startline ("200", "OK");
			//accessFile(reponse, normaliseURL(getHost(map), get_target(map)));			
		}
		else if (stringLEq (map->methode, (StringL){"POST", 4}) == 1)
			printf("On ne r�pond pas encore au POST ...");
		else	
			error(reponse, "501", "M�thode non support�e");
	}
	
	addHeaderfield(reponse, "Connection: Close");
	reponse->messagebody = malloc(5*sizeof(char));
	reponse->messagebody[0]='G';
	reponse->messagebody[1]='\0';

	/*if (stringLEq (get_Connection(map), (StringL){"Close", 3}) == 1) {
		addHeaderfield(reponse, "Connection: Close");
		return 1;
	}
	else {
		addHeaderfield(reponse, "Connection: Keep-Alive");
		return 0;
	}*/
	return 1;
}
