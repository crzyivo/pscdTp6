
all: subastador

subastador: subastador.cpp Socket.o mSubasta.o
	g++ -std=c++11 -pthread -o subastador subastador.cpp Socket.o mSubasta.o

Socket.o: Socket.cpp
	g++ -c -std=c++11 Socket.cpp

mSubasta.o: mSubasta.cpp
	g++ -c -std=c++11 mSubasta.cpp