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

int puertoSubasta = 32005;


void subastaCliente(Socket *subasta, int cliente){
	bool enSubasta = true;
	const int maxMensaje = 1000;
	while(enSubasta && !mSubas.CerrarSalon()){
		//esperar a que se dé la condición de iniciar subasta
		int precioInicial = mSubas.comenzarSubasta();
		string mensajeIn = "";
		string mensajeOut = "Comienza la subasta en " + to_string(precioInicial) + "\n";
		bool seguirPuja = true;
		while(seguirPuja){
			subasta->Send(cliente, mensajeOut);
			subasta->Recv(cliente, mensajeIn,maxMensaje);
			//tratar mensajeIn
			if(mensajeIn == SaltarPujas){
				seguirPuja = false;
				subasta->Send(cliente, "Saliendo de subasta actual, esperando a que termine\n");
    		}else if(mensajeIn == SalirSubasta){
				seguirPuja = false;
				enSubasta = false;
				subasta->Send(cliente, "Saliendo del salon de subastas. Hasta pronto\n");
    		}else{
				mSubas.pujar(mensajeIn, cliente);
				int numPujadores;
				switch(numPujadores = mSubas.nPujas()){
				case 0://Nadie ha pujado
					mensajeOut = "Ganador de la puja\n";
					break;
				case 1://Solo hay una puja mas alta
					mensajeOut = "Cliente X";
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
//	puertoSubasta = 25000;
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