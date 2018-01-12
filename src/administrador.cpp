/*
 * Autores: Lafuente, Víctor M.; Escuín González, Iván; Morón Vidal, Elena
 * NIP: 747325; 684146; 739324
 * Fecha: enero 2018
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <thread>
#include <atomic>

#include "mValla.hpp"
#include "anuncio.hpp"
#include "mSubasta.hpp"
#include "subastador.cpp"
#include "gestorValla.cpp"
#include "Socket.hpp"
using namespace std;

  
void mostrarEstado(MonitorValla* m, int& nPeticiones, double& tiempoEstimado){
  int imgTot;
  int imgCol;
  int tiempoM;
  int tiempoC;
  m->informacion(imgTot,tiempoM,tiempoC,imgCol);
  nPeticiones = imgCol;
  tiempoEstimado = (tiempoC-tiempoM);
}

void mostrarHistorico(MonitorValla* m, int& nPeticiones, double& tiempoTotal, double& tiempoMedio){
  int imgTot;
  int imgCol;
  int tiempoM;
  int tiempoC;
  m->informacion(imgTot,tiempoM,tiempoC,imgCol);
  nPeticiones = imgTot;
  tiempoTotal = tiempoM;
  tiempoMedio = tiempoTotal/nPeticiones;
}

int main(int argc, char * argv[]){
  int puertoSubasta = 32005;
  bool modoTest = false;
   if(argc >1){	//Inicializa con Parametros
 		for (int i = 1; i< argc; i++){
 			if(*argv[i]++ == '-'){
 				if(*argv[i] == 'p'){		//numero de vueltas
 					if(*++argv[i] == '\0'){i++;/*saltar espacio en blanco*/}
 					puertoSubasta = atoi(argv[i]);
 				}else if(*argv[i] == 't'){	//direccion del servidor
					if(*++argv[i] == '\0'){i++; /*saltar espacio en blanco*/}
					modoTest = true;
        }else{
 					cout << "Uso: [-p<puerto>] [-t]\n";
 					cout << "\t-p<puerto>: puerto del servidor\n";
          cout << "\t-t: modo test con anuncios ya cargados\n";
 					exit(1);
 				}
 			}
 		}
 	}
 	
 	//Monitores
  MonitorValla vallas;
  monitorSubasta subasta;
  
  if(modoTest){
    char direccion[] ="http://i.imgur.com/evzIQVF.jpg";
    Anuncio a(direccion, 3);
    vallas.encolar(a);
    vallas.encolar(a);
    vallas.encolar(a);
    vallas.encolar(a);
    vallas.encolar(a);
    vallas.encolar(a);
  }
  
  //Inicio el gestor de vallas y de la subasta.
  thread thMV(&runGestorValla,&vallas);
  thread thMS(&runSubastador,puertoSubasta ,&subasta, &vallas);
  
  //Bucle de instrucciones
  bool fin = false;
  int n;
  double t;
  double m;
  Socket socket("localhost",puertoSubasta);
  int socket_fd;
  while(!fin){
    cout<<"\nInstrucciones del administrador:\n";
    cout<<"1-> Mostrar estado de las vallas\n"
        <<"2-> Mostrar historico de las vallas\n"
        <<"3-> Apagar el sistema"<<endl;
    int op;
    cin>>op;
    switch(op){
      case 1:
              mostrarEstado(&vallas,n,t);
              cout<<"\nAnuncios en espera: "<<n<<"."<<endl;
              cout<<"Tiempo actual contratado: "<<t<<" segundos."<<endl;
              break;
      case 2:
              mostrarHistorico(&vallas,n,t,m);
              cout<<"\nAnuncios totales mostrados: "<<n<<"."<<endl;
              cout<<"Tiempo total de anuncios: "<<t<<" segundos."<<endl;
              cout<<"Tiempo medio de los anuncios: "<<m<<" segundos."<<endl;
              break;
      case 3:
              cout<<"\nEsperando terminación ordenada..."<<endl;
              //Cierro la subasta y espero.
              subasta.CerrarSalon();
              socket_fd=socket.Connect();
              socket.Close(socket_fd);
              thMS.join();
              cout<<"Subastas finalizadas"<<endl;
              //Una vez cerrada la subasta, cierro las vallas.
              vallas.finServicio();
              thMV.join();
              cout<<"Vallas finalizadas"<<endl;

              fin = true;
              break;
      default:
              cout<<"Instruccion erronea"<<endl;
    }
  }

  
  cout<<"\nFin correcto del servicio"<<endl;
  
  return 0;
  
}