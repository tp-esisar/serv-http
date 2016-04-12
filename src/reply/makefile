PROJECT=../..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc
CFLAGS=-g -Wall -std=c99 -I$(INC)

objects: $(OBJ)/fichier.o $(OBJ)/process.o $(OBJ)/reponse.o

$(OBJ)/fichier.o: fichier.c $(INC)/*
	gcc $(CFLAGS) -c fichier.c -o $(OBJ)/fichier.o
	
$(OBJ)/process.o: process.c $(INC)/*
	gcc $(CFLAGS) -c process.c -o $(OBJ)/process.o -lrequest
	
$(OBJ)/reponse.o: reponse.c $(INC)/*
	gcc $(CFLAGS) -c reponse.c -o $(OBJ)/reponse.o -lrequest
	


