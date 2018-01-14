/*
 * Autores: Lafuente, Víctor M.; Escuín González, Iván; Morón Vidal, Elena
 * NIP: 747325; 684146; 739324
 * Fecha: enero 2018
 */

#include <iostream>
#include <signal.h>
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
 * Mensajes Cliente Servidor
 *  "Acepto"
 *  "Subo %d"
 *  "Paso"
 *  "Salir puja Actual"
 *  "Salir de subasta"
 **********************/ 

//monitorSubasta mSubas;
const int maxNumCLientes = 35;
const int tiempoEntrePujas = 5000; //Tiempo que ha de esperar a que los interesados contesten
const int MAX_TIEMPO_VALLA = 50;
const int MIN_TIEMPO_VALLA = 20;
int puertoSubasta = 32005;
monitorSubasta * mSubas;
void sinConectar(int i){signal(SIGABRT, sinConectar);
	cerr << "\033[1;31mSenal de abort durante el proceso de conexiones\033[0m\n";
}
void clienteAbandona(int i){
	signal(SIGPIPE, clienteAbandona);
	cerr << "\033[1;31mCLiente salio inesperadamente\033[0m\n";
}

void controlSubasta(){
	signal(SIGPIPE, clienteAbandona);
	srand(time(NULL));
	while(mSubas->SalonAbierto()){
		//NO EMPEZAR CON 0 Clientes
		if(mSubas->comenzarSubastas()){
			this_thread :: sleep_for(chrono :: milliseconds(tiempoEntrePujas));
			mSubas->iniciarNuevaSubasta(rand()%MAX_TIEMPO_VALLA+MIN_TIEMPO_VALLA); 
			while(mSubas->numPujadores() > 0 && mSubas->SubastaEnCurso()){
				this_thread :: sleep_for(chrono :: milliseconds(tiempoEntrePujas));
				mSubas->enviarPuja();
			}
		}
	}
	mSubas->noAceptarPujas(); //asegurarse de que esta todo cerrado
	mSubas->CerrarSalon(); //avisar si aun queda gente esperando
	//Esperar a que todos los clientes se puedan desconectar antes de forzar cierre del subastador
	for(int i = 0; i < 3 && mSubas->numPujadores() >0; i++){ 
		this_thread :: sleep_for(chrono :: milliseconds(tiempoEntrePujas));
	}
	//Si quedasen clientes, se fuerza el cierre del sistema
	if(mSubas->numPujadores() >0){
		mSubas->forzarCierre();
	}
}


void subastaCliente(Socket *subasta, int cliente, MonitorValla *mV){
	signal(SIGPIPE, clienteAbandona);
	//signal(SIGABRT, sinConectar);
	bool errorRecv = false;
	bool enSubasta = true;
	const int maxMensaje = 1000;
	//Bucle de subastas
	//Cada iteración completa comprende desde el comienzo hasta su finalizacion de una subasta
	if(errorRecv || subasta->Send(cliente, to_string(cliente)+ "\n") <= 0){
		errorRecv = true;
		enSubasta = false;
	}else{
		mSubas->anyadirPujador();
		while( !errorRecv && enSubasta && mSubas->SalonAbierto()){
			//esperar a que se dé la condición de iniciar subasta
			int precioInicial = mSubas->comenzarSubasta();	//Bloqueante. devuelve precio de salida de subasta
			string mensajeIn = "";	//Mensajes desde cliente
			bool seguirPuja = true;	//true = Cliente esta interesado en observar constantemente el estado de la subasta actual
			int t; //Tiempo a subasta
			string mensajeOut;
			if(errorRecv || precioInicial <1){
				seguirPuja = false;
			}else{
				t = mSubas->tiempoSubas();
				mensajeOut = to_string(mSubas->nMensaje()) +";Comienza la subasta en " + to_string(precioInicial) + " para un tiempo " + to_string(t) +"\n"; //mensajes para cliente
			}
			//Bucle de subasta
			//Cada iteracion corresponde al envio del precio actual, recibir y tratar mensajes de pujadores
			while(!errorRecv && seguirPuja && mSubas->SubastaEnCurso()){
				if(!errorRecv && subasta->Send(cliente, mensajeOut) >0){
					if(errorRecv || subasta->Recv(cliente, mensajeIn, maxMensaje)<1){
						cerr << "\033[31mError al recibir, cliente desconectado\033[0m\n";
						enSubasta = false;
						seguirPuja = false;
						errorRecv = true;
					}else{
						
						string mensajeChar;
						if(mSubas->separaMensaje(mensajeIn, mensajeChar) || mensajeChar == "Salir de subasta\n" ){
							if(mensajeChar == SaltarPujas){	//pujador no interesado en seguir subasta actual
								seguirPuja = false;
								if(!errorRecv && subasta->Send(cliente, to_string(mSubas->nMensaje()) +";Subasta actual ignorada, esperando a que termine\n") <= 0){
									enSubasta = false;
									errorRecv = true;
								}
							}else if(mensajeChar == SalirSubasta){	//pujador no interesado en ninguna subasta
								seguirPuja = false;
								enSubasta = false;
								if(!errorRecv && subasta->Send(cliente, to_string(mSubas->nMensaje()) +";Saliendo del salon de subastas. Hasta pronto\n") <1){
									errorRecv =true;
								}
							}else{ //Cliente mantiene interes en la puja (haya aceptado o pasado)
								mSubas->pujar(mensajeChar, cliente);
								int numPujadores;
								switch(numPujadores = mSubas->nPujas()){	//Bloqueante. Devuelve nº de pujadores interesados
								case 0://Nadie ha pujado
									if(mSubas->SubastaAceptada()){
										mensajeOut = to_string(mSubas->nMensaje()) +";Ganador de la puja: " + to_string(mSubas->PujadorActual()) + "\n";
										if(mSubas->PujadorActual() ==  cliente){
											if(!errorRecv && subasta->Send(cliente, mensajeOut) >0){
												if(errorRecv || subasta->Recv(cliente, mensajeIn, maxMensaje)<1){
													cerr << "\033[31mError al recibir, cliente desconectado\033[0m\n";
													enSubasta = false;
													seguirPuja = false;
													errorRecv = true;
												}else{
												
													string obUrl = strtok(strdup(mensajeIn.c_str()), ":");
													while("http" != obUrl && !errorRecv) {
														if(!errorRecv && subasta->Send(cliente, "Usted ha ganado la puja, por favor, envia una url valida\n") <1){
															errorRecv = true;
														}
														if(!errorRecv && subasta->Recv(cliente, mensajeIn, maxMensaje)>1){
															obUrl = strtok(strdup(mensajeIn.c_str()), ":");
														}else{
															cerr << "\033[31mError al recibir, cliente desconectado\033[0m\n";
															enSubasta = false;
															seguirPuja = false;
															errorRecv = true;
														}
													}
														mensajeOut = "URL recibida\n";
														Anuncio anun(mensajeIn.c_str(), t); 
														mV->encolar(anun);
												}
											}else{
												errorRecv = true;
											}
											//Encolar URL
										}
									}else{
										mensajeOut = to_string(mSubas->nMensaje()) +";Minimo no alcanzado\n";
									}
									if(!errorRecv && subasta->Send(cliente, mensajeOut) <= 0){
										enSubasta = false;
										errorRecv = true;
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
							if(!errorRecv && subasta->Send(cliente, mensajeOut) <= 0){
								enSubasta = false;
								errorRecv = true;
							}
							seguirPuja = false;
						}

					}
				}else{
					cerr << "\033[1;31m" + to_string(cliente) + " FALLO EN EL ENVIO\033[0m\n";
					errorRecv = true;
					seguirPuja = false;
					enSubasta = false;
				}
			}
		}
	}

	if(!errorRecv && subasta->Send(cliente, "0;Desconectado del servidor\n")){
		errorRecv = true;
	}
	int error = subasta->Close(cliente);
	if(error < 0){
		printf("error al desconectar cliente\n");
	}

	mSubas->quitarPujador();

}

int runSubastador(int puertoSubasta, monitorSubasta *mSubasAdmin, MonitorValla *mV){

	mSubas = mSubasAdmin;
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
        int cliente;
	while((cliente =subasta.Accept()) > -1 && mSubas->SalonAbierto()){
		//Abrir Salon
		thread th(&subastaCliente, &subasta,cliente, mV);
		th.detach();
		// esperar a que puedan entrar más clientes a Subasta

	}

	subasta.Close(cliente);
	mSubas->finSubasta();
	subasta.Close(sockSubasta);

	cout << "Proceso de subasta finalizado correctamente\n";
	return 0;
}