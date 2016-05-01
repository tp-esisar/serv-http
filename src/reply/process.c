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
			reponse->messagebody = malloc(5*sizeof(char));
			reponse->messagebody[0]='G';
			reponse->messagebody[1]='\0';
			//accessFile(reponse, normaliseURL(getHost(map), get_target(map)));			
		}
		else if (stringLEq (map->methode, (StringL){"POST", 4}) == 1) {
			//Vérifier la taille
			error(reponse, "201", "Accepted");
		}
		else	
			error(reponse, "501", "Methode non supportee");
	}

	
	if (connectionType == NULL || getValue(connectionType->connection_option, (StringL){"Close", 5}).s != NULL ) {
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

URI_Info extractInfoFromURI(StringL uri) {
	StringL wBuff = uri;
	URI_Info uri_info = {{NULL,0},{NULL,0},{NULL,0},{NULL,0}};
	reader scheme_r = get_reader(scheme, &wBuff);
	reader colon_r = letter_builder(&wBuff,':');
	reader hier_part_r = get_reader(hier_part, &wBuff);
	reader qmark_r = letter_builder(&wBuff,'?');
	reader query_r = get_reader(query, &wBuff);
	reader hashtag_r = letter_builder(&wBuff,'#');
	reader fragment_r = get_reader(fragment,&wBuff);	
}