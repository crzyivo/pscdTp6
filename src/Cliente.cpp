//*****************************************************************
// File:   Cliente.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Ejemplo de cliente con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <csignal>
#include <fcntl.h>
#include "Socket.hpp"

using namespace std;
string puja = "";
const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres
int s = fcntl(0, F_GETFL, 0);
void pasar(int i, siginfo_t* a, void * p){
	fcntl(0, F_SETFL, s |  !O_NONBLOCK);
	fprintf(stderr,"\033[32mCliente no ha contestado\033[0m\n");
	puja = "p";
	
}


int main(int argc, char *argv[]) {
    
	// struct sigaction acti;
	// struct sigaction def;
    // acti.sa_sigaction = pasar;
    // acti.sa_flags =0;
    // sigemptyset(&acti.sa_mask);
	// sigaction(SIGALRM, &acti, &def);
	bool automatic = false;
	int maxOferta = 50;
	srand(time(0));
	const string MENS_FIN("END OF SERVICE");
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = "localhost";
	string SERVER_PORT = "32005";
		string dir;
		if(argc >1){	//Inicializa con Parametros
		for (int i = 1; i< argc; i++){
			if(*argv[i]++ == '-'){
				if(*argv[i] == 'p'){		//numero de vueltas
					if(*++argv[i] == '\0'){i++;/*saltar espacio en blanco*/}
					 SERVER_PORT = argv[i];
				}else if(*argv[i] == 'd'){	//mnumero surtidores
					if(*++argv[i] == '\0'){i++; /*saltar espacio en blanco*/}
					SERVER_ADDRESS = argv[i];
				}else if(*argv[i] == 'a'){	//Modo automatico
					automatic = true;
					maxOferta = rand() % 150;
				}else{
					cout << "Uso: [-p<puerto>] [-d<direccion>]\n";
					cout << "\t-p<puerto>: puerto del servidor\n";
					cout << "\t-d<direccion>: direccion servidor\n";
					exit(1);
				}
			}
		}
	}
	// cout<<"Introduce la direccion servidor: \n";
	// getline(cin,SERVER_ADDRESS);
	// cout<<"Introduce el numero de puerto: \n";
	// getline(cin,SERVER_PORT);
	string imagenes[7]={
		"http://i.imgur.com/eTuCPxM.jpg",
		"http://i.imgur.com/evzIQVF.jpg",
		"http://i.imgur.com/BiFkD83.jpg",
		"http://i.imgur.com/ApuihUx.jpg",
		"http://i.imgur.com/LUiR7W8.jpg",
		"http://i.imgur.com/5NljFvC.jpg",
		"http://i.imgur.com/EygIdZU.jpg"};
	
	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_ADDRESS,stoi(SERVER_PORT));
		// Conectamos con el servidor. Probamos varias conexiones
	const int MAX_ATTEMPS = 10;
	int count = 0;
	int socket_fd;
	do {
		// Conexión con el servidor
	socket_fd = socket.Connect();
		count++;
		
		// Si error --> esperamos 1 segundo para reconectar
		if(socket_fd == -1){
			this_thread::sleep_for(chrono::seconds(1));
		}
	} while(socket_fd == -1 && count < MAX_ATTEMPS);
	
	// Chequeamos si se ha realizado la conexión
	if(socket_fd == -1){
		return socket_fd;
	}		

	cout<<"He conectado\n";
	string nueva_puja="";
	int read_bytes;
	string numMensaje;
	read_bytes = socket.Recv(socket_fd,nueva_puja, MESSAGE_SIZE);
	int miID = stoi(nueva_puja);
	read_bytes = socket.Recv(socket_fd,nueva_puja, MESSAGE_SIZE);
	if(read_bytes == -1){
		cerr << "Error al recibir datos: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
	}else{
		numMensaje = strtok(strdup(nueva_puja.c_str()), ";");
	}
	
	cout << nueva_puja << endl;
	bool salirSubastas = false;
	bool aceptarSiguiente = true;
	while(!salirSubastas){
		
		if(!automatic){
			cout << "escriba a,p,sp o ss para:\na -> para aceptar puja\np -> pasar puja\nsp-> salir de la puja actual y esperar una nueva\nss -> finalizar subasta\n";
			cin >> puja;
			cout << "\033[32m" + puja + "\033[0m\n";
		}else if(!aceptarSiguiente && automatic){
			puja = "sp";
		}else if(automatic){
			int noPujar = rand() %100;
			if(noPujar < 60){
				puja = "a";
			}else if(noPujar == 99){
				puja = "sp";
			}else{
				puja = "p";
			}
		}
		if(puja == "p"){
			read_bytes = socket.Send(socket_fd, numMensaje + ";Paso\n");
		}else if(puja == "a"){
			read_bytes = socket.Send(socket_fd, numMensaje  + ";Acepto\n");
		}else if(puja == "ss"){
			read_bytes = socket.Send(socket_fd, numMensaje  + ";Salir de subasta\n");
			salirSubastas = true;
		}else if(puja == "sp"){	
			read_bytes = socket.Send(socket_fd, numMensaje  + ";Salir puja Actual\n");
		}else{//Tratar mensajes no entendidos
			cout<<"No te he entendido"<<endl;
			read_bytes = socket.Send(socket_fd, numMensaje  + ";Paso\n");
		}
		
		read_bytes = socket.Recv(socket_fd,nueva_puja,MESSAGE_SIZE); 
		numMensaje = strtok(strdup(nueva_puja.c_str()), ";");
		if(numMensaje == "0"){
			salirSubastas = true;
		}

		nueva_puja = strtok(NULL, ";");		//TRATAR MENSAJES DE ENTRADA DE LA SUBASTA
		cout << nueva_puja;
		string gan = strtok(strdup(nueva_puja.c_str()), " ");
		if(automatic && numMensaje != "0"){
			string seguir = strtok(strdup(nueva_puja.c_str()), " ");
			int pujadores;
			int pujaActual;
			if(seguir == "Hay"){
			//	mensajeOut = to_string(mSubas->nMensaje()) +";Hay " + to_string(numPujadores) + ". Quien ofrece: " +to_string(mSubas->pujaActual()) + "\n";
				sscanf(nueva_puja.c_str(), "Hay %d. Quien ofrece: %d\n", &pujadores, &pujaActual);
					if(pujaActual > maxOferta){
						aceptarSiguiente = false;
					}else{
						aceptarSiguiente = true;
					}
				
			}else if(seguir == "Pujador"){
				sscanf(nueva_puja.c_str(), "Pujador %d hizo la max oferta. Quien ofrece: %d\n", &pujadores, &pujaActual);
				if(pujadores == miID){ //no volver a pujar
					aceptarSiguiente = false;
					
				}else{ //volver a pujar o salir puja
					if(pujaActual > maxOferta){
						aceptarSiguiente = false;
					}else{
						aceptarSiguiente = true;
					}
				}
			}
		}
		if(gan == "Ganador" || "Usted" == gan){
			int ganador;
			if(gan == "Ganador"){
				gan = strtok(NULL, ":");
				gan = strtok(NULL, " ");
				cout << gan << endl;
				ganador = stoi(gan);
			}
			if(gan == "Usted" || ganador == miID){
				cout << "SOY EL GANADOR\n";
				//Enviar url
				//esperar ACK
				do{
					string url;	
					if(!automatic){
						cout << "Escriba la url valida de la imagen\n";
						cin >> url;
					}else{
						url = imagenes[rand()%7];
					}

					socket.Send(socket_fd,url);
					socket.Recv(socket_fd,nueva_puja,MESSAGE_SIZE); 
					cout << nueva_puja << endl;
					cout << "se ha enviado algo\n";
				}while(nueva_puja != "URL recibida\n");

			}
		}
		
		//Si hay ganador (Si soy yo o no)
		//Si no hay ganador
		//Si he salido de la puja
		//Si he salido de la subasta
		nueva_puja = strtok(strdup(nueva_puja.c_str()), " "); 
		cerr << "\033[34m" + nueva_puja + "\033[0m\n";
		if(nueva_puja != "Pujador" && nueva_puja != "Hay" && nueva_puja !="Saliendo"){
		//REVISAR GUARDA!!!!!!!!!!
		if(numMensaje != "0"){
				cout << "Estoy esperando nueva subasta\n";
				read_bytes = socket.Recv(socket_fd,nueva_puja,MESSAGE_SIZE); 
				cout << "\033[34m" + nueva_puja + "\033[0m\n";
				numMensaje = strtok(strdup(nueva_puja.c_str()), ";");
				nueva_puja = strtok(NULL, ";");
				if(numMensaje == "0"){
					salirSubastas = true;
				}
				if(read_bytes > 0){
					cout << nueva_puja;
				}
			}
		}
	}
	
	
	// Cerramos el socket
	int error_code = socket.Close(socket_fd);
	if(error_code == -1){
		cerr << "Error cerrando el socket: " << strerror(errno) << endl;
	}
	cout << "Gracias por asistir a nuestra subasta. Vuelva cuando quiera\n";
	
}
