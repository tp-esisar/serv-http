#include "error.h"

int main (void)
{
	message* mess;
	int i = 0;

	mess = error ("420", "Message d'erreur");

	do {
		printf("%c", mess->buf[i++]);
	} while (i<mess->len);

        system("pause");
	return 0;
}
