all: compile link

compile:
	g++ -c main.cpp

link:
	g++ main.o -o main -lsfml-window -lsfml-system -lsfml-graphics -lmingw32