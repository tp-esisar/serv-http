
PROJECT=../..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc
LIB=$(PROJECT)/lib
CFLAGS=-g -Wall -std=c99 -I$(INC)

objects: $(OBJ)/StringL.o

test_stringL: $(BIN)/test_stringL.e
	$(BIN)/test_stringL.e

$(BIN)/test_stringL.e: $(OBJ)/test_stringL.o $(OBJ)/StringL.o 
	gcc $^ -o $@
	

$(OBJ)/StringL.o: StringL.c $(INC)/*
	gcc -c StringL.c -o $(OBJ)/StringL.o $(CFLAGS)

$(OBJ)/test_stringL.o: test_stringL.c $(INC)/*
	gcc -c test_stringL.c -o $(OBJ)/test_stringL.o $(CFLAGS)


