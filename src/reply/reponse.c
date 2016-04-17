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

	return reponse;
}

char* startline (char* num, char* detail)
{
	int taille = 8+1+3+1+strlen(detail)+2;

	char* line = malloc((taille+1)*sizeof(char));

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

void addHeaderfield(Sreponse* reponse, char* ajout)
{
	int taille = 0;

	if (reponse->headerfield == NULL) {
		reponse->headerfield = malloc(sizeof(char)*(strlen(ajout)+3));
		reponse->headerfield[0] = '\0';
	}
	else
		reponse->headerfield = realloc(reponse->headerfield, sizeof(char)*(strlen(reponse->headerfield)+strlen(ajout)+2));
	if (reponse->headerfield == NULL)
	{
		perror ("Erreur d'allocation mémoire");
		exit(1);
	}
	
	memcpy(&(reponse->headerfield[strlen(reponse->headerfield)]), ajout, strlen(ajout)+1);
	taille = strlen(reponse->headerfield);
	reponse->headerfield[taille] = '\r';
	reponse->headerfield[taille+1] = '\n';
	reponse->headerfield[taille+2] = '\0';	
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
	
	reponse->len = strlen(Sreponse->startline)+strlen(Sreponse->headerfield)+strlen(Sreponse->messagebody)+2;
	
	memcpy(reponse->buf,Sreponse->startline, strlen(Sreponse->startline));
	memcpy(&(reponse->buf[strlen(Sreponse->startline)]),Sreponse->headerfield, strlen(Sreponse->headerfield));
	reponse->buf[strlen(Sreponse->startline)+strlen(Sreponse->headerfield)]='\r';
	reponse->buf[strlen(Sreponse->startline)+strlen(Sreponse->headerfield)+1]='\n';
	memcpy(&(reponse->buf[strlen(Sreponse->startline)+strlen(Sreponse->headerfield)+2]),Sreponse->messagebody, strlen(Sreponse->messagebody));	
	
	free(Sreponse->startline);
	free(Sreponse->headerfield);
	free(Sreponse->messagebody);
	free(Sreponse);
	
	return reponse;
}

void error (Sreponse* message, char* num, char* detail)
{
	message->startline = startline (num, detail);
	addHeaderfield(message, "Connection: Close");
	addHeaderfield(message, "Content-Type: text/html");
	message->messagebody = malloc(sizeof(char)*strlen(detail));
	memcpy(message->messagebody, detail, strlen(detail));
	//Determiner la taille du paquet

}
	
