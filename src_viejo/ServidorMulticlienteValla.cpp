
//*****************************************************************
// File:   ServidorMulticlienteVallas.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Servidor para gestionar clientes y vallas, del ejercicio 3.
//         Compilar con el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <ctime>
#include <vector>
#include <list>
#include <functional>
#include <sstream>


#include "Servicio.h"
#include "CImg.h"
#include "Socket.h"
#include "CierreServidor.h"
#include "coutMonitor.h"
#include "ImageDownloader.h"

using namespace std;
char MENS_FIN[]="END OF SERVICE";
string imgfolder="img/";
CierreServidor cierre;
Servicio sp;
Servicio sg;
coutMonitor out;
ImageDownloader downloader;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres


void estadoColas(){
    out.coutc("-----------------------------------");
    while(true){
        list<string> l;
        cout<<"Grande: ";
        if(sg.listarCola(l)){
            string n = l.front();
            l.pop_front();
            out.coutc(n+" imagenes por mostrar");
            string img="|";
            for(string s: l){
                img=img+s+"|";
            }
            out.coutc(img);
        }else{
            out.coutc("Vacia");
        }
        l.clear();
        cout<<"Pequeña: ";
        if(sp.listarCola(l)){
            string n = l.front();
            l.pop_front();
            out.coutc(n+" imagenes por mostrar");
            string img="|";
            for(string s: l){
                img=img+s+"|";
            }
            out.coutc(img);
        }else{
            out.coutc("Vacia");
        }
        out.coutc("-----------------------------------");
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    
}
/*
 * Funcion para dividir un string dado un delimitador.
 * Codigo fuente:http://ysonggit.github.io/coding/2014/12/16/split-a-string-using-c.html
 */
vector<string> split(const string &s, char delim) {
    stringstream ss(s);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim)) {
        tokens.push_back(item);
    }
    return tokens;
}

/*
 * Proceso Valla Grande.
 * Crea una valla de 500x500 pixels, donde muestra las imagenes que obtiene del monitor.
 * En caso de que no queden imagenes por mostrar, enseña un fondo negro
 */
void vallaG(Servicio& s){
    cimg_library::CImgDisplay main_disp(500,500,"Valla Grande");
    while(true){
        Imagen re;
        re = s.envio_valla();
        string ss;
        re.getImagen(ss);
        char * namel = const_cast<char*>(ss.c_str());
        string dwld = imgfolder+"g.jpg";
        char * namec = const_cast<char*>(dwld.c_str());
        if((downloader.downloadImage(namel,namec))!=-1){
            cimg_library::CImg<unsigned char> image(namec);
            main_disp.display(image);
            
            out.coutc("Valla Grande: "+ss);
        }else{
            cerr<<"Error al descargar la imagen"<<endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds((re.getTiempo())));
        cimg_library::CImg<unsigned char> visu(500,400,1,3,0);
        main_disp.display(visu);
    }
}
/*
 * Proceso Valla Pequeña.
 * Crea una valla de 250x250 pixels, donde muestra las imagenes que obtiene del monitor.
 * En caso de que no queden imagenes por mostrar, enseña un fondo negro
 */
void vallaP(Servicio& s, int i){
    cimg_library::CImgDisplay main_disp(250,250,"Valla Pequeña "+i);
    while(true){
        Imagen re;
        re = s.envio_valla();
        string ss;
        re.getImagen(ss);
        char * namel = const_cast<char*>(ss.c_str());
        string dwld = imgfolder+to_string(i)+".jpg";
        char * namec = const_cast<char*>(dwld.c_str());
        if((downloader.downloadImage(namel,namec))!=-1){
            cimg_library::CImg<unsigned char> image(namec);
            main_disp.display(image);
            
            out.coutc("Valla Pequeña "+to_string(i)+" : "+ss);
        }else{
            cerr<<"Error al descargar la imagen"<<endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds((re.getTiempo())));
        cimg_library::CImg<unsigned char> visu(500,400,1,3,0);
        main_disp.display(visu);
    }
}

void conexion(int client_fd,int socket_fd,Socket socket,int ncliente){
    // Buffer para recibir el mensaje
    int length = 100;
    string buffer;
    out.coutc("Nueva conexion, cliente: "+to_string(ncliente));
    int clientes=cierre.getConectados();
    out.coutc("Clientes conectados: "+to_string(clientes));
    // Recibimos el mensaje del cliente
    int rcv_bytes = socket.Recv(client_fd, buffer, MESSAGE_SIZE);
    if(rcv_bytes == -1) {
        cerr << "Error al recibir datos: " << strerror(errno) << endl;
        // Cerramos los sockets
        socket.Close(client_fd);
        socket.Close(socket_fd);
    }
    
    out.coutc("Mensaje recibido de "+to_string(ncliente)+" : '"+buffer+"'");
    
    // Si recibimos "END OF SERVICE" --> Fin de la comunicación
    if(buffer == MENS_FIN){
    } else {
        // Tratamos la peticion de cliente mediante el servicio
        vector<string> param(3);
        param=split(buffer,',');
        cout<<param[0]<<endl;
        if(param[2]=="g"){
            int coste;
            string tiempo;
            Imagen img(stoi(param[1]),true,param[0]);
            if(sg.servicio(img,coste,tiempo)){
                string resp = "Coste "+to_string(coste)+","+tiempo;
                int send_bytes = socket.Send(client_fd, resp);
                if(send_bytes == -1) {
                    cerr << "Error al enviar datos: " << strerror(errno) << endl;
                    // Cerramos los sockets
                    socket.Close(client_fd);
                }
            }
            else{
                cout<<"Errpr"<<endl;
                int send_bytes = socket.Send(client_fd, "Denegado");
                if(send_bytes == -1) {
                    cerr << "Error al enviar datos: " << strerror(errno) << endl;
                    // Cerramos los sockets
                    socket.Close(client_fd);
                }
            }
        }else{
                     int coste;
            string tiempo;
            Imagen img(stoi(param[1]),false,param[0]);
            if(sp.servicio(img,coste,tiempo)){
                string resp ="Coste "+to_string(coste)+","+tiempo;
                int send_bytes = socket.Send(client_fd, resp);
                if(send_bytes == -1) {
                    cerr << "Error al enviar datos: " << strerror(errno) << endl;
                    // Cerramos los sockets
                    socket.Close(client_fd);
                }
            }
            else{
                cout<<"Errpr"<<endl;
                int send_bytes = socket.Send(client_fd, "Denegado");
                if(send_bytes == -1) {
                    cerr << "Error al enviar datos: " << strerror(errno) << endl;
                    // Cerramos los sockets
                    socket.Close(client_fd);
                }
            } 
        }
    }
    
    // Cerramos el socket del cliente
    int c = cierre.addCerrados();
    int error_code = socket.Close(client_fd);
    if(error_code == -1){
        cerr << "Error cerrando el socket del cliente: " << strerror(errno) << endl;
    }
}
//-------------------------------------------------------------
int main() {
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT;
    cout<<"Introduce el numero de puerto:\n";
    cin>>SERVER_PORT;
    
    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket socket(SERVER_PORT);
    
    // Bind 
    int socket_fd =socket.Bind();
    if (socket_fd == -1) {
        cerr << "Error en el bind: " << strerror(errno) << endl;
        exit(1);
    }
    
    //Inicio de las vallas
    thread th1(&vallaG,ref(sg));
    thread th2(&vallaP,ref(sp),1);
    thread th3(&vallaP,ref(sp),2);
    
    //Proceso que muestra el estado de los servicios
    thread th4(&estadoColas);
    // Listen
    int max_connections = 30;
    int error_code = socket.Listen(max_connections);
    if(error_code == -1) {
        cerr << "Error en el listen: " << strerror(errno) << endl;
        // Cerramos el socket
        socket.Close(socket_fd);
        exit(1);
    }
    
    // Accept
    
    int client_fd;
    int n = 0;
    while((client_fd = socket.Accept())>-1){
        cierre.addAbiertos();
        thread th(&conexion,client_fd,socket_fd,socket,n);
        th.detach();
        n++;
        
        
    }
    if(client_fd == -1) {
        cerr << "Error en el accept: " << strerror(errno) << endl;
        // Cerramos el socket
        socket.Close(socket_fd);
        exit(1);
    }
    
    
    
    // Cerramos el socket del servidor
    error_code = socket.Close(socket_fd);
    if(error_code == -1){
        cerr << "Error cerrando el socket del servidor: " << strerror(errno) << endl;
    }
    // Mensaje de despedida
    cout << "Bye bye" << endl;
    th1.join();
    th2.join();
    th3.join();
    th4.join();
    
    return error_code;
}
