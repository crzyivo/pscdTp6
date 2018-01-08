//*****************************************************************
// File:   Cliente.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Ejemplo de cliente con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include "Socket.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

int main(int argc, char *argv[]) {
    srand(time(0));
    const string MENS_FIN("END OF SERVICE");
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS = "localhost";
    string SERVER_PORT = "32005";
    	string dir;
		if(argc >1){	//Inicializa con Parametros
		for (int i = 1; i< argc; i++){
			if(*argv[i]++ == '-'){
				if(*argv[i] == 'p'){		//numero de vueltas
					if(*++argv[i] == '\0'){i++;/*saltar espacio en blanco*/}
					 SERVER_PORT = argv[i];
				}else if(*argv[i] == 'd'){	//mnumero surtidores
					if(*++argv[i] == '\0'){i++; /*saltar espacio en blanco*/}
					SERVER_ADDRESS = argv[i];
				}else{
					cout << "Uso: [-p<puerto>] [-d<direccion>]\n";
					cout << "\t-p<puerto>: puerto del servidor\n";
					cout << "\t-d<direccion>: direccion servidor\n";
					exit(1);
				}
			}
		}
	}
    // cout<<"Introduce la direccion servidor: \n";
    // getline(cin,SERVER_ADDRESS);
    // cout<<"Introduce el numero de puerto: \n";
    // getline(cin,SERVER_PORT);
    string imagenes[7]={
        "http://i.imgur.com/eTuCPxM.jpg",
        "http://i.imgur.com/evzIQVF.jpg",
        "http://i.imgur.com/BiFkD83.jpg",
        "http://i.imgur.com/ApuihUx.jpg",
        "http://i.imgur.com/LUiR7W8.jpg",
        "http://i.imgur.com/5NljFvC.jpg",
        "http://i.imgur.com/EygIdZU.jpg"};
    
    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket socket(SERVER_ADDRESS,stoi(SERVER_PORT));
    while(true){
        // Conectamos con el servidor. Probamos varias conexiones
        const int MAX_ATTEMPS = 10;
        int count = 0;
        int socket_fd;
        do {
            // Conexión con el servidor
            socket_fd = socket.Connect();
            count++;
            
            // Si error --> esperamos 1 segundo para reconectar
            if(socket_fd == -1){
                this_thread::sleep_for(chrono::seconds(1));
            }
        } while(socket_fd == -1 && count < MAX_ATTEMPS);
        
        // Chequeamos si se ha realizado la conexión
        if(socket_fd == -1){
            return socket_fd;
        }
        
        
        
//         string mensaje;
//         string tvalla;
//         int tam = rand()%2;
//         int img = rand()%7;
//         int tm = rand()%30+10;
//         if(tam==0){
//             tvalla="g";
//         }else{
//             tvalla="p";
//         }
//         mensaje = imagenes[img]+","+to_string(tm)+","+tvalla;
//         // Enviamos el mensaje
//         int send_bytes = socket.Send(socket_fd, mensaje);
//         
//         if(send_bytes == -1){
//             cerr << "Error al enviar datos: " << strerror(errno) << endl;
//             // Cerramos el socket
//             socket.Close(socket_fd);
//         }
//         
//         if(mensaje != MENS_FIN){
//             // Buffer para almacenar la respuesta
//             string buffer;
//             
//             // Recibimos la respuesta del servidor  
//             int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
//             
//             // Mostramos la respuesta
//             cout << "Mensaje enviado: '" << mensaje << "'" << endl;
//             cout << "Peticion: " << buffer << endl;
//         }
        cout<<"He conectado\n";
        string inicio_puja="";
        string puja = "";
        string nueva_puja="";
        
        int read_bytes;
        
        read_bytes = socket.Recv(socket_fd,inicio_puja, MESSAGE_SIZE);
        if(read_bytes == -1){
          cerr << "Error al enviar datos: " << strerror(errno) << endl;
          // Cerramos el socket
          socket.Close(socket_fd);
        }
        
        cout << inicio_puja << endl;
        while(true){
          cin >> puja;
          if(puja == "p"){
              read_bytes = socket.Send(socket_fd,"Paso\n");
          }else if(puja == "a"){
              read_bytes = socket.Send(socket_fd,"Acepto\n");
        
          }else{
              cout<<"No te he entendido"<<endl;
          }
          read_bytes = socket.Recv(socket_fd,nueva_puja,MESSAGE_SIZE);
          cout<<nueva_puja;
        }
        
        
        // Cerramos el socket
        int error_code = socket.Close(socket_fd);
        if(error_code == -1){
            cerr << "Error cerrando el socket: " << strerror(errno) << endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
