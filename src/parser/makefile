

parser.o: parser.c parser.h
	gcc -c parser.c -o parser.o -g -Wall

stringL.o: stringL.c stringL.h
	gcc -c stringL.c -o stringL.o -g -Wall
	
test: test_stringL.o stringL.o
	gcc $^ -o test.e
	./test.e
	
test_stringL.o: test_stringL.c stringL.h
	gcc -c test_stringL.c -o test_stringL.o -g -Wall