
# Autores: Lafuente, Víctor M.; Escuín González, Iván; Morón Vidal, Elena
# NIP: 747325; 684146; 739324
# Fecha: enero 2018

#Descomentar SOCKETFLAGS en Hendrix
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
SRCDIR=src/

SOCKET=Socket
CLIENTE=Cliente

ADMIN=administrador

SUBASTADOR=subastador
MONSUBASTA = mSubasta

GESTVALLAS=gestorValla
ANUNCIO=anuncio
VALLA=valla
MONVALLA=mValla
CIMG = CImg.h
imageDownloader=ImageDownloader

# #################### #
# FLAGS DE COMPILACION #
# #################### #
CPPFLAGS=-I./src -I/usr/local/include -O2 -std=c++11 -pthread
LDFLAGS=-L/usr/X11R6/lib -L/usr/local/lib -lm -pthread -lcurl -lX11  
SOCKETSFLAGS=-lsocket -lnsl # Flags linkado sockets (Solaris SunOS)

all: ${ADMIN} ${CLIENTE} mv

#Subastador
${SUBASTADOR}.o: ${SRCDIR}${SUBASTADOR}.cpp ${SOCKET}.o ${MONSUBASTA}.o ${ANUNCIO}.o ${MONVALLA}.o
	${CC} -c ${CPPFLAGS} ${SRCDIR}${SUBASTADOR}.cpp ${SOCKET}.o ${MONSUBASTA}.o ${ANUNCIO}.o ${MONVALLA}.o #${SOCKETSFLAGS}

${SOCKET}.o: ${SRCDIR} ${SRCDIR}${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SRCDIR}${SOCKET}.cpp

${MONSUBASTA}.o: ${SRCDIR}${MONSUBASTA}.cpp
	${CC} -c ${CPPFLAGS} ${SRCDIR}${MONSUBASTA}.cpp
	
#Cliente

${CLIENTE}: ${SRCDIR}${CLIENTE}.cpp ${SOCKET}.o 
	${CC} ${CPPFLAGS} -o ${CLIENTE} ${SRCDIR}${CLIENTE}.cpp ${SOCKET}.o #${SOCKETSFLAGS}

#GestorValla

${GESTVALLAS}.o:${SRCDIR}${GESTVALLAS}.cpp ${ANUNCIO}.o ${VALLA}.o ${MONVALLA}.o ${imageDownloader}.o
	${CC} -c ${CPPFLAGS} ${SRCDIR}${GESTVALLAS}.cpp ${ANUNCIO}.o ${VALLA}.o ${MONVALLA}.o ${imageDownloader}.o ${LDFLAGS}

${ANUNCIO}.o: ${SRCDIR}${ANUNCIO}.hpp ${SRCDIR}${ANUNCIO}.cpp
	${CC} -c ${CPPFLAGS} ${SRCDIR}${ANUNCIO}.cpp

${VALLA}.o: ${SRCDIR}${VALLA}.hpp ${SRCDIR}${VALLA}.cpp
	${CC} -c ${CPPFLAGS} ${SRCDIR}${VALLA}.cpp 

${MONVALLA}.o:: ${SRCDIR}${MONVALLA}.hpp ${SRCDIR}${MONVALLA}.cpp
	${CC} -c ${CPPFLAGS} ${SRCDIR}${MONVALLA}.cpp

${imageDownloader}.o: ${SRCDIR}${imageDownloader}.hpp ${SRCDIR}${imageDownloader}.cpp
	${CC} -c ${CPPFLAGS} ${SRCDIR}${imageDownloader}.cpp
	
#Administrador

${ADMIN}: ${ADMIN}.o ${MONSUBASTA}.o ${MONVALLA}.o ${VALLA}.o ${ANUNCIO}.o ${SOCKET}.o ${imageDownloader}.o
	${CC} ${ADMIN}.o ${MONVALLA}.o ${VALLA}.o ${ANUNCIO}.o ${SOCKET}.o ${MONSUBASTA}.o ${imageDownloader}.o -o ${ADMIN} ${LDFLAGS} #${SOCKETSFLAGS}
	
${ADMIN}.o: ${SRCDIR}${ADMIN}.cpp ${SRCDIR}${SUBASTADOR}.cpp ${SRCDIR}${GESTVALLAS}.cpp
	${CC} -c ${CPPFLAGS} ${SRCDIR}${ADMIN}.cpp ${SRCDIR}${SUBASTADOR}.cpp ${SRCDIR}${GESTVALLAS}.cpp 
	
mv:
	mv *.o ./bin
	mv ./${ADMIN} ./bin
	mv ./${CLIENTE} ./bin
	
# LIMPIEZA
clean:
	${RM} *.o
	${RM} *.jpg
	${RM} ./bin/*.o
	${RM} ./bin/*.jpg
	${RM} ./bin/${ADMIN}
	${RM} ./bin/${CLIENTE}