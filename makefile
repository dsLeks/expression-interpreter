all: token.o astnode.o lexer.o parser.o interpreter.o
	g++ -std=c++11 main.cpp token.o astnode.o lexer.o parser.o interpreter.o -o main
token.o:
	g++ -std=c++11 -c token.cpp -o token.o
astnode.o:
	g++ -std=c++11 -c astnode.cpp -o astnode.o
lexer.o:
	g++ -std=c++11 -c lexer.cpp -o lexer.o
parser.o:
	g++ -std=c++11 -c parser.cpp -o parser.o
interpreter.o:
	g++ -std=c++11 -c interpreter.cpp -o interpreter.o
clean:
	rm -rf *.o main