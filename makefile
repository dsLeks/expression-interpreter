all: token.o astnode.o lexer.o parser.o interpreter.o
	g++ main.cpp token.o astnode.o lexer.o parser.o interpreter.o -o main
token.o:
	g++ -c token.cpp -o token.o
astnode.o:
	g++ -c astnode.cpp -o astnode.o
lexer.o:
	g++ -c lexer.cpp -o lexer.o
parser.o:
	g++ -c parser.cpp -o parser.o
interpreter.o:
	g++ -c interpreter.cpp -o interpreter.o
clean:
	rm -rf *.o main