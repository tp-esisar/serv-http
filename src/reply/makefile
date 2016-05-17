PROJECT=../..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc
LIB=$(PROJECT)/lib
CFLAGS=-g -Wall -std=c99 -I$(INC)

objects: $(OBJ)/fichier.o $(OBJ)/reponse.o $(OBJ)/process.o

$(OBJ)/reponse.o: reponse.c $(INC)/*
	gcc $(CFLAGS) -c reponse.c -o $(OBJ)/reponse.o -lrequest

$(OBJ)/fichier.o: fichier.c $(INC)/*
	gcc $(CFLAGS) -c fichier.c -o $(OBJ)/fichier.o
	
$(OBJ)/process.o: process.c $(INC)/*
	gcc $(CFLAGS) -c process.c -o $(OBJ)/process.o -lrequest
	
$(OBJ)/test_process.o: test_process.c $(INC)/*
	gcc $(CFLAGS) -c test_process.c -o $(OBJ)/test_process.o 
	
$(BIN)/test_process.e: $(OBJ)/test_process.o $(OBJ)/StringL.o $(OBJ)/process.o $(OBJ)/mini-map.o $(OBJ)/map.o $(OBJ)/header-getter.o $(OBJ)/reponse.o $(OBJ)/fichier.o $(OBJ)/reader.o $(LIB)/libcjson.a $(LIB)/libgc.a.ext
	gcc -I$(INC) $^ -o $@

test_process: $(BIN)/test_process.e
	$(BIN)/test_process.e

