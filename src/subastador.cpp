#include <iostream>
#include <string>
#include <thread>
#include "Socket.hpp"

int puertoSubasta = 32005;

void subastaCliente(Socket& subasta, int cliente){
	while(/*Condicion cliente desconectado*/){
		string mensajeOut = "";
		string mensajeIn = "";
		int precioInicial = global.comenzarSubasta();
		mensajeOut = "Comienza la subasta en " + to_string(precioInical) + "\n";
		subasta.Send(cliente, mensajeOut);
		//esperar a que se dé la condición de iniciar subasta
		while(/*Condición de fin subasta Actual*/){
			subasta.Recv(cliente, mensajeIn);
			//leer mensaje
			if(global.pujar(pujaCliente)){
				mensajeOut = "Puja cliente " + to_string(cliente) + "aceptada. Quien ofrece\n" + to_string(global.pujaActual()+10) + "\n";
			}else{
				mensajeOut = "Puja no valida. Actual puja " + to_string(global.PujadorActual()) + to_string(global.pujaActual()) + "\n";
			}
			mensajeIn = "";
			subasta.Recv(cliente, mensajeIn);
		}
	}
	int error = subasta.Close(cliente);
	if(error < 0){
		printf("error al desconectar cliente\n");
	}
}

int main(int argc, char * argv[]){
	
	Socket subasta(puertoSubasta);
	
	int sockSubasta = subasta.Bind();
	int error = subasta.Listen();

	while(/*Condición fin de todas las subastas*/){
		//Abrir Salon
		int cliente = subasta.Accept();
		thread th(&subastaCliente, subasta, cliente);
		th.detach();
		// esperar a que puedan entrar más clientes a Subasta

	}
	subasta.Close(sockSubasta);
	
	return 0;
}