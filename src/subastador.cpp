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
const int maxNumCLientes = 35;
monitorSubasta mSubas;
list<int> clientes;

int puertoSubasta = 32005;


void subastaCliente(Socket *subasta, int cliente){
	const maxMensaje = 1000;
	while(1/*Condicion cliente desconectado*/){
		int precioInicial = mSubas.comenzarSubasta();
		string mensajeIn = "";
		string mensajeOut = "Comienza la subasta en " + to_string(precioInicial) + "\n";
		
		//esperar a que se dé la condición de iniciar subasta
		while(1/*Condición de fin subasta Actual*/){
			subasta->Send(cliente, mensajeOut);
			subasta->Recv(cliente, mensajeIn,maxMensaje);
			int pujaCliente;
			//tratar mensajeIn
			//leer mensaje
			if(mSubas.pujar(pujaCliente, cliente)){
				mensajeOut = "Puja cliente " + to_string(cliente) + " aceptada. Quien ofrece\n" + to_string(mSubas.pujaActual()+10) + "\n";
			}else{
				mensajeOut = "Puja no valida. Actual puja " + to_string(mSubas.PujadorActual()) + to_string(mSubas.pujaActual()) + "\n";
			}
			mensajeIn = "";
			subasta->Recv(cliente, mensajeIn,maxMensaje);
		}
	}
	int error = subasta->Close(cliente);
	if(error < 0){
		printf("error al desconectar cliente\n");
	}
}

int main(int argc, char * argv[]){
	
	Socket subasta(puertoSubasta);
	
	int sockSubasta = subasta.Bind();
	int error = subasta.Listen(maxNumCLientes);

	while(mSubas.SalonAbierto()){
		//Abrir Salon
		int cliente = subasta.Accept();
		thread th(&subastaCliente, &subasta,cliente);
		//th.detach();
		// esperar a que puedan entrar más clientes a Subasta

	}
	mSubas.finSubasta();
	subasta.Close(sockSubasta);
	
	return 0;
}