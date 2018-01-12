
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

all: ${ADMIN} ${CLIENTE}

#Subastador
${SUBASTADOR}.o: ${SUBASTADOR}.cpp ${SOCKET}.o ${MONSUBASTA}.o ${ANUNCIO}.o ${MONVALLA}.o
	${CC} -c ${CPPFLAGS} ${SUBASTADOR}.cpp ${SOCKET}.o ${MONSUBASTA}.o ${ANUNCIO}.o ${MONVALLA}.o

${SOCKET}.o: ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp

${MONSUBASTA}.o: ${MONSUBASTA}.cpp
	${CC} -c ${CPPFLAGS} ${MONSUBASTA}.cpp
	
#Cliente

${CLIENTE}: ${CLIENTE}.cpp ${SOCKET}.o 
	${CC} ${CPPFLAGS} -o ${CLIENTE} ${CLIENTE}.cpp ${SOCKET}.o #${SOCKETSFLAGS}

#GestorValla

${GESTVALLAS}.o:${GESTVALLAS}.cpp ${ANUNCIO}.o ${VALLA}.o ${MONVALLA}.o ${imageDownloader}.o
	${CC} -c ${CPPFLAGS} ${GESTVALLAS}.cpp ${ANUNCIO}.o ${VALLA}.o ${MONVALLA}.o ${imageDownloader}.o

${ANUNCIO}.o: ${ANUNCIO}.hpp ${ANUNCIO}.cpp
	${CC} -c ${CPPFLAGS} ${ANUNCIO}.cpp

${VALLA}.o: ${VALLA}.hpp ${VALLA}.cpp
	${CC} -c ${CPPFLAGS} ${VALLA}.cpp 

${MONVALLA}.o:: ${MONVALLA}.hpp ${MONVALLA}.cpp
	${CC} -c ${CPPFLAGS} ${MONVALLA}.cpp

${imageDownloader}.o: ${imageDownloader}.hpp ${imageDownloader}.cpp
	${CC} -c ${CPPFLAGS} ${imageDownloader}.cpp
	
#Administrador

${ADMIN}: ${ADMIN}.o ${MONSUBASTA}.o ${MONVALLA}.o ${VALLA}.o ${ANUNCIO}.o ${SOCKET}.o ${imageDownloader}.o
	${CC} ${ADMIN}.o ${MONVALLA}.o ${VALLA}.o ${ANUNCIO}.o ${SOCKET}.o ${MONSUBASTA}.o ${imageDownloader}.o -o ${ADMIN} ${LDFLAGS} #${SOCKETSFLAGS}
	
${ADMIN}.o: ${ADMIN}.cpp ${SUBASTADOR}.cpp ${GESTVALLAS}.cpp
	${CC} -c ${CPPFLAGS} ${ADMIN}.cpp ${SUBASTADOR}.cpp ${GESTVALLAS}.cpp 
	
# LIMPIEZA
clean:
	${RM} *.o
	${RM} *.jpg