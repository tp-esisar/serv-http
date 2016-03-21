
PROJECT=..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc

DEBUG_FILE=$(PROJECT)/tests/get1
DEBUG_ITEM=method

$(BIN)/mainTest.e: objects
	gcc -g -Wall -Igc/include -I$(INC) $(OBJ)/StringL.o $(OBJ)/reader.o $(OBJ)/parser.o $(OBJ)/main.o $(OBJ)/map.o $(OBJ)/api_test.o gc/libgc.a.ext -o $(BIN)/mainTest.e

objects: parseur map gc StringL $(INC)/* main.c api_test.c
	$(MAKE) -C StringL objects
	$(MAKE) -C parseur objects
	$(MAKE) -C map objects
	gcc -g -Wall -c main.c -I$(INC) -o $(OBJ)/main.o
	gcc -g -Wall -c api_test.c -I$(INC) -o $(OBJ)/api_test.o
	

test: objects $(BIN)/mainTest.e
	$(MAKE) -C StringL test_stringL
	$(MAKE) -C parseur test_reader
	./mainTest.sh

debug_mainTest: objects $(BIN)/mainTest.e
	gdb $(BIN)/mainTest.e -ex "break $(BP)" -ex "run $(DEBUG_FILE) $(DEBUG_ITEM)"

clear:
	rm ../obj/*.o ../bin/*.e
	
