
CFLAGS=-g -Wall -std=c99


test_stringL: test_stringL.o stringL.o
	gcc $^ -o test_stringL.e
	./test_stringL.e

test_reader: test_reader.o reader.o stringL.o
	gcc -Igc/include $^ gc/libgc.a.ext -o test_reader.e
	./test_reader.e

parseur.o: parseur.c parseur.h
	gcc -c parseur.c -o parseur.o $(CFLAGS)

stringL.o: stringL.c stringL.h
	gcc -c stringL.c -o stringL.o $(CFLAGS)
	
reader.o: reader.c reader.h 
	gcc $(CFLAGS) -Igc/include -c reader.c -o reader.o 
	
test_stringL.o: test_stringL.c stringL.h
	gcc -c test_stringL.c -o test_stringL.o $(CFLAGS)
	
clear:
	rm -rf *.o *.e