#include "fichier.h"
#include "fcgi.h"


char* loadFile(FILE* file) {
	char* chaine;
	unsigned long int size=0;
	fseek (file , 0 , SEEK_END);
	size = ftell (file);
	rewind (file);
	chaine = malloc ((size+1)*sizeof(char));
	if(chaine == NULL) {
		fprintf(stderr, "erreur malloc loadFile");
		exit(EXIT_FAILURE);
	}

	if (fread (chaine,1,size,file) != size) {
		fprintf(stderr, "erreur fread loadFile");
		exit(EXIT_FAILURE);
	}
	chaine[size] = '\0';
	return chaine;
}

#define TAILLE_MAX 100


static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static int mod_table[] = {0, 2, 1};

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
	char* chemin_lock;
	StringL login;
	
	int i = strlen(chemin);
	while (chemin[i-1] != '/')
		i--;
	
	chemin_lock = malloc(sizeof(char)*(i+6));
	memcpy (chemin_lock, chemin, i);
	strcpy(&(chemin_lock[i]), ".lock");

	file = fopen(chemin_lock, "r");
	if(file == NULL) {
		free(chemin_lock);
		return 0;
	}
	
	fgets(chaine, TAILLE_MAX, file);
	fclose(file);
	free(chemin_lock);

	login.s = base64_encode((unsigned char*)chaine, (size_t)strlen(chaine)-1, (size_t*)&(login.len));

	if (Authorization != NULL && stringLEq(Authorization->auth_scheme, (StringL){"Basic", 5}) == 1 && stringLEq(Authorization->token68, login) == 1) {
		free(login.s);
		return 0;
	}
	free(login.s);
	return -1;
}

void accessFile (Sreponse* reponse, char *chemin, Authorization_HS* Authorization, mapStruct* map, cJSON* config_php, URI_Info uri_info)
{
	FILE* file = NULL;
	char header_size[30];
	int i=0, j=0;
	unsigned long int size=0;
	char ext[6];
	
	if (chemin == NULL){
		error(reponse, "400", "400 : Host ou Target errone");
		return;
	}
		
	
	if (droit_acces(chemin, Authorization) == -1) {
		error(reponse, "401", "401 : Non autorisé");
		addHeaderfield(reponse, "WWW-Authenticate: Basic realm=\"Espace privé\"");
		return;
	}

	file = fopen(chemin, "rb");
	if(file == NULL) {
		error(reponse, "404", "404 : Page inexistante");
		return;
	}
	
	i=strlen(chemin)-1;
	while(chemin[i--] != '.');
	i+=2;
	while(j<6 && chemin[i] != '\0')
		ext[j++]= chemin[i++];
	ext[j] = '\0';

	if (strcmp(ext, "php") == 0){
		if(php_request (reponse, chemin, map, config_php, (StringL){"", 0}, uri_info )==-1){
			error(reponse, "500", "500 : Erreur PHP");
			return;	
		}
		snprintf (header_size, 30, "Content-Length: %d", reponse->messagebody.len);
	}
	else {
		fseek (file , 0 , SEEK_END);
		size = ftell (file);
		rewind (file);

		reponse->messagebody.s = malloc (size*sizeof(char));
		reponse->messagebody.len = size;
		if(reponse->messagebody.s == NULL) {
			error(reponse, "500", "500 : Erreur interne");
			return;
		}

		if (fread (reponse->messagebody.s,1,size,file) != size) {
			error(reponse, "500", "500 : Erreur interne");
			return;
		}
		snprintf (header_size, 30, "Content-Length: %ld", size);

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
		else if (strcmp(ext, "html") == 0 || strcmp(ext, "php") == 0)
			addHeaderfield(reponse, "Content-Type: text/html");
		else 
			addHeaderfield(reponse, "Content-Type: application/octet-stream");
	}
	
	fclose (file);
	
	addHeaderfield(reponse, header_size);
		
}


void updateJsonObject(cJSON* obj, char* name, char* value) {
	if(cJSON_HasObjectItem(obj,name))
		cJSON_DeleteItemFromObject(obj,name);
	cJSON_AddStringToObject(obj,name,value);
}


int php_request (Sreponse* reponse, char *chemin, mapStruct* map, cJSON* config_php, StringL stdinbuf, URI_Info uri_info) {
	
	if(!cJSON_HasObjectItem(config_php,"param")) {
		fprintf(stderr,"erreur pas de champ param dans config_php.json\n");
		return -1;
	}
	cJSON* param = cJSON_GetObjectItem(config_php,"param");
	if(param->type != cJSON_Object) {
		fprintf(stderr,"erreur champ param invalide dans config_php.json\n");
		return -1;
	}


	///Ajout des attribus dans le config JSON !
	char* remoteDocRoot;
	char* localDocRoot;
	char* commonPath;
	char* remotePath;
	
	if(!cJSON_HasObjectItem(param,"DOCUMENT_ROOT")) {
		fprintf(stderr,"erreur DOCUMENT_ROOT manquant dans param de json\n");
		return -1;
	}
	remoteDocRoot = cJSON_GetObjectItem(param,"DOCUMENT_ROOT")->valuestring;
	if(!cJSON_HasObjectItem(config_php,"localRoot")) {
		fprintf(stderr,"erreur localRoot manquant dans json");
		return -1;
	}
	localDocRoot = cJSON_GetObjectItem(config_php,"localRoot")->valuestring;
	commonPath = &chemin[strlen(localDocRoot)];
	remotePath = malloc(strlen(remoteDocRoot) + strlen(commonPath) + 1);
	if(remotePath == NULL) {
		fprintf(stderr,"erreur malloc");
		return -1;
	}
	sprintf(remotePath,"%s%s",remoteDocRoot,commonPath);
	updateJsonObject(param, "SCRIPT_FILENAME",remotePath);
	free(remotePath);


	
	if (stringLEq (map->methode, (StringL){"GET", 3}) == 1) {
		updateJsonObject(param, "REQUEST_METHOD","GET");
		char* query = toRegularString(uri_info.query);
		updateJsonObject(param, "QUERY_STRING",query);
		free(query);
	}
	else if (stringLEq (map->methode, (StringL){"POST", 4}) == 1) {
		updateJsonObject(param, "REQUEST_METHOD","POST");
		Content_Length_HS* contentLength = get_Content_Length(map);
		if(contentLength == NULL) {
			fprintf(stderr,"erreur post, contentLength manquant");
			return -1;
		}
		char length[50];
		sprintf(length,"%ld",contentLength->Content_Length);
		updateJsonObject(param, "CONTENT_LENGTH", length);

		
		Content_Type_HS* contentType = get_Content_Type(map);
		if(contentType == NULL) {
			fprintf(stderr,"erreur post, contentType manquant");
			return -1;
		}
		if(contentType->next != NULL) {
			fprintf(stderr,"erreur post, plus d'1 contentType");
			return -1;
		}
		char* type = toRegularString(contentType->Content_Type);
		updateJsonObject(param, "CONTENT_TYPE",type);
		free(type);
		
	
	}
	

	if(!cJSON_HasObjectItem(config_php,"ip")) {
		fprintf(stderr,"erreur ip manquant dans json");
		return -1;
	}
	char* ip = cJSON_GetObjectItem(config_php,"ip")->valuestring;
	if(!cJSON_HasObjectItem(config_php,"port")) {
		fprintf(stderr,"erreur port manquant dans json");
		return -1;
	}
	int port = cJSON_GetObjectItem(config_php,"port")->valueint;
	
	AppResult result = FCGI_Request(stdinbuf, param,ip,port);
	printf("===> Envoi requete au serveur PHP \n");
	StringL stream = result.stdout;
	
	if (result.status == -1)
		return -1;
	
	printf("%.*s", result.stderr.len, result.stderr.s);
	//printf("%.*s", result.stdout.len, result.stdout.s);

	if (stream.len == 0)
		return -1;

	int i;
	int j=0;
	for(i=0; i<stream.len-4; i++) {
		if (stream.s[i]=='\r' && stream.s[i+1]=='\n' && stream.s[i+2]=='\r' && stream.s[i+3]=='\n') {
			stream.s[i] = '\0';
			addHeaderfield(reponse, &(stream.s[j]));
			stream.s[i]= '\r';
			i += 4;
			break;
		}
		else if (stream.s[i]=='\r' && stream.s[i+1]=='\n') {
			if (j==0 && (strncmp(stream.s, "Status", 6)==0) ){
				char num[3];
				char* detail = malloc(sizeof(char)*(i-11));
				memcpy(num, &(stream.s[8]), 3);
				memcpy(detail, &(stream.s[12]), i-12);
				detail[i-12] = '\0';
				reponse->startline=startline (num, detail);
				free(detail);
			}

			stream.s[i] = '\0';
			addHeaderfield(reponse, &(stream.s[j]));
			stream.s[i]= '\r';
			i=j=i+2;
		}
	}
	reponse->messagebody.s = malloc ((stream.len-i)*sizeof(char));
	reponse->messagebody.len = stream.len-i;
	memcpy(reponse->messagebody.s, &(stream.s[i]), reponse->messagebody.len);
	
	free(stream.s); 
	return 0;
}
