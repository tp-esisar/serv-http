#include "socket.h"

int record_fcgi (char* adresse, int port, FCGI_Record_generic* message) { 
	
	int sock;
	int taille = 8 + message->header.contentLength + message->header.paddingLength;
	struct sockaddr_in serv_addr;
	struct in_addr adresse1;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) 
		return -1;

	bzero((char *) &serv_addr, sizeof(serv_addr));
	adresse1.s_addr=inet_addr(adresse);
	serv_addr.sin_addr = adresse1;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_family = AF_INET;

	if(connect(sock,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1)  {
		close(sock);
		return -1;
	}

	if(send(sock, message, taille, 0) != taille) {
		close(sock);
		return -1;
	}
	printf("===> Envoi requete au serveur PHP \n");

	return sock;
}

FCGI_Record_generic* get_fcgi (int sock) {

	int taille_header, taille_body;
	FCGI_Header header;
	FCGI_Record_generic* record = NULL; 

	taille_header = recv(sock, &header, sizeof(header)-1, 0);
	if (taille_header != sizeof(header)) {
		perror("erreur reception header");
		exit(-1);
	}

	int taille = header.contentLength + header.paddingLength;

	record = malloc(8+taille);
	if (record == NULL){
		perror("erreur allocation record");
		exit(-1);
	}

	memcpy(record, &header, sizeof(FCGI_Header));

	taille_body = recv(sock, record->dataAndPad, taille-1, 0);
	if (taille != taille_body) {
		perror("erreur reception body");
		exit(-1);
	}

	return record;
}

