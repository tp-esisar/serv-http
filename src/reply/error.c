#include "error.h"

message* error (char* num, char* detail)
{
	message* message = NULL;
	int taille = 8+1+3+1+strlen(detail)+2;

	message = malloc(sizeof(message));

	if (message == NULL)
	{
		perror ("Erreur d'allocation mémoire");
		exit(1);
	}

	message->buf = malloc(taille*sizeof(char));

	if (message->buf == NULL)
	{
		perror ("Erreur d'allocation mémoire");
		exit(1);
	}

	strcpy(message->buf, "HTTP/1.1 ");
	strcpy(&(message->buf[9]), num);
	message->buf[12] = ' ';
	strcpy(&(message->buf[13]), detail);
	message->buf[taille-2]='\r';
	message->buf[taille-1]='\n';

    message->len = taille;
	return message;
}


