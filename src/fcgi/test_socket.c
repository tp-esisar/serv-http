
#include "socket.h"

int main (void){
	FCGI_BeginRequestRecord* begin = make_FCGI_BeginRequestRecord(1,FCGI_RESPONDER,0);
	int sock = creat_fcgi ("127.0.0.1", 9000);
	if(put_fcgi(sock, (FCGI_Record_generic*) begin) == -1) {
		printf("Erreur d'envoi\n");
		exit(-1);
	}
	FCGI_Record_generic* recu = get_fcgi (sock);
	if (recu == NULL) {
		printf("Erreur de reception\n"); 
		exit(-1);
	}
	printf("Version : %d\nType : %d\nRequestId : %dContentLength : %d\nPaddingLength : %d\n", recu->header.version, recu->header.type, recu->header.requestId,recu->header.contentLength, recu->header.paddingLength);
	int i;
	for (i=0; i< recu->header.contentLength + recu->header.paddingLength; i++)
		printf("%c", recu->dataAndPad[i]);

	free(recu);
	close(sock);	
}
