PROJECT=../..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc
LIB=$(PROJECT)/lib
CFLAGS=-g -Wall -std=c99 -I$(INC)

objects: $(OBJ)/fcgi.o

test_fcgi: $(BIN)/test_fcgi.e
	$(BIN)/test_fcgi.e
	
$(BIN)/test_fcgi.e: $(OBJ)/test_fcgi.o $(OBJ)/fcgi.o 
	gcc $^ -o $@
	
$(OBJ)/fcgi.o: fcgi.c $(INC)/*
	gcc $(CFLAGS) -c fcgi.c -o $(OBJ)/fcgi.o

$(OBJ)/test_fcgi.o: test_fcgi.c $(INC)/*
	gcc $(CFLAGS) -c test_fcgi.c -o $(OBJ)/test_fcgi.o

