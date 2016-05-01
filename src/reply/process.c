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

URI_Info extractInfoFromURI(StringL uri) {
	StringL wBuff = uri;
	URI_Info uri_info = {{NULL,0},{NULL,0},{NULL,0},{NULL,0},{NULL,0},{NULL,0}};
	//les 4 formes de la request_target
	reader origin_form_r = get_reader(origin_form, &wBuff);
	reader absolute_form_r = get_reader(absolute_form, &wBuff);
	reader authority_form_r = get_reader(authority_form, &wBuff);
	reader asterisk_form_r = get_reader(asterisk_form, &wBuff);
	//les recettes de origin_form
	reader absolute_path_r = get_reader(absolute_path, &wBuff);
	reader qmark_r = letter_Builder(&wBuff,'?');
	reader query_r = get_reader(query, &wBuff);
	//les recettes de absolute_form
	reader scheme_r = get_reader(scheme, &wBuff);
	reader colon_r = letter_Builder(&wBuff,':');
	reader hier_part_r = get_reader(hier_part, &wBuff);
	//les recettes de hier_part
	reader double_slash_r = get_reader(double_slash, &wBuff);
	reader authority_r = get_reader(authority, &wBuff);
	reader path_abempty_r = get_reader(path_abempty, &wBuff);
	reader path_absolute_r = get_reader(path_absolute, &wBuff);
	reader path_rootless_r = get_reader(path_rootless, &wBuff);
	reader path_empty_r = get_reader(path_abempty, &wBuff);
	//les recettes de authority
	reader userinfo_r = get_reader(userinfo, &wBuff);
	reader arobase_r = letter_Builder(&wBuff,'@');
	reader host_r = get_reader(host, &wBuff);
	reader port_r = get_reader(port, &wBuff);
	
	StringL wBuff_backup = wBuff;
	read_return temp_rr;
	//identification du type de request_target
	if( (temp_rr = CALL_CLOSURE(origin_form_r)).state == SUCC ) {
		wBuff = wBuff_backup;
		uri_info.path = CALL_CLOSURE(absolute_form_r).string;
		if( (CALL_CLOSURE(origin_form_r)).state == SUCC ) {
			uri_info.query = CALL_CLOSURE(query_r).string;
		}
	}
	else if( (temp_rr = CALL_CLOSURE(absolute_form_r)).state == SUCC ) {
		wBuff = wBuff_backup;
		uri_info.scheme = CALL_CLOSURE(scheme_r).string;
		
	}
	else if( (temp_rr = CALL_CLOSURE(authority_form_r)).state == SUCC ) {
		wBuff = wBuff_backup;
		if( (temp_rr = CALL_CLOSURE(userinfo_r)).state == SUCC && CALL_CLOSURE(arobase_r).state == SUCC) {
			uri_info.userinfo = temp_rr.string;
		}
		
		uri_info.host = CALL_CLOSURE(host_r).string;
		
		if( CALL_CLOSURE(colon_r).state == SUCC && (temp_rr = CALL_CLOSURE(port_r)).state == SUCC) {
			uri_info.port = temp_rr.string;
		}
		
	}
	else if( (temp_rr = CALL_CLOSURE(asterisk_form_r)).state == SUCC ) {
		uri_info.path = temp_rr.string;
	}
	
	
	return uri_info;
}
