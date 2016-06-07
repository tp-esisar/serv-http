#include "socket.h"
#include "fcgi.h"
#include "StringL.h"

int main (void){
	FCGI_BeginRequestRecord* begin;
	FCGI_Record_generic* stdin;
	char *param1, *param2, *param3, *param4;	
	
	param1 = malloc(35);	
	param2 = malloc(18);	
	param3 = malloc(24);	
	param4 = malloc(23);

	begin = malloc(sizeof(FCGI_BeginRequestRecord));
	stdin = malloc(sizeof(FCGI_Record_generic));
	
	begin->header = make_FCGI_Header(FCGI_BEGIN_REQUEST, 1, 8, 0);
	begin->body.role = 1;
	begin->body.flags = 0;

	param1 = "0d16DOCUMENT_ROOT/var/www/fcgi/groupe12";
	param2 = "0e04REQUEST_SCHEMEHTTP";
	param3 = "1107GATEWAY_INTERFACECGI/1.1";
	param4 = "0f08SERVER_PROTOCOLHTTP/1.1";

	stdin->header = make_FCGI_Header(FCGI_STDIN, 1, 0, 0);
	
	int sock = creat_fcgi("172.20.11.118", 9000);

	printf("\nconnexion\n");

	put_fcgi(sock, (FCGI_Record_generic*)begin);
	sendStreamChunk(sock, FCGI_PARAMS, 1, ((StringL){param1,35}));
	sendStreamChunk(sock, FCGI_PARAMS, 1, ((StringL){param2,18}));
	sendStreamChunk(sock, FCGI_PARAMS, 1, ((StringL){param3,24}));
	sendStreamChunk(sock, FCGI_PARAMS, 1, ((StringL){param4,23}));
	put_fcgi(sock, stdin);

	printf("\n\nenvoyé\n");

	FCGI_Record_generic* recu = get_fcgi (sock);
	printf("Version : %d\nType : %d\nRequestId : %dContentLength : %d\nPaddingLength : %d\n", recu->header.version, recu->header.type, recu->header.requestId,recu->header.contentLength, recu->header.paddingLength);
	int i;
	for (i=0; i< recu->header.contentLength + recu->header.paddingLength; i++)
		printf("%c", recu->dataAndPad[i]);

	free(recu);
	close(sock);	
}