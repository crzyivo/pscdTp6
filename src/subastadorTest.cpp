#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <thread>
#include <atomic>
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
//monitorSubasta mSubas;
const int maxNumCLientes = 35;
const int tiempoEntrePujas = 2000; //Tiempo que ha de esperar a que los interesados contesten
const int MAX_TIEMPO_VALLA = 30;
const int MIN_TIEMPO_VALLA = 2;

void controlSubasta(){
	srand(time(NULL));
	while(mSubas.SalonAbierto()){
		//NO EMPEZAR CON 0 Clientes
		this_thread :: sleep_for(chrono :: milliseconds(tiempoEntrePujas*10));
		mSubas.iniciarNuevaSubasta(rand()%MAX_TIEMPO_VALLA+MIN_TIEMPO_VALLA); 
		while(mSubas.numPujadores() > 0 && mSubas.SubastaEnCurso()){
			cerr << "\033[31m Esperar \033[0m\n";
			this_thread :: sleep_for(chrono :: milliseconds(tiempoEntrePujas));
			mSubas.enviarPuja();
			cerr << "\033[32m Todos Avisados \033[0m\n";
		}
	}
}


void subastaCliente(Socket *subasta, int cliente, monitorSubasta mSubas){
	bool enSubasta = true;
	const int maxMensaje = 1000;
	//Bucle de subastas
	//Cada iteración completa comprende desde el comienzo hasta su finalizacion de una subasta
	mSubas.anyadirPujador();
	while(enSubasta && mSubas.SalonAbierto()){
		//esperar a que se dé la condición de iniciar subasta
		int precioInicial = mSubas.comenzarSubasta();	//Bloqueante. devuelve precio de salida de subasta
		string mensajeIn = "";	//Mensajes desde cliente
		string mensajeOut = "Comienza la subasta en " + to_string(precioInicial) + " para un tiempo " + to_string(mSubas.tiempoSubas()) +"\n"; //mensajes para cliente
		bool seguirPuja = true;	//true = Cliente esta interesado en observar constantemente el estado de la subasta actual
		//Bucle de subasta
		//Cada iteracion corresponde al envio del precio actual, recibir y tratar mensajes de pujadores
		while(seguirPuja && mSubas.SubastaEnCurso()){
			cout << mensajeOut;
			subasta->Send(cliente, mensajeOut);
			subasta->Recv(cliente, mensajeIn, maxMensaje);
			cout << mensajeIn;
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
						mensajeOut = "Ganador de la puja " + to_string(mSubas.PujadorActual()) + "\n";
						cout << "Hay ganador\n";
					}else{
						mensajeOut = "Minimo no alcanzado\n";
					}
					subasta->Send(cliente, mensajeOut);
					seguirPuja = false;
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
	mSubas.quitarPujador();
}

void RunSubastador(int puertoSubasta){
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
		thread com(&controlSubasta);
		com.detach();
	while(mSubas.SalonAbierto()){
		//Abrir Salon
		int cliente = subasta.Accept();
		cout << "Cliente con fd " + to_string(cliente) + " ha entrado en subasta conectado\n";
		thread th(&subastaCliente, &subasta,cliente);
		th.detach();
		// esperar a que puedan entrar más clientes a Subasta

	}
	cout << "bye bye\n";
	mSubas.finSubasta();
	subasta.Close(sockSubasta);

	
	return 0;
}