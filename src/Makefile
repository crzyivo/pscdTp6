
all: subastador cliente gestor

#Subastador
subastador: subastador.cpp Socket.o mSubasta.o
	g++ -std=c++11 -pthread -o subastador subastador.cpp Socket.o mSubasta.o

Socket.o: Socket.cpp
	g++ -c -std=c++11 Socket.cpp

mSubasta.o: mSubasta.cpp
	g++ -c -std=c++11 mSubasta.cpp
	
#Cliente

cliente: Cliente.cpp Socket.o 
	g++ -std=c++11 -pthread -o cliente Cliente.cpp Socket.o 

#GestorValla

gestor: gestor.o anuncio.o valla.o mValla.o imageDownloader.o
	g++ anuncio.o valla.o mValla.o imageDownloader.o gestor.o -o gestor -L/usr/X11R6/lib -L/usr/local/lib -lm -pthread -lcurl -lX11

gestor.o:gestorValla.cpp anuncio.o valla.o mValla.o imageDownloader.o
	g++ -c -I. -I/usr/local/include -O2 -std=c++11 -fmax-errors=1 -Werror gestorValla.cpp anuncio.o valla.o mValla.o imageDownloader.o

anuncio.o: anuncio.hpp anuncio.cpp
	g++ -c -std=c++11 anuncio.cpp

valla.o: valla.cpp anuncio.o
	g++ -c -std=c++11 valla.cpp anuncio.o

mValla.o:: mValla.cpp valla.o anuncio.o
	g++ -c -std=c++11 mValla.cpp valla.o anuncio.o

imageDownloader.o: ImageDownloader.hpp ImageDownloader.cpp
	g++ -c -I. -I/usr/local/include -O2 -std=c++11 -fmax-errors=1 -Werror ImageDownloader.cpp