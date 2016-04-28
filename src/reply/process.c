#include "process.h"

int processing(parse_state state, mapStruct* map, Sreponse* reponse) 
{
	int retour;	
	Connection_HS* connectionType = get_Connection(map);

	if (state == PARSE_FAIL)
	{
		if (map == NULL )	
			error(reponse, "500", "Erreur de mémoire");
		else 
			error(reponse, "400", "Erreur de syntaxe");
	}

	else if (map->http_version.s[5] != '1')
		error(reponse, "505", "Version de HTTP non supportee");

	else {
		//Normalisation URL
		if (stringLEq (map->methode, (StringL){"GET", 3}) == 1) {
			reponse->startline=startline ("200", "OK");
			accessFile(reponse, "../www/site1/index.html");		
		}
		else if (stringLEq (map->methode, (StringL){"POST", 4}) == 1) {
			//Vérifier la taille : map->message_body.len
			error(reponse, "201", "Accepted");
		}
		else	
			error(reponse, "501", "Methode non supportee");
	}

	
	if (connectionType != NULL && getValue(connectionType->connection_option, (StringL){"Close", 5}).s != NULL ) {
		addHeaderfield(reponse, "Connection: Close");
		retour = 1;
	}
	else {
		addHeaderfield(reponse, "Connection: Keep-Alive");
		retour = 0;
	}
	free_Connection_HS(connectionType);

	return retour;
}
