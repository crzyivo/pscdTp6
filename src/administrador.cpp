
#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include <thread>
#include <atomic>

#include "mValla.hpp"
// #include "mSubasta.hpp"
// #include "subastadorTest.cpp"
#include "gestorVallaTest.cpp"
using namespace std;

  
void mostrarEstado(MonitorValla m, int& nPeticiones, int& tiempoEstimado){
  int imgTot;
  int imgCol;
  int tiempoM;
  int tiempoC;
  m.informacion(imgTot,tiempoM,tiempoC,imgCol);
  nPeticiones = imgCol;
  tiempoEstimado = tiempoC;
}

int main(int argc, char * argv[]){
	string dir;
  int puertoSubasta = 32005;
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
  //Monitores
  MonitorValla vallas();
  //monitorSubasta subasta();
  
  thread thMV(&runGestorValla,vallas);
  
  return 0;
  
}