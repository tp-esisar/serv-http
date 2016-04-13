#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "process.h"
#include "reponse.h"
#include "request.h"

int main(int argc, char *argv[])
{
	message *requete = NULL; 
	message *reponse = NULL;
	Sreponse *Sreponse = NULL;
	parse_return parse;
	int close, debug = 0;
	
	if (argc < 2)
	{
		printf("Usage : %s port [d]", argv[0]);
		return (0);
	}
	if (argc == 2 && strcmp(argv[2], "d"))
		debug = 1;
	
	while ( 1 )
	{
		requete = getRequest(argv[1]);
		Sreponse = init_Sreponse ();
		if (debug == 1)
		{
			printf("Client [%d] [%s:%d]\n",requete->clientId,inet_ntoa(requete->clientAddress->sin_addr),htons(requete->clientAddress->sin_port));
			printf("Contenu requete %.*s\n\n",requete->len,requete->buf);  
		}

		parse = parse_HTTP_message((StringL){requete->buf, requete->len});
		if (parse.state == PARSE_FAIL)
		{
			if (parse.map == NULL )	
				error(Sreponse, "500", "Erreur de mémoire");
			else 
				error(Sreponse, "400", "Erreur de syntaxe");
		}
		else 
			close = processing(parse.map, Sreponse);

		reponse = SreponseToMessage(Sreponse);
		reponse->clientId=requete->clientId; 
		sendReponse(reponse); 
		if (debug == 1)
		{
			printf("Serveur [%d]\n",reponse->clientId);
			printf("Contenu reponse %.*s\n\n",reponse->len,reponse->buf);  
		}
		free(reponse->buf);
		free(reponse); 
		if (close == 1)
			requestShutdownSocket(reponse->clientId);
		freeRequest(requete);
	}