
PROJECT=..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc
LIB=$(PROJECT)/lib

DEBUG_FILE=$(PROJECT)/tests/get1
DEBUG_ITEM=method

$(BIN)/mainTest.e: objects
	gcc -g -Wall -I$(INC) $(OBJ)/StringL.o $(OBJ)/reader.o $(OBJ)/parser.o $(OBJ)/main.o $(OBJ)/map.o $(OBJ)/api_test.o $(OBJ)/fichier.o $(OBJ)/process.o $(OBJ)/reponse.o $(LIB)/libgc.a.ext -o $(BIN)/mainTest.e $(LIB)/librequest.so

objects: parseur map StringL $(INC)/* main.c api_test.c
	$(MAKE) -C StringL objects
	$(MAKE) -C parseur objects
	$(MAKE) -C map objects
	$(MAKE) -C extracteur objects
	#gcc -g -Wall -c main.c -I$(INC) -o $(OBJ)/main.o
	gcc -g -Wall -c api_test.c -I$(INC) -o $(OBJ)/api_test.o
	$(MAKE) -C reply objects
	gcc -g -Wall -c main.c -I$(INC) -o $(OBJ)/main.o -lrequest
	

test: objects $(BIN)/mainTest.e
	$(MAKE) -C StringL test_stringL
	$(MAKE) -C extracteur test_mini-map
	$(MAKE) -C parseur test_reader
	$(MAKE) -C extracteur test_header-getter
	
	./mainTest.sh

debug_mainTest: objects $(BIN)/mainTest.e
	gdb $(BIN)/mainTest.e -ex "break $(BP)" -ex "run $(DEBUG_FILE) $(DEBUG_ITEM)"

clear:
	rm ../obj/*.o ../bin/*.e
	
