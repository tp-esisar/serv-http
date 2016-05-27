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
	reponse->messagebody.s = NULL;
	reponse->messagebody.len = 0;

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
		taille = strlen(ajout);
		reponse->headerfield = malloc(sizeof(char)*(taille+3));
		reponse->headerfield[0] = '\0';
	}
	else {
		taille = strlen(reponse->headerfield)+strlen(ajout);
		reponse->headerfield = realloc(reponse->headerfield, sizeof(char)*(taille+3));	
	}

	if (reponse->headerfield == NULL)
	{
		perror ("Erreur d'allocation mémoire");
		exit(1);
	}
	
	memcpy(&(reponse->headerfield[strlen(reponse->headerfield)]), ajout, strlen(ajout)+1);
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
	//size_t youhou = (strlen(Sreponse->startline)+strlen(Sreponse->headerfield)+Sreponse->messagebody.len+2);
	//reponse->buf = malloc(sizeof(char)*24800);
	reponse->buf = malloc(sizeof(char)*(strlen(Sreponse->startline)+strlen(Sreponse->headerfield)+Sreponse->messagebody.len+2));
	if (reponse->buf == NULL)
	{
		perror ("Erreur d'allocation mémoire");
		exit(1);
	}
	
	reponse->len = strlen(Sreponse->startline)+strlen(Sreponse->headerfield)+Sreponse->messagebody.len+2;
	
	memcpy(reponse->buf,Sreponse->startline, strlen(Sreponse->startline));
	memcpy(&(reponse->buf[strlen(Sreponse->startline)]),Sreponse->headerfield, strlen(Sreponse->headerfield));
	reponse->buf[strlen(Sreponse->startline)+strlen(Sreponse->headerfield)]='\r';
	reponse->buf[strlen(Sreponse->startline)+strlen(Sreponse->headerfield)+1]='\n';	
	memcpy(&(reponse->buf[strlen(Sreponse->startline)+strlen(Sreponse->headerfield)+2]),Sreponse->messagebody.s, Sreponse->messagebody.len);	
	
	free(Sreponse->startline);
	free(Sreponse->headerfield);
	free(Sreponse->messagebody.s);
	free(Sreponse);
	
	return reponse;
}

void error (Sreponse* message, char* num, char* detail)
{
	char header_size[30];
	if (message->startline != NULL);
		free(message->startline);
	message->startline = startline (num, detail);
	if (strcmp(num, "200") != 0) {
		message->messagebody.s = malloc(strlen(detail)*sizeof(char)+1);
		message->messagebody.len = strlen(detail);
		if (message->messagebody.s == NULL)
		{
			perror ("Erreur d'allocation mémoire");
			exit(1);
		}
		strcpy(message->messagebody.s, detail);
		snprintf (header_size, 30, "Content-Length: %ld", strlen(detail));
		addHeaderfield(message, header_size);
	}
}
	
