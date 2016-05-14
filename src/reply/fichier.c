#include "fichier.h"

#define TAILLE_MAX 100

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length) {
    int i,j;
    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL) return NULL;

    for (i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}

int droit_acces (char *chemin, Authorization_HS* Authorization) {	
	FILE* file = NULL;
	char chaine[TAILLE_MAX];
	int taille;
	char* chemin_lock, encode;

	char* login = getLogin(Authorization);
	
	int i = strlen(chemin);
	while (chemin[--i] != '/');
	
	chemin_lock = malloc(sizeof(char)*(i+6));
	memcpy (chemin_lock, chemin, i);
	strcpy(&(chemin_lock[i]), ".lock"));

	file = fopen(chemin_lock, "r");
	if(file == NULL)
		return 0;
	
	fgets(chaine, TAILLE_MAX, file);
	fclose(file);
	free(chemin_lock);

	encode = base64_encode((unsigned char*)chaine, (size_t)strlen(chaine), &taille);

	if (strncmp(encode, login, taille) == 0) {
		free(encode);
		return 0;
	}
	free(encode);
	return -1;
}

void accessFile (Sreponse* reponse, char *chemin, Authorization_HS* Authorization)
{
	FILE* file = NULL;
	char header_size[30];
	int i=0, j=0;
	unsigned long int size=0;
	char ext[6];
	
	if (droit_acces(chemin, Authorization) == -1) {
		error(reponse, "401", "Non autorisé");
		addHeaderfield(reponse, "WWW-Authenticate: Basic realm=\"Espace privé\"");
		return;
	}

	file = fopen(chemin, "rb");
	if(file == NULL) {
		error(reponse, "404", "Page inexistante");
		return;
	}
	
	fseek (file , 0 , SEEK_END);
	size = ftell (file);
	rewind (file);

	reponse->messagebody.s = malloc (size*sizeof(char));
	reponse->messagebody.len = size;
	if(reponse->messagebody.s == NULL) {
		error(reponse, "500", "Erreur interne");
		return;
	}

	if (fread (reponse->messagebody.s,1,size,file) != size) {
		error(reponse, "500", "Erreur interne");
		return;
	}

	fclose (file);
	
	snprintf (header_size, 30, "Content-Length: %ld", size);
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
