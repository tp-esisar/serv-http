
PROJECT=..
OBJ=$(PROJECT)/obj
BIN=$(PROJECT)/bin
INC=$(PROJECT)/inc

objects: parseur map gc StringL $(INC)/*
	$(MAKE) -C StringL objects
	$(MAKE) -C parseur objects
	$(MAKE) -C map objects
	

test: objects
	$(MAKE) -C StringL test_stringL
	$(MAKE) -C parseur test_reader
	$(MAKE) -C map test_map

clear:
	rm ../obj/*.o ../bin/*.e