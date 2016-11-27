compile: compile.c parser.o lexer.o error.o
	gcc compile.c parser.o lexer.o error.o -o compile
	
parser.o: parser.c parser.h lexer.o error.o
	gcc -c parser.c

lexer.o: lexer.c lexer.h 
	gcc -c lexer.c
	
error.o: error.c error.h
	gcc -c error.c

clean:
	rm *.o compile