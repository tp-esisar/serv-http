
#include "socket.h"

int main (void){
	FCGI_Record_generic* paquet;
	paquet = malloc(20);
	
	int sock = record_fcgi("172.20.11.118", 9000, paquet);
	FCGI_Record_generic* recu = get_fcgi (sock);
	printf("Version : %d\nType : %d\nRequestId : %dContentLength : %d\nPaddingLength : %d\n", recu->header.version, recu->header.type, recu->header.requestId,recu->header.contentLength, recu->header.paddingLength);
	int i;
	for (i=0; i< recu->header.contentLength + recu->header.paddingLength; i++)
		printf("%c", recu->dataAndPad[i]);

	free(recu);
	close(sock);	
}
