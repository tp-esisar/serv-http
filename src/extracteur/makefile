PROJECT=../..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc
CFLAGS=-g -Wall -std=c99 -I$(INC)

objects: $(OBJ)/header-getter.o $(OBJ)/mini-map.o $(OBJ)/StringL.o

test_header-getter: $(BIN)/test_header-getter.e
	$(BIN)/test_header-getter.e
	
test_mini-map: $(BIN)/test_mini-map.e
	$(BIN)/test_mini-map.e
	
debug_mini-map: $(BIN)/test_mini-map.e
	gdb $(BIN)/test_mini-map.e

$(BIN)/test_header-getter.e: $(OBJ)/test_header-getter.o $(OBJ)/header-getter.o $(OBJ)/map.o $(OBJ)/StringL.o $(OBJ)/parser.o $(OBJ)/reader.o ../gc/libgc.a.ext
	gcc -I../gc/include -I$(INC) $^ -o $@

$(BIN)/test_mini-map.e: $(OBJ)/test_mini-map.o $(OBJ)/mini-map.o $(OBJ)/map.o $(OBJ)/StringL.o $(OBJ)/parser.o $(OBJ)/reader.o ../gc/libgc.a.ext
	gcc -I../gc/include -I$(INC) $^ -o $@
	

$(OBJ)/header-getter.o: header-getter.c $(INC)/*
	gcc $(CFLAGS) -c header-getter.c -o $(OBJ)/header-getter.o

$(OBJ)/test_header-getter.o: test_header-getter.c $(INC)/*
	gcc $(CFLAGS) -c test_header-getter.c -o $(OBJ)/test_header-getter.o

$(OBJ)/mini-map.o: mini-map.c $(INC)/*
	gcc $(CFLAGS) -c mini-map.c -o $(OBJ)/mini-map.o

$(OBJ)/test_mini-map.o: test_mini-map.c $(INC)/*
	gcc $(CFLAGS) -c test_mini-map.c -o $(OBJ)/test_mini-map.o


