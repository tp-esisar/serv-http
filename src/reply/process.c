#include "process.h"

/*normaliseURL

fichier= ?*/


int processing(mapStruct* map, Sreponse* reponse) 
{
	if (map->request_target[5] != 1)
		error(reponse, "505", "Version de HTTP non supportée");
	else {
		//Normalisation URL
		if (stringLEq (map->methode, (StringL){"GET", 3}) == 1) {
			reponse->startline=startline ("200", "OK");
			//accessFile(reponse, normaliseURL(getHost(map), get_target(map)));			
		}
		else if (stringLEq (map->methode, (StringL){"POST", 4}) == 1)
			printf("On ne répond pas encore au POST ...");
		else	
			error(reponse, "501", "Méthode non supportée");
	}
	
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
