CFLAGS=-g -Wall -std=c99
PROJECT=../..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc


test_map: $(BIN)/test_map.e
	$(BIN)/test_map.e

$(BIN)/test_map.e: $(OBJ)/test_map.o $(OBJ)/map.o 
	gcc $^ -o $@
	

$(OBJ)/map.o: map.c $(INC)/*
	gcc -c map.c -o $(OBJ)/map.o $(CFLAGS) -I$(INC)

$(OBJ)/test_map.o: test_map.c $(INC)/*
	gcc -c test_map.c -o $(OBJ)/test_map.o $(CFLAGS) -I$(INC)


