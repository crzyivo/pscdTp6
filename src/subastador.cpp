#include <iostream>
#include <string>
#include <list>
#include <thread>
#include "Socket.hpp"
#include "mSubasta.hpp"
/***********************
 * Mensajes Servidor Cliente:
 * "Comienza la subasta en %d\n"
 * "Puja cliente %d aceptada. Quien ofrece %d\n" 
 * "Puja no valida. Actual puja %d %d \n", cliente, puja
 * 
 * Mensajes Cliente Servidor
 *  "Acepto"
 *  "Subo %d"
 *  "Paso"
 *  "Salir puja Actual"
 *  "Salir de subasta"
 **********************/ 
monitorSubasta mSubas;
const int maxNumCLientes = 35;
const int tiempoEntrePujas = 3000; //Tiempo que ha de esperar a que los interesados contesten

int puertoSubasta = 32005;

void controlSubasta(){
	while(mSubas.SalonAbierto()){
		this_thread :: sleep_for(chrono :: milliseconds(tiempoEntrePujas));
		mSubas.enviarPuja();
	}

}


void subastaCliente(Socket *subasta, int cliente){
	bool enSubasta = true;
	const int maxMensaje = 1000;
	//Bucle de subastas
	//Cada iteración completa comprende desde el comienzo hasta su finalizacion de una subasta
	while(enSubasta && !mSubas.CerrarSalon()){
		//esperar a que se dé la condición de iniciar subasta
		int precioInicial = mSubas.comenzarSubasta();	//Bloqueante. devuelve precio de salida de subasta
		string mensajeIn = "";	//Mensajes desde cliente
		string mensajeOut = "Comienza la subasta en " + to_string(precioInicial) + "\n"; //mensajes para cliente
		bool seguirPuja = true;	//true = Cliente esta interesado en observar constantemente el estado de la subasta actual
		//Bucle de subasta
		//Cada iteracion corresponde al envio del precio actual, recibir y tratar mensajes de pujadores
		while(seguirPuja){
			subasta->Send(cliente, mensajeOut);
			subasta->Recv(cliente, mensajeIn, maxMensaje);
			
			//tratar mensajeIn
			if(mensajeIn == SaltarPujas){	//pujador no interesado en seguir subasta actual
				seguirPuja = false;
				subasta->Send(cliente, "Saliendo de subasta actual, esperando a que termine\n");
    		}else if(mensajeIn == SalirSubasta){	//pujador no interesado en ninguna subasta
				seguirPuja = false;
				enSubasta = false;
				subasta->Send(cliente, "Saliendo del salon de subastas. Hasta pronto\n");
    		}else{ //Cliente mantiene interes en la puja (haya aceptado o pasado)
				mSubas.pujar(mensajeIn, cliente);
				int numPujadores;
				switch(numPujadores = mSubas.nPujas()){	//Bloqueante. Devuelve nº de pujadores interesados
				case 0://Nadie ha pujado
					if(mSubas.SubastaAceptada()){
						mensajeOut = "Ganador de la puja" + to_string(mSubas.PujadorActual()) + "\n";
					}else{
						mensajeOut = "Minimo no alcanzado\n";
					}
					break;
				case 1://Solo hay una puja mas alta
					mensajeOut = "Pujador " + to_string(mSubas.PujadorActual()) + " hizo la max oferta. Quien ofrece: " + to_string(mSubas.pujaActual()) + "\n";
					break;
				default:// >1 pujador
					mensajeOut = "Hay " + to_string(numPujadores) + ". Quien ofrece: " +to_string(mSubas.pujaActual()+10) + "\n";
				}
			}
		}
	}
	int error = subasta->Close(cliente);
	if(error < 0){
		printf("error al desconectar cliente\n");
	}
}

int main(int argc, char * argv[]){
	string dir;
		if(argc >1){	//Inicializa con Parametros
		for (int i = 1; i< argc; i++){
			if(*argv[i]++ == '-'){
				if(*argv[i] == 'p'){		//numero de vueltas
					if(*++argv[i] == '\0'){i++;/*saltar espacio en blanco*/}
					puertoSubasta = atoi(argv[i]);
				}else if(*argv[i] == 'd'){	//mnumero surtidores
					if(*++argv[i] == '\0'){i++; /*saltar espacio en blanco*/}
					dir = argv[i];
				}else{
					cout << "Uso: [-p<puerto>] [-d<direccion>]\n";
					cout << "\t-p<puerto>: puerto del servidor\n";
					cout << "\t-d<direccion>: direccion servidor\n";
					exit(1);
				}
			}
		}
	}
//	puertoSubasta = 25000;
	Socket subasta(puertoSubasta);
        cout<<"Puerto de la subasta "<< puertoSubasta << endl;
	
	int sockSubasta = subasta.Bind();
        if (sockSubasta == -1) {
        cerr << "Error en el bind: " << strerror(errno) << endl;
        exit(1);
        }
	int error = subasta.Listen(maxNumCLientes);
        if(error == -1) {
        cerr << "Error en el listen: " << strerror(errno) << endl;
        // Cerramos el socket
        subasta.Close(sockSubasta);
        exit(1);
        }

	while(mSubas.SalonAbierto()){
		//Abrir Salon
		int cliente = subasta.Accept();
		thread th(&subastaCliente, &subasta,cliente);
		th.detach();
		// esperar a que puedan entrar más clientes a Subasta

	}
	mSubas.finSubasta();
	subasta.Close(sockSubasta);
	
	return 0;
}