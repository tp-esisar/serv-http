#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <stdio.h>
#include "string.h"

typedef struct {
	char *buf;				/**< buf Un pointeur vers le message recu */
	unsigned int len;			/**< len La longueur du message recu */
	unsigned int clientId;			/**< clientId identifiant du client, recu dans une requete, doit etre recopier dans la reponse correspondante*/
	struct sockaddr_in *clientAddress; 	/**< iclientAddress pointeur vers une structure permettant de recuperer l'adresse IP et le port du client HTTP */
} message;

message* error (char* num, char* detail);

#endif // ERROR_H
