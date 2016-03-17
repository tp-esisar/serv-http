
PROJECT=../..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc
CFLAGS=-g -Wall -std=c99 -I$(INC)

all: $(OBJ)/reader.o $(OBJ)/parser.o

test_reader: $(BIN)/test_reader.e
	$(BIN)/test_reader.e

$(BIN)/test_reader.e: $(OBJ)/test_reader.o $(OBJ)/StringL.o $(OBJ)/reader.o
	gcc -Igc/include $^ gc/libgc.a.ext -o $@
	
$(OBJ)/reader.o: reader.c $(INC)/*
	gcc $(CFLAGS) -Igc/include -c reader.c -o $(OBJ)/reader.o
	

$(OBJ)/test_reader.o: test_reader.c $(INC)/*
	gcc $(CFLAGS) -c test_reader.c -o $(OBJ)/test_reader.o 

$(OBJ)/parser.o: parser.c $(INC)/*
	gcc $(CFLAGS) -c parser.c -o $(OBJ)/parser.o 

