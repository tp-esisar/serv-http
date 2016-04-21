#include "process.h"

int processing(mapStruct* map, Sreponse* reponse) 
{
	Connection_HS* connectionType = get_Connection(map);

	if (map->http_version.s[5] != '1')
		error(reponse, "505", "Version de HTTP non supportee");
	else {
		//Normalisation URL
		if (stringLEq (map->methode, (StringL){"GET", 3}) == 1) {
			reponse->startline=startline ("200", "OK");
			reponse->messagebody = malloc(5*sizeof(char));
			reponse->messagebody[0]='G';
			reponse->messagebody[1]='\0';
			//accessFile(reponse, normaliseURL(getHost(map), get_target(map)));			
		}
		else if (stringLEq (map->methode, (StringL){"POST", 4}) == 1) {
			reponse->startline=startline ("202", "Accepted");
			reponse->messagebody = malloc(5*sizeof(char));
			reponse->messagebody[0]='G';
			reponse->messagebody[1]='\0';
		}
		else	
			error(reponse, "501", "Methode non supportee");
	}

	
	if (connectionType == NULL || getValue(connectionType->connection_option, (StringL){"Close", 5}).s != NULL ) {
		addHeaderfield(reponse, "Connection: Close");
		return 1;
	}
	else {
		addHeaderfield(reponse, "Connection: Keep-Alive");
		return 0;
	}

	return 1;
}
