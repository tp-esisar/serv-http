#include "fichier.h"

void accessFile (Sreponse* reponse, char *chemin)
{
	FILE* file = NULL;
	char header_size[30];
	int i=0, j=0, size=0;
	char ext[6];

	file = fopen(chemin, "rb");
	if(file == NULL) {
		error(reponse, "404", "Page inexistante");
		return;
	}
	
	fseek (file , 0 , SEEK_END);
	size = ftell (file);
	rewind (file);

	reponse->messagebody = malloc (sizeof(char)*(size+1));
	if(reponse->messagebody == NULL) {
		error(reponse, "500", "Erreur interne");
		return;
	}

	if (fread (reponse->messagebody,1,size,file) != size) {
		error(reponse, "500", "Erreur interne");
		return;
	}
	reponse->messagebody[size]='\0';

	fclose (file);
	
	snprintf (header_size, 30, "Content-Length: %d", size);
	addHeaderfield(reponse, header_size);
	
	i=strlen(chemin)-1;
	while(chemin[i--] != '.');
	i+=2;
	while(j<6 && chemin[i] != '\0')
		ext[j++]= chemin[i++];
	ext[j] = '\0';
	
	if (strcmp(ext, "js") == 0)
		addHeaderfield(reponse, "Content-Type: application/javascript");
	else if (strcmp(ext, "pdf") == 0)
		addHeaderfield(reponse, "Content-Type: application/pdf");
	else if (strcmp(ext, "gif") == 0)
		addHeaderfield(reponse, "Content-Type: image/gif");
	else if (strcmp(ext, "jpg") == 0)
		addHeaderfield(reponse, "Content-Type: image/jpeg");
	else if (strcmp(ext, "png") == 0)
		addHeaderfield(reponse, "Content-Type: image/png");
	else if (strcmp(ext, "css") == 0)
		addHeaderfield(reponse, "Content-Type: text/css");
	else if (strcmp(ext, "html") == 0)
		addHeaderfield(reponse, "Content-Type: text/html");
	else 
		addHeaderfield(reponse, "Content-Type: application/octet-stream");
		
}
