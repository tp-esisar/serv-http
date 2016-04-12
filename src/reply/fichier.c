#include "fichier.h"

void accessFile (Sreponse* reponse, char *chemin)
{
    FILE* file = NULL;
    int i=0, j=0;
	char ext[6];

    file = fopen(chemin, "r");
    if(file == NULL) {
		error(reponse, "404", "Page inexistante");
		return;
	}

	while (fgetc(file) != EOF)
        i++;
    reponse->messagebody = malloc(i*sizeof(char));
    if(reponse->messagebody == NULL) {
        error(reponse, "500", "Erreur interne");
		return;
	}

    i=0; rewind(file);
    do
    {
        reponse->messagebody[i]=fgetc(file);
    } while (reponse->messagebody[i++] != EOF);

    fclose (file);
	
	//Ajouter la taille du contenu
	
	i--;
	while(j<6 && chemin[i] != '.')
		ext[j++]= chemin[i--];
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
