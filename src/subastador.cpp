#include <iostream>
#include <string>
#include <thread>
#include "Socket.hpp"
#include "monitorSubasta.hpp"
/***********************
 * Mensajes Servidor Cliente:
 * "Comienza la subasta en %d\n"
 * "Puja cliente %d aceptada. Quien ofrece %d\n" 
 * "Puja no valida. Actual puja %d %d \n", cliente, puja
 **********************/ 
const int maxNumCLientes = 35;
monitorSubasta global();

int puertoSubasta = 32005;

void subastaCliente(Socket& subasta, int cliente){
	while(/*Condicion cliente desconectado*/){
		string mensajeOut = "";
		string mensajeIn = "";
		int precioInicial = global.comenzarSubasta();
		mensajeOut = "Comienza la subasta en " + to_string(precioInicial) + "\n";
		subasta.Send(cliente, mensajeOut);
		//esperar a que se dé la condición de iniciar subasta
		while(1/*Condición de fin subasta Actual*/){
			subasta.Recv(cliente, mensajeIn);
			//leer mensaje
			if(global.pujar(pujaCliente, cliente)){
				mensajeOut = "Puja cliente " + to_string(cliente) + " aceptada. Quien ofrece\n" + to_string(global.pujaActual()+10) + "\n";
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
	int error = subasta.Listen(maxNumCLientes);

	while(global.SalonAbierto()){
		//Abrir Salon
		int cliente = subasta.Accept();
		thread th(&subastaCliente, subasta, cliente);
		th.detach();
		// esperar a que puedan entrar más clientes a Subasta

	}
	global.finSubasta();
	subasta.Close(sockSubasta);
	
	return 0;
}