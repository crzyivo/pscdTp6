#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   noviembre 2015
# Coms:   Genera sockets, cliente y servidor para la práctica 5
#         Ejecutar como "make"
#         Para compilar en Hendrix (Solaris OS), descomentar
#         "#${SOCKETSFLAGS}"
#*****************************************************************

# ######## #
# COMANDOS #
# ######## #
RM = rm -f # Comando de limpieza
CC = g++ # Comando de compilacion

# ######################## # 
# VARIABLES GLOBALES       #
# Definimos:               #
#   * Librería de sockets  # 
#   * Cliente              #
#   * Servidor             #
#   * ServidorMulticliente #
# ######################## #
SOCKET=Socket
CLIENT=Cliente
SERVER=Servidor
MULTISERVER=ServidorMulticlienteValla
SER = Servicio
CIMG = CImg.h
imageDownloader=ImageDownloader

# #################### #
# FLAGS DE COMPILACION #
# #################### #
CPPFLAGS=-I. -I/usr/local/include -O2 -std=c++11
LDFLAGS=-L/usr/X11R6/lib -L/usr/local/lib -lm -pthread -lcurl -lX11  
SOCKETSFLAGS=-lsocket -lnsl # Flags linkado sockets (Solaris SunOS)

.PHONY:all

all: ${CLIENT} ${MULTISERVER}

# SOCKETS
# Compilacion libreria de Sockets
${SOCKET}.o: ${SOCKET}.h ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp
#-----------------------------------------------------------
# CLIENTE
# Compilacion
${CLIENT}.o: ${CLIENT}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENT}.cpp

# Linkado
${CLIENT}: ${SOCKET}.o ${CLIENT}.o  
	${CC} ${LDFLAGS} ${SOCKET}.o ${CLIENT}.o -o ${CLIENT} #${SOCKETSFLAGS} #descomentar para Hendrix
#-----------------------------------------------------------
# SERVIDOR
# Compilacion
${MULTISERVER}.o: ${MULTISERVER}.cpp CierreServidor.h Imagen.h coutMonitor.h ${CIMG}
	${CC} -c ${CPPFLAGS} ${MULTISERVER}.cpp
${SER}.o: ${SER}.h ${SER}.cpp Imagen.h
	${CC} -c ${SER}.cpp ${CPPFLAGS}
	
${imageDownloader}.o: ${imageDownloader}.h ${imageDownloader}.cpp
	${CC} -c ${CPPFLAGS} ${imageDownloader}.cpp
# Linkado
${MULTISERVER}: ${SOCKET}.o ${MULTISERVER}.o ${SER}.o ${imageDownloader}.o
	${CC} ${SOCKET}.o ${MULTISERVER}.o ${SER}.o ${imageDownloader}.o -o ${MULTISERVER} ${LDFLAGS} 
#-----------------------------------------------------------

# LIMPIEZA
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${CLIENT} ${CLIENT}.o
	$(RM) ${SERVER} ${SERVER}.o
	$(RM) ${MULTISERVER} ${MULTISERVER}.o
	$(RM) ${imageDownloader}.o
	$(RM) ${SER}.o

