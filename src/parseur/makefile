
PROJECT=../..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc
LIB=$(PROJECT)/lib
CFLAGS=-g -Wall -std=c99 -I$(INC)

objects: $(OBJ)/reader.o $(OBJ)/parser.o

test_reader: $(BIN)/test_reader.e
	$(BIN)/test_reader.e

$(BIN)/test_reader.e: $(OBJ)/test_reader.o $(OBJ)/StringL.o $(OBJ)/reader.o $(LIB)/libgc.a.ext
	gcc $^ -o $@
	
$(OBJ)/reader.o: reader.c $(INC)/* base_readers.c
	gcc $(CFLAGS) -c reader.c -o $(OBJ)/reader.o
	

$(OBJ)/test_reader.o: test_reader.c $(INC)/*
	gcc $(CFLAGS) -c test_reader.c -o $(OBJ)/test_reader.o 

$(OBJ)/parser.o: parser.c $(INC)/*
	gcc $(CFLAGS) -c parser.c -o $(OBJ)/parser.o 
