
PROJECT=..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc
LIB=$(PROJECT)/lib

DEBUG_FILE=$(PROJECT)/tests/get1
DEBUG_ITEM=method
MAKEPARAM=--no-print-directory -s -C

$(BIN)/mainTest.e: objects
	gcc -g -Wall -I$(INC) $(OBJ)/StringL.o $(OBJ)/reader.o $(OBJ)/parser.o $(OBJ)/main.o $(OBJ)/map.o $(OBJ)/api_test.o $(OBJ)/fichier.o $(OBJ)/process.o $(OBJ)/reponse.o $(LIB)/libgc.a.ext $(OBJ)/cJSON.o $(LIB)/librequest.so $(OBJ)/header-getter.o $(OBJ)/mini-map.o -o $(BIN)/mainTest.e -lm

objects: parseur map StringL $(INC)/* main.c api_test.c
	
	$(MAKE) $(MAKEPARAM) StringL objects
	$(MAKE) $(MAKEPARAM) parseur objects
	$(MAKE) $(MAKEPARAM) map objects
	$(MAKE) $(MAKEPARAM) extracteur objects
	#gcc -g -Wall -c main.c -I$(INC) -o $(OBJ)/main.o
	gcc -g -Wall -c api_test.c -I$(INC) -o $(OBJ)/api_test.o
	$(MAKE) $(MAKEPARAM) reply objects
	gcc -g -Wall -c main.c -I$(INC) -o $(OBJ)/main.o -lrequest
	

test: objects $(BIN)/mainTest.e
	
	$(MAKE) $(MAKEPARAM) StringL test_stringL
	$(MAKE) $(MAKEPARAM) extracteur test_mini-map
	$(MAKE) $(MAKEPARAM) parseur test_reader
	$(MAKE) $(MAKEPARAM) extracteur test_header-getter
	
	./mainTest.sh

debug_mainTest: objects $(BIN)/mainTest.e
	gdb $(BIN)/mainTest.e -ex "break $(BP)" -ex "run $(DEBUG_FILE) $(DEBUG_ITEM)"

clear:
	rm ../obj/*.o ../bin/*.e
	
