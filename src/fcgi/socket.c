#include "socket.h"

int creat_fcgi (char* adresse, int port) { 
	
	int sock;
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

	return sock;
}

int put_fcgi (int sock, FCGI_Record_generic* message) { 
	
	int taille = 8 + message->header.contentLength + message->header.paddingLength;

	message->header.requestId = htons(message->header.requestId);
	message->header.contentLength = htons(message->header.contentLength);

	if(send(sock, message, taille, 0) != taille) {
		close(sock);
		return -1;
	}

	return 0;
}

FCGI_Record_generic* get_fcgi (int sock) {

	int taille_header, taille_body;
	FCGI_Header header;
	FCGI_Record_generic* record = NULL; 

	taille_header = recv(sock, &header, sizeof(header), 0);
	if (taille_header != sizeof(header)) {
		printf("erreur reception header\n");
		return(NULL);
	}

	header.requestId = ntohs(header.requestId);
	header.contentLength = ntohs(header.contentLength);

	int taille = header.contentLength + header.paddingLength;

	record = malloc(8+taille);
	if (record == NULL){
		printf("erreur allocation record\n");
		return(NULL);
	}

	memcpy(record, &header, sizeof(FCGI_Header));

	taille_body = recv(sock, record->dataAndPad, taille, 0);
	if (taille != taille_body) {
		printf("erreur reception body\n");
		return(NULL);
	}

	return record;
}

