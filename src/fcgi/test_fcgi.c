#include "fcgi.h"


int main() {
    //FCGI_Header head = make_FCGI_Header(FCGI_GET_VALUES,FCGI_NULL_REQUEST_ID,8,0);
    //FCGI_BeginRequestRecord* begin = make_FCGI_BeginRequestRecord(1,FCGI_RESPONDER,0);
    	FILE* file_config_php = fopen("../../www/config_php.json", "r");
	if(file_config_php == NULL) {
		perror("Impossible de charger le fichier de configuration php");
		exit(-1);
	}
	char* conf_php = loadFile(file_config);
	cJSON* config_php = cJSON_Parse(conf);
	fclose(file_config_php);
	free(conf_php);

    FCGI_Request((StringL){"yolo",4}, cJSON* param);
    
    return 0;
}
