/*
 * Autores: Lafuente, Víctor M.; Escuín González, Iván; Morón Vidal, Elena
 * NIP: 747325; 684146; 739324
 * Fecha: enero 2018
 */

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
    

	bool automatic = false;
	int maxOferta = 50;
	srand(time(0)+getpid());
	const string MENS_FIN("END OF SERVICE");
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = "localhost";
	string SERVER_PORT = "32005";
		string dir;
		if(argc >1){	//Inicializa con Parametros
		for (int i = 1; i< argc; i++){
			if(*argv[i]++ == '-'){
				if(*argv[i] == 'p'){		//Puerto del servidor
					if(*++argv[i] == '\0'){i++;/*saltar espacio en blanco*/}
					 SERVER_PORT = argv[i];
				}else if(*argv[i] == 'd'){	//direccion del servidor
					if(*++argv[i] == '\0'){i++; /*saltar espacio en blanco*/}
					SERVER_ADDRESS = argv[i];
				}else if(*argv[i] == 'a'){	//Modo automatico
					automatic = true;
					maxOferta += rand() % 150;
				}else{
					cout << "Uso: [-p<puerto>] [-d<direccion>] [-a]\n";
					cout << "\t-p<puerto>: puerto del servidor\n";
					cout << "\t-d<direccion>: direccion servidor\n";
                                        cout << "\t-a: flag de cliente automatico\n";
					exit(1);
				}
			}
		}
	}

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
	bool salirSubastas = false;
	bool aceptarSiguiente = true;
	cout<<"He conectado\n";
	string nueva_puja="";
	int read_bytes;
	string numMensaje;
	if((read_bytes = socket.Recv(socket_fd,nueva_puja, MESSAGE_SIZE))<0){
		cerr << "\033[31mError al recibir miID\033[0m\n";
		salirSubastas = true;
		socket.Close(socket_fd);
		exit(1);
	}
	int miID = stoi(nueva_puja);
	read_bytes = socket.Recv(socket_fd,nueva_puja, MESSAGE_SIZE); //comienzo 1ª subasta
	if(read_bytes == -1){
		cerr << "\033[31mError al recibir datos\033[0m\n";
		// Cerramos el/ socket
		socket.Close(socket_fd);
		bool salirSubastas = true;
		bool aceptarSiguiente = false;
		exit(1);
	}else{
		numMensaje = strtok(strdup(nueva_puja.c_str()), ";");
		cout << "\033[33m" + nueva_puja + "\033[0m";
	}
	

	while(!salirSubastas){
		
		if(!automatic && numMensaje != "0"){
			cout << "escriba a,p,sp o ss para:\na -> para aceptar puja\np -> pasar puja\nsp-> salir de la puja actual y esperar una nueva\nss -> finalizar subasta\n";
			cin >> puja;
		}else if(!aceptarSiguiente && automatic){
			puja = "p";
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
		string aout;
		if(puja == "p"){
				aout = numMensaje + ";Paso\n";
		}else if(puja == "a"){
				aout = numMensaje  + ";Acepto\n";
		}else if(puja == "ss"){
			aout = numMensaje  + ";Salir de subasta\n";
			salirSubastas = true;
		}else if(puja == "sp"){	
			aout = numMensaje  + ";Salir puja Actual\n";
		}else if(numMensaje != "0"){//Tratar mensajes no entendidos
			aout = numMensaje + ";Paso\n";
		}
		read_bytes = socket.Send(socket_fd, aout);
		if(automatic){
			cout << " " << aout;
		}
		if(read_bytes > 0 && numMensaje == "0"){
			salirSubastas = true;
		}else if(read_bytes >0){
			if((read_bytes = socket.Recv(socket_fd,nueva_puja,MESSAGE_SIZE)) >0){
				numMensaje = strtok(strdup(nueva_puja.c_str()), ";");
				nueva_puja = strtok(NULL, ";");		//TRATAR MENSAJES DE ENTRADA DE LA SUBASTA
				//Observar si existe ganador
				string gan = strtok(strdup(nueva_puja.c_str()), " ");
				if(automatic && numMensaje != "0"){
					string seguir = strtok(strdup(nueva_puja.c_str()), " ");
					int pujadores;
					int pujaActual;
					if(seguir == "Hay"){
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
						ganador = stoi(gan);
					}
					if(gan == "Usted" || ganador == miID){
						//Enviar url
						//esperar ACK
						do{
							string url;	
							if(!automatic){
								cout << "Ha ganado, Escriba la url valida de la imagen o r si la desea aleatoria\n";
								cin >> url;
								if(url == "r"){
									url = imagenes[rand()%7];
								}
							}else{
								url = imagenes[rand()%7];
							}

							if(read_bytes < 1 || (read_bytes = socket.Send(socket_fd,url))<0){
								cerr << "\033[31mError en el envio de una URL\033[0m\n";
								salirSubastas = true;
							}
							if(automatic){
								cout << url << endl;
							}
							if(read_bytes < 1 || (read_bytes =socket.Recv(socket_fd,nueva_puja,MESSAGE_SIZE))<0){
								cerr << "\033[31mError al recibir confimacion del envio de la URL\033[0m\n";
								salirSubastas = true;
								socket.Close(socket_fd);
								exit(1);
							}
							cout <<"\033[33m" + nueva_puja + "\033[0m" << endl;
						}while(nueva_puja != "URL recibida\n");
							
					}
				}
				
				//Si hay ganador (Si soy yo o no)
				//Si no hay ganador
				//Si he salido de la puja
				//Si he salido de la subasta
				nueva_puja = strtok(strdup(nueva_puja.c_str()), " "); 
				cout << "\033[34m" + nueva_puja + "\033[0m\n";
				if(nueva_puja != "Pujador" && nueva_puja != "Hay" && nueva_puja !="Saliendo"){
					if(numMensaje != "0"){
						cout << "Estoy esperando nueva subasta\n";
						if(read_bytes > 0 && socket.Recv(socket_fd,nueva_puja,MESSAGE_SIZE)>0){
							cout << "\033[34m" + nueva_puja + "\033[0m\n";
							if(read_bytes > 0){
								cout << nueva_puja;
								numMensaje = strtok(strdup(nueva_puja.c_str()), ";");
								if(numMensaje == "0"){
									nueva_puja = strtok(NULL, ";");
									salirSubastas = true;
								}else{
									aceptarSiguiente = true;
								}
							}
						}else{
							cerr << "\033[31mServidor no contesa correctamente. Cierre del sistema\033[0m\n";
							salirSubastas = true;
							socket.Close(socket_fd);
							exit(1);
						}
					}
				}
			}else{ //Error al recibir mensaje
				cerr << "\033[31mError al recibir mensaje del subastador\033[0m\n";
				salirSubastas = true;
				socket.Close(socket_fd);
				exit(1);
			}
		}else{ //Error al enviar mensaje
			cerr << "\033[31mError al enviar mensaje:\033[0m\n";
			salirSubastas = true;
			socket.Close(socket_fd);
			exit(1);
		}
	}
	
	
	// Cerramos el socket
	int error_code = socket.Close(socket_fd);
	if(error_code == -1){
		cerr << "Error cerrando el socket\n";
	}
	cout << "Gracias por asistir a nuestra subasta. Vuelva cuando quiera\n";
	
}
