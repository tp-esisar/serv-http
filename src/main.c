#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "parser.h"
#include "process.h"
#include "reponse.h"
#include "request.h"
#include "cJSON.h"

int main(int argc, char *argv[])
{
	message *requete = NULL; 
	message *reponse = NULL;
	Sreponse *Sreponse = NULL;
	parse_return parse;
	StringL wBuff;
	int close, debug = 0;
	
	if (argc < 2)
	{
		printf("Usage : %s port [d]\n", argv[0]);
		return (0);
	}
	if (argc == 3 && argv[2][0]=='d')
		debug = 1;

	FILE* file_config = fopen("../www/config.json", "r");
	if(file_config == NULL) {
		perror("Impossible de charger le fichier de configuration");
		exit(-1);
	}
	cJSON* config = cJSON_Parse(loadFile(file_config));
	fclose(file_config);

	while ( 1 )
	{
		requete = getRequest(atoi(argv[1]));
		wBuff.s = requete->buf ;
		wBuff.len = requete->len;
		Sreponse = init_Sreponse ();

		if (debug == 1)
			printf("\n\n---Client [%d] [%s:%d]\n%.*s\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port),requete->len,requete->buf);

		parse = parse_HTTP_message(&wBuff);
		close = processing(parse.state, parse.map, Sreponse, config);

		reponse = SreponseToMessage(Sreponse);
		reponse->clientId=requete->clientId; 
		sendReponse(reponse); 
		if (debug == 1)
			printf("\n\n---Serveur [%d]\n%.*s\n",reponse->clientId,reponse->len,reponse->buf);

		free(reponse->buf);
		free(reponse); 
		if (parse.map != NULL)	
			free_map(parse.map);

		if (close == 1)
			requestShutdownSocket(reponse->clientId);
		freeRequest(requete);
	}
}

