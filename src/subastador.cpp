#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <thread>
#include <atomic>
#include "anuncio.hpp"
#include "mValla.hpp"
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
const int tiempoEntrePujas = 10000; //Tiempo que ha de esperar a que los interesados contesten
const int MAX_TIEMPO_VALLA = 30;
const int MIN_TIEMPO_VALLA = 2;
int puertoSubasta = 32005;

void controlSubasta(monitorSubasta *mSubas){
	srand(time(NULL));
	while(mSubas->comenzarSubastas() && mSubas->SalonAbierto()){
		//NO EMPEZAR CON 0 Clientes
		cout << "\033[1;34mSalon abierto. Esperando a que se conecten todos los clientes interesados\033[0m\n";
		this_thread :: sleep_for(chrono :: milliseconds(tiempoEntrePujas));
		mSubas->iniciarNuevaSubasta(rand()%MAX_TIEMPO_VALLA+MIN_TIEMPO_VALLA); 
		while(mSubas->numPujadores() > 0 && mSubas->SubastaEnCurso()){
			cerr << "\033[31m Esperar \033[0m\n";
			this_thread :: sleep_for(chrono :: milliseconds(tiempoEntrePujas));
			mSubas->enviarPuja();
			cerr << "\033[32m Todos Avisados \033[0m\n";
		}
	}
}


void subastaCliente(Socket *subasta, int cliente , monitorSubasta * mSubas,MonitorValla *mV){
	bool enSubasta = true;
	const int maxMensaje = 1000;
	//Bucle de subastas
	//Cada iteración completa comprende desde el comienzo hasta su finalizacion de una subasta
	if(subasta->Send(cliente, to_string(cliente)+ "\n") <= 0){

	}else{
		mSubas->anyadirPujador();
		while(enSubasta && mSubas->SalonAbierto()){
			//esperar a que se dé la condición de iniciar subasta
			int precioInicial = mSubas->comenzarSubasta();	//Bloqueante. devuelve precio de salida de subasta
			string mensajeIn = "";	//Mensajes desde cliente
			int t = mSubas->tiempoSubas();
			string mensajeOut = to_string(mSubas->nMensaje()) +";Comienza la subasta en " + to_string(precioInicial) + " para un tiempo " + to_string(t) +"\n"; //mensajes para cliente
			bool seguirPuja = true;	//true = Cliente esta interesado en observar constantemente el estado de la subasta actual
			//Bucle de subasta
			//Cada iteracion corresponde al envio del precio actual, recibir y tratar mensajes de pujadores
			while(seguirPuja && mSubas->SubastaEnCurso()){
				cout << mensajeOut;
				if(subasta->Send(cliente, mensajeOut) >0){
					subasta->Recv(cliente, mensajeIn, maxMensaje);
					cout << mensajeIn;
					//tratar mensajeIn
					
					char * mensajesN = strtok( strdup(mensajeIn.c_str()), ";");//Separar numMensaje
					string mensajeChar = strtok( NULL, ";"); //Separar mensaje
					cerr << "\033[31m" + mensajeChar + "\033[0m";
					if(mSubas->numMenAceptado(atoi(mensajesN)) || mensajeChar == "Salir de subasta\n" ){
						if(mensajeChar == SaltarPujas){	//pujador no interesado en seguir subasta actual
							seguirPuja = false;
							if(subasta->Send(cliente, to_string(mSubas->nMensaje()) +";Subasta actual ignorada, esperando a que termine\n") <= 0){
								enSubasta = false;
							}
						}else if(mensajeChar == SalirSubasta){	//pujador no interesado en ninguna subasta
							seguirPuja = false;
							enSubasta = false;
							subasta->Send(cliente, to_string(mSubas->nMensaje()) +";Saliendo del salon de subastas. Hasta pronto\n");
						}else{ //Cliente mantiene interes en la puja (haya aceptado o pasado)
							mSubas->pujar(mensajeChar, cliente);
							cerr << "\033[33mSE ha pujado\033[0m\n";
							int numPujadores;
							switch(numPujadores = mSubas->nPujas()){	//Bloqueante. Devuelve nº de pujadores interesados
							case 0://Nadie ha pujado
								if(mSubas->SubastaAceptada()){
									mensajeOut = to_string(mSubas->nMensaje()) +";Ganador de la puja: " + to_string(mSubas->PujadorActual()) + "\n";
									cout << "Hay ganador\n";
									if(mSubas->PujadorActual() ==  cliente){
										subasta->Send(cliente, mensajeOut);
										subasta->Recv(cliente, mensajeIn, maxMensaje);
										string obUrl = strtok(strdup(mensajeIn.c_str()), ":");
										cout << mensajeIn << endl;
										while("http" != obUrl) {
											subasta->Send(cliente, "Usted ha ganado la puja, por favor, envia una url valida\n");
											subasta->Recv(cliente, mensajeIn, maxMensaje);
											obUrl = strtok(strdup(mensajeIn.c_str()), ":");
										}
											//subasta->Send(cliente, "URL recibida\n");
											mensajeOut = "URL recibida\n";
											Anuncio anun(mensajeIn.c_str(), t); 
											mV->encolar(anun);
											
										//Encolar URL
									}
								}else{
									mensajeOut = to_string(mSubas->nMensaje()) +";Minimo no alcanzado\n";
								}
								if(subasta->Send(cliente, mensajeOut) <= 0){
									enSubasta = false;
								}
								seguirPuja = false;
								break;
							case 1://Solo hay una puja mas alta
								mensajeOut = to_string(mSubas->nMensaje()) +";Pujador " + to_string(mSubas->PujadorActual()) + " hizo la max oferta. Quien ofrece: " + to_string(mSubas->pujaActual()) + "\n";
								break;
							default:// >1 pujador
								mensajeOut = to_string(mSubas->nMensaje()) +";Hay " + to_string(numPujadores) + ". Quien ofrece: " +to_string(mSubas->pujaActual()) + "\n";
							}	
						}
					}else{
						mensajeOut = to_string(mSubas->nMensaje()) +";La subasta esta mas adelantada. Por favor, espere una nueva subasta\n";
						cerr << "\033[35mCliente va con retraso, esperara nueva subasta\033[0m\n";
						if(subasta->Send(cliente, mensajeOut) <= 0){
							enSubasta = false;
						}
						seguirPuja = false;
					}
				}else{
					seguirPuja = false;
					enSubasta = false;
				}
			}
		}
	}
	int error = subasta->Close(cliente);
	if(error < 0){
		printf("error al desconectar cliente\n");
	}
	mSubas->quitarPujador();
}

int runSubastador(int puertoSubasta, monitorSubasta *mSubas, MonitorValla *mV){
	
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
		thread com(&controlSubasta, mSubas);
		com.detach();
	while(mSubas->SalonAbierto()){
		//Abrir Salon
		int cliente = subasta.Accept();
		cout << "Cliente con fd " + to_string(cliente) + " ha entrado en subasta conectado\n";
		thread th(&subastaCliente, &subasta,cliente, mSubas, mV);
		th.detach();
		// esperar a que puedan entrar más clientes a Subasta

	}
	cout << "bye bye\n";
	mSubas->finSubasta();
	subasta.Close(sockSubasta);
	
	
	return 0;
}