#include "process.h"

int processing(parse_state state, mapStruct* map, Sreponse* reponse, cJSON* config, cJSON* config_php) 
{
	int retour;	
	char* file;

	if (state == PARSE_FAIL)
	{
		error(reponse, "400", "400 : Erreur de syntaxe");
		return 1;
	}

	Connection_HS* connectionType = get_Connection(map);
	Content_Length_HS* Content_length = get_Content_Length(map);
	Authorization_HS* Authorization = get_Authorization(map);
	Host_HS* Host = get_Host(map);
	//TranfertEncoding_HS* TranfertEncoding = get_TranfertEncoding(map);

	if (map->http_version.s[5] != '1')
		error(reponse, "505", "505 : Version de HTTP non supportee");

	else if (Host == NULL) 
		error(reponse, "400", "400 : Host obligatoire");

	else {
		if (stringLEq (map->methode, (StringL){"GET", 3}) == 1) {
			reponse->startline=startline ("200", "OK");
			URI_Info uri_info = extractInfoFromURI(map->request_target);
			file = get_final_file_path(uri_info, config, Host->Host);
			accessFile(reponse, file, Authorization, map, config_php, uri_info);
			free(file);	
		}
		else if (stringLEq (map->methode, (StringL){"POST", 4}) == 1) {
			/*if (TransfertEncoding != NULL)
				error(reponse, "501", "Non implémenté");
			else*/ if (Content_length == NULL)
				error(reponse, "411", "411 : Content-length requis");
			else if(Content_length->next != NULL || map->message_body.len != Content_length->Content_Length) 
				error(reponse, "400", "400 : Erreur de Content_length");
			else {
				error(reponse, "201", "201 : Accepted");
				file = get_final_file_path(extractInfoFromURI(map->request_target), config, Host->Host);				
				if(php_request (reponse, file, map, config_php, map->message_body)==-1){
					error(reponse, "404", "404 : Erreur PHP");
				free(file);	
		}	
			}
		}
		else	
			error(reponse, "501", "501 : Methode non supportee");
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
	free_Content_Length_HS(Content_length);
	free_Authorization_HS(Authorization);
	free_Host_HS(Host);

	return retour;
}

StringL normalisation (StringL url)
{
    int i, j=0;
    char temp[]="  ";

    for (i=0; i<url.len; i++)
    {
        if (url.s[i] == '%') {
            temp[0]=url.s[i+1];
            temp[1]=url.s[i+2];
            url.s[j++] = (char)strtol(temp, NULL, 16);
            i +=2;
        }
        else if (url.s[i] == '.') {
            if (i == url.len-1 || url.s[i+1] == '/')
                i++;
            else if (url.s[i+1] == '.')
                if (i == url.len-2 || url.s[i+2] == '/')
                    i+=2;
                else
                    url.s[j++] = url.s[i];
            else
                url.s[j++] = url.s[i];

        }
        else
            url.s[j++] = url.s[i];
    }

    url.len = j;
    return url;
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
	read_return temp_2_rr;
	//identification du type de request_target
	if( (temp_rr = CALL_CLOSURE(origin_form_r)).state == SUCC ) {
		wBuff = wBuff_backup;
		uri_info.path = CALL_CLOSURE(absolute_path_r).string;
		if( (CALL_CLOSURE(qmark_r)).state == SUCC ) {
			uri_info.query = CALL_CLOSURE(query_r).string;
		}
	}
	else if( (temp_rr = CALL_CLOSURE(absolute_form_r)).state == SUCC ) {
		wBuff = wBuff_backup;
		uri_info.scheme = CALL_CLOSURE(scheme_r).string;
		CALL_CLOSURE(colon_r);
		CALL_CLOSURE(double_slash_r);
		if( (temp_rr = CALL_CLOSURE(authority_r)).state == SUCC && (temp_2_rr = CALL_CLOSURE(path_abempty_r)).state == SUCC) {
			uri_info.path = temp_2_rr.string;
			wBuff = wBuff_backup;
			if( (temp_rr = CALL_CLOSURE(userinfo_r)).state == SUCC && CALL_CLOSURE(arobase_r).state == SUCC) {
				uri_info.userinfo = temp_rr.string;
			}
			wBuff = wBuff_backup;
			uri_info.host = CALL_CLOSURE(host_r).string;
			
			if( CALL_CLOSURE(colon_r).state == SUCC && (temp_rr = CALL_CLOSURE(port_r)).state == SUCC) {
				uri_info.port = temp_rr.string;
			}
		}
		else {
			wBuff = wBuff_backup;
			if( (temp_rr = CALL_CLOSURE(path_absolute_r)).state == SUCC ) {
				uri_info.path = temp_rr.string;
			}
			else if( (temp_rr = CALL_CLOSURE(path_rootless_r)).state == SUCC ) {
				uri_info.path = temp_rr.string;
			}
			else if( (temp_rr = CALL_CLOSURE(path_empty_r)).state == SUCC ) {
				uri_info.path = temp_rr.string;
			}
		}
		
	}
	else if( (temp_rr = CALL_CLOSURE(authority_form_r)).state == SUCC ) {
		wBuff = wBuff_backup;
		if( (temp_rr = CALL_CLOSURE(userinfo_r)).state == SUCC && CALL_CLOSURE(arobase_r).state == SUCC) {
			uri_info.userinfo = temp_rr.string;
		}
		wBuff = wBuff_backup;
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


char* getIndex(char* folder) {
	
	int folderLen = strlen(folder);
	int i,k;
	FILE *fichier = NULL;
	
	
	char* htmlIndex = "index.html";
	int htmlIndexLen = strlen(htmlIndex);
	int totalHtmlLen = folderLen+htmlIndexLen;
	char* finalPathHtml = malloc((totalHtmlLen+1)*sizeof(char));
	for(i=0;i<folderLen;i++) {
		finalPathHtml[i] = folder[i];
	}
	for(k=0;k<htmlIndexLen;k++) {
		finalPathHtml[i+k]=htmlIndex[k];
	}
	finalPathHtml[i+k] = '\0';
	
	
	
	fichier = fopen(finalPathHtml,  "r");
	 
	if (fichier != NULL) {
		fclose(fichier);
		return finalPathHtml;
	}
	
	
	free(finalPathHtml);
	char* phpIndex = "index.php";
	int phpIndexLen = strlen(phpIndex);
	int totalPhpLen = folderLen+phpIndexLen;
	char* finalPathPhp = malloc((totalPhpLen+1)*sizeof(char));
	for(i=0;i<folderLen;i++) {
		finalPathPhp[i] = folder[i];
	}
	for(k=0;k<phpIndexLen;k++) {
		finalPathPhp[i+k]=phpIndex[k];
	}
	finalPathPhp[i+k] = '\0';
	
	fichier = fopen(finalPathPhp,  "r");
	
	if (fichier != NULL) {
		fclose(fichier);
		return finalPathPhp;
	}
	return NULL;
}

char* get_final_file_path(URI_Info info, cJSON* jsonDB, StringL headerHost) {
	cJSON* jsonPath;
	if(headerHost.s == NULL) {
		if(info.host.s == NULL) {
			if (cJSON_HasObjectItem(jsonDB,"default")) {
				jsonPath = cJSON_GetObjectItem(jsonDB,"default");
			}
			else {
				fprintf(stderr,"erreur pas de site par defaut");
				return NULL;
			}
		}
		else {
			char* temp = toRegularString(info.host);
			if (cJSON_HasObjectItem(jsonDB,temp)) {
				jsonPath = cJSON_GetObjectItem(jsonDB,temp);
			}
			else {
				fprintf(stderr,"erreur pas de site \"%s\"",temp);
				return NULL;
			}
			free(temp);
		}
	}
	else {
		char* temp = toRegularString(headerHost);
		if (cJSON_HasObjectItem(jsonDB,temp)) {
			jsonPath = cJSON_GetObjectItem(jsonDB,temp);
		}
		else {
			fprintf(stderr,"erreur pas de site \"%s\"",temp);
			return NULL;
		}
		free(temp);
	}
	char* relative;
	if(info.path.s != NULL) {
		info.path = normalisation(info.path);
		relative = toRegularString(info.path);
	}
	else {
		relative = toRegularString((StringL){"/",1});
	}
	
	char* absoluteGuillemet = cJSON_PrintUnformatted(jsonPath);
	int absoluteGuillemetLen = strlen(absoluteGuillemet);
	char* absolute = malloc((absoluteGuillemetLen)*sizeof(char));
	int j;
	for(j=0;j<absoluteGuillemetLen-2;j++) {
		absolute[j]=absoluteGuillemet[j+1];
	}
	absolute[j]='\0';
	free(absoluteGuillemet);
	int absoluteLen = strlen(absolute);
	int relativeLen = strlen(relative);
	int totalLen = absoluteLen+relativeLen;
	char* finalPath = malloc((totalLen+1)*sizeof(char));
	int i;
	for(i=0;i<absoluteLen;i++) {
		finalPath[i] = absolute[i];
	}
	int k;
	if(relative[0] != '/') {
		for(k=0;k<relativeLen;k++) {
			finalPath[i+k]=relative[k];
		}
	}
	else {
		for(k=0;k<relativeLen-1;k++) {
			finalPath[i+k]=relative[k+1];
		}
	}
	finalPath[i+k]='\0';
	
	
	if(finalPath[strlen(finalPath)-1]=='/') {
		char* Old = finalPath;
		finalPath = getIndex(Old);
		free(Old);
		if (finalPath == NULL) {
			fprintf(stderr,"erreur index.{html,php} introuvable");
			return NULL;
		}
		
	}
	
	
	printf("path : %s\n",finalPath);
	free(relative);
	free(absolute);
	return finalPath;
}

