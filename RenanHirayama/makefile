all:
	make compile run clear

main.o: main.cpp header.hpp
	g++ -std=gnu++11 -c main.cpp

compile: main.o
	g++ main.o -o output.o
run: output.o
	./output.o

clear:
	rm *.o
reset:
	rm *.o *.dat
