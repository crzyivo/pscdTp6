

using namespace std;

Class estadoSistema{
  private:
    int nPeticiones;
    int tiempoEstimado;
  public:
    estadoSistema(int n,int t){
      nPeticiones = n;
      tiempoEstimado = t;
    }
    int getnPeticiones(){
      return nPeticiones;
    }
    int gettiempoEstimado(){
      return tiempoEstimado;
    }
};
  
estadoSistema mostrarEstado (MonitorValla m){
  int imgTot;
  int imgCol;
  int tiempoM;
  int tiempoC;
  m.informacion(img,tiempoM,tiempoC,imgCol);
  estadoSistema estado = estadoSistema(imgCol,tiempoC);
  return estado;
}

void main(int argc, char * argv[]){
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
  MonitorValla vallas = MonitorValla();
  monitorSubasta subasta = monitorSubasta();
  
  thread thMV(&runGestorValla,vallas);
  
}