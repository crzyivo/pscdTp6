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
#include "Socket.h"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

int main() {
    srand(time(0));
    const string MENS_FIN("END OF SERVICE");
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS;
    string SERVER_PORT;
    cout<<"Introduce la direccion servidor: \n";
    getline(cin,SERVER_ADDRESS);
    cout<<"Introduce el numero de puerto: \n";
    getline(cin,SERVER_PORT);
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
        
        
        string mensaje;
        string tvalla;
        int tam = rand()%2;
        int img = rand()%7;
        int tm = rand()%30+10;
        if(tam==0){
            tvalla="g";
        }else{
            tvalla="p";
        }
        mensaje = imagenes[img]+","+to_string(tm)+","+tvalla;
        // Enviamos el mensaje
        int send_bytes = socket.Send(socket_fd, mensaje);
        
        if(send_bytes == -1){
            cerr << "Error al enviar datos: " << strerror(errno) << endl;
            // Cerramos el socket
            socket.Close(socket_fd);
        }
        
        if(mensaje != MENS_FIN){
            // Buffer para almacenar la respuesta
            string buffer;
            
            // Recibimos la respuesta del servidor  
            int read_bytes = socket.Recv(socket_fd, buffer, MESSAGE_SIZE);
            
            // Mostramos la respuesta
            cout << "Mensaje enviado: '" << mensaje << "'" << endl;
            cout << "Peticion: " << buffer << endl;
        }
        
        // Cerramos el socket
        int error_code = socket.Close(socket_fd);
        if(error_code == -1){
            cerr << "Error cerrando el socket: " << strerror(errno) << endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}
