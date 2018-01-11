

# #################### #
# FLAGS DE COMPILACION #
# #################### #
CPPFLAGS=-I. -I/usr/local/include -O2 -std=c++11
LDFLAGS=-L/usr/X11R6/lib -L/usr/local/lib -lm -pthread -lcurl -lX11  
SOCKETSFLAGS=-lsocket -lnsl # Flags linkado sockets (Solaris SunOS)

all: subastador cliente

#Subastador
subastador: subastador.cpp Socket.o mSubasta.o anuncio.o mValla.o
	g++ -std=c++11 -pthread -o subastador subastador.cpp Socket.o mSubasta.o anuncio.o mValla.o
	
subastador.o: subastador.cpp Socket.o mSubasta.o anuncio.o mValla.o
	g++ -c -std=c++11 -pthread subastador.cpp Socket.o mSubasta.o anuncio.o mValla.o

Socket.o: Socket.cpp
	g++ -c -std=c++11 Socket.cpp

mSubasta.o: mSubasta.cpp
	g++ -c -std=c++11 mSubasta.cpp
	
#Cliente

cliente: Cliente.cpp Socket.o 
	g++ -std=c++11 -pthread -o cliente Cliente.cpp Socket.o 

#GestorValla

gestor: gestorValla.o anuncio.o valla.o mValla.o ImageDownloader.o
	g++ anuncio.o valla.o mValla.o ImageDownloader.o gestorValla.o -o gestor -L/usr/X11R6/lib -L/usr/local/lib -lm -pthread -lcurl -lX11

gestorValla.o:gestorValla.cpp anuncio.o valla.o mValla.o ImageDownloader.o
	g++ -c -pthread -I. -I/usr/local/include -O2 -std=c++11  -fmax-errors=1 -Werror gestorValla.cpp anuncio.o valla.o mValla.o ImageDownloader.o

anuncio.o: anuncio.hpp anuncio.cpp
	g++ -c -std=c++11 anuncio.cpp

valla.o: valla.cpp anuncio.o
	g++ -c -std=c++11 valla.cpp anuncio.o

mValla.o:: mValla.cpp valla.o anuncio.o
	g++ -c -std=c++11 mValla.cpp valla.o anuncio.o

ImageDownloader.o: ImageDownloader.hpp ImageDownloader.cpp
	g++ -c -I. -I/usr/local/include -O2 -std=c++11 -fmax-errors=1 -Werror ImageDownloader.cpp
	
#Administrador

administrador: administrador.o mSubasta.o mValla.o valla.o anuncio.o Socket.o ImageDownloader.o
	g++ administrador.o mValla.o valla.o anuncio.o Socket.o mSubasta.o ImageDownloader.o -o admin ${LDFLAGS} 
	
administrador.o: administrador.cpp mSubasta.o mValla.o valla.o anuncio.o Socket.o 
	g++ -c ${CPPFLAGS} administrador.cpp mSubasta.o mValla.o valla.o anuncio.o Socket.o 
	
# LIMPIEZA
clean:
	rm -f *.o