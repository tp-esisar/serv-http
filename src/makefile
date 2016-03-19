
PROJECT=..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc

objects: parseur map gc StringL $(INC)/* main.c api_test.c
	$(MAKE) -C StringL objects
	$(MAKE) -C parseur objects
	$(MAKE) -C map objects
	gcc -c main.c -I$(INC) -o $(OBJ)/main.o
	gcc -c api_test.c -I$(INC) -o $(OBJ)/api_test.o
	

test: objects $(BIN)/mainTest.e
	$(MAKE) -C StringL test_stringL
	$(MAKE) -C parseur test_reader
	$(BIN)/mainTest.e

$(BIN)/mainTest.e: objects
	gcc -Igc/include -I$(INC) $(OBJ)/StringL.o $(OBJ)/parser.o $(OBJ)/main.o $(OBJ)/map.o $(OBJ)/api_test.o gc/libgc.a.ext -o $(BIN)/mainTest.e

clear:
	rm ../obj/*.o ../bin/*.e