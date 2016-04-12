#include "reponse.h"

Sreponse* init_Sreponse (void)
{
	Sreponse* reponse = malloc(sizeof(Sreponse));
	if (reponse == NULL)
	{
		perror ("Erreur d'allocation mémoire");
		exit(1);
	}
	reponse->startline = NULL;
	reponse->headerfield = NULL;
	reponse->messagebody = NULL;
}

char* startline (char* num, char* detail)
{
	int taille = 8+1+3+1+strlen(detail)+2;

	line = malloc((taille+1)*sizeof(char));

	if (line == NULL)
	{
		perror ("Erreur d'allocation mémoire");
		exit(1);
	}

	strcpy(line, "HTTP/1.1 ");
	strcpy(&(line[9]), num);
	line[12] = ' ';
	strcpy(&(line[13]), detail);
	line[taille-2]='\r';
	line[taille-1]='\n';
	line[taille]='\0';

	return line;
}

Sreponse* addHeaderfield(Sreponse* reponse, char* ajout)
{
	reponse->headerfield = realloc(reponse->headerfield, strlen(reponse)+strlen(ajout)+1);
	if (reponse->headerfield == NULL)
	{
		perror ("Erreur d'allocation mémoire");
		exit(1);
	}
	
	strcpy(&(reponse->headerfield[strlen(reponse)]), ajout);
	reponse->headerfield[strlen(reponse)+strlen(ajout)] = '\0';
	
	return reponse;
}

message* SreponseToMessage (Sreponse* Sreponse)
{
	message* reponse = malloc(sizeof(message));
	if (reponse == NULL)
	{
		perror ("Erreur d'allocation mémoire");
		exit(1);
	}
	
	reponse->buf = malloc(sizeof(char)*(strlen(Sreponse->startline)+strlen(Sreponse->headerfield)+strlen(Sreponse->messagebody)+2));
	if (reponse->buf == NULL)
	{
		perror ("Erreur d'allocation mémoire");
		exit(1);
	}
	
	reponse->taille = strlen(Sreponse->startline)+strlen(Sreponse->headerfield)+strlen(Sreponse->messagebody)+2;
	
	memcpy(message->buf,Sreponse->startline, strlen(Sreponse->startline));
	memcpy(&(message->buf[strlen(Sreponse->startline)]),Sreponse->headerfield, strlen(Sreponse->headerfield));
	message->buf[strlen(Sreponse->headerfield)]='\r';
	message->buf[strlen(Sreponse->headerfield)+1]='\n';
	memcpy(&(message->buf[strlen(Sreponse->headerfield)+2]),Sreponse->messagebody, strlen(Sreponse->messagebody));	
	
	free(Sreponse->startline);
	free(Sreponse->headerfield);
	free(Sreponse->messagebody);
	free(Sreponse);
	
	return reponse;
}

void error (Sreponse* message, char* num, char* detail)
{
	message->startline = startline (num, detail);
	message->headerfield = addHeaderfield(message, "Connection: Close");
	message->headerfield = addHeaderfield(message, "Content-Type: text/html");
	message->messagebody = malloc(sizeof(char)*strlen(detail));
	memcpy(message->messagebody, detail, strlen(detail));
	//Determiner la taille du paquet

}
	