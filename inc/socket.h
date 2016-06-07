#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "fcgi.h"

int creat_fcgi (char* adresse, int port);
int put_fcgi (int sock, FCGI_Record_generic* message);
FCGI_Record_generic* get_fcgi (int sock); 
//Ne pas oublier de free et de fermer la connection !!!

#endif //SOCKET_H
