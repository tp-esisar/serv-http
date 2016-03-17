PROJECT=../..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc
CFLAGS=-g -Wall -std=c99 -I$(INC)

objects: $(OBJ)/map.o

test_map: $(BIN)/test_map.e
	$(BIN)/test_map.e

$(BIN)/test_map.e: $(OBJ)/test_map.o $(OBJ)/map.o 
	gcc $^ -o $@
	

$(OBJ)/map.o: map.c $(INC)/*
	gcc $(CFLAGS) -c map.c -o $(OBJ)/map.o

$(OBJ)/test_map.o: test_map.c $(INC)/*
	gcc $(CFLAGS) -c test_map.c -o $(OBJ)/test_map.o


