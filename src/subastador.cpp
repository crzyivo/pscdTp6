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
	bool enSubasta = true;
	const int maxMensaje = 1000;
	while(seguirSubasta = true && !subasta.CerrarSalon() ){
		//esperar a que se dé la condición de iniciar subasta
		int precioInicial = mSubas.comenzarSubasta();
		string mensajeIn = "";
		string mensajeOut = "Comienza la subasta en " + to_string(precioInicial) + "\n";
		bool seguirPuja = true;
		while(seguirPuja){
			subasta->Send(cliente, mensajeOut);
			subasta->Recv(cliente, mensajeIn,maxMensaje);
			int pujaCliente;
			//tratar mensajeIn
			if(mensaje == SaltarPujas){
				seguirPuja = false;
				subasta->Send(cliente, "Saliendo de subasta actual, esperando a que termine\n");
    		}else if(mensaje == SalirSubasta){
				seguirPuja = false;
				seguirSubasta = false;
				subasta->Send(cliente, "Saliendo del salon de subastas. Hasta pronto\n")
    		}else{
				if(mSubas.pujar(pujaCliente, cliente)){
					mensajeOut = "Hay" +  + " aceptada. Quien ofrece\n" + to_string(mSubas.pujaActual()+10) + "\n";
    			}else{
					mensajeOut = "Puja no valida. Actual puja " + to_string(mSubas.PujadorActual()) + to_string(mSubas.pujaActual()) + "\n";
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
					puertoSubasta = atoi(++argv[i]);
				}else if(*argv[i] == 'd'){	//mnumero surtidores
					dir = ++argv[i];
				}else{
					cout << "Uso: [-p<puerto>] [-d<direccion>]\n";
					cout << "\t-p<puerto>: puerto del servidor\n";
					cout << "\t-d<direccion>: direccion servidor\n";
					exit(1);
				}
			}
		}
	}
	puertoSubasta = 25000;
	Socket subasta(puertoSubasta);
        cout<<"Puerto de la subasta"<<puertoSubasta<<endl;
	
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