#include "fcgi.h"
#include <stdio.h>
#include <stdlib.h>
#include "fichier.h"


int main() {
	//FCGI_Header head = make_FCGI_Header(FCGI_GET_VALUES,FCGI_NULL_REQUEST_ID,8,0);
	//FCGI_BeginRequestRecord* begin = make_FCGI_BeginRequestRecord(1,FCGI_RESPONDER,0);
	FILE* file_config_php = fopen("../../www/config_php_test.json", "r");
	if(file_config_php == NULL) {
		perror("Impossible de charger le fichier de configuration php");
		exit(-1);
	}
	char* conf_php = loadFile(file_config_php);
	cJSON* config_php = cJSON_Parse(conf_php);
	fclose(file_config_php);
	free(conf_php);
	if(!cJSON_HasObjectItem(config_php,"ip")) {
		fprintf(stderr,"erreur ip manquant dans json");
		return -1;
	}
	char* ip = cJSON_GetObjectItem(config_php,"ip")->valuestring;
	if(!cJSON_HasObjectItem(config_php,"port")) {
		fprintf(stderr,"erreur port manquant dans json");
		return -1;
	}
	int port = cJSON_GetObjectItem(config_php,"port")->valueint;
	AppResult result = FCGI_Request((StringL){"name=coucou",11}, config_php,ip,port);
	if (result.status == -1)
		return -1;

	printf("Erreur : \n%.*s\n", result.stderr.len, result.stderr.s);

	if (result.stdout.len == 0)
		return -1;

	printf("STDOUT : \n%.*s\n", result.stdout.len, result.stdout.s);
    
    return 0;
}