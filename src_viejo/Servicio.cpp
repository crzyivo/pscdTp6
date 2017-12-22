//*****************************************************************
// File:   Servicio.cpp
// Author: Ivan Escuin Gonzalez
// Date:   diciembre 2016
// Coms:   Implementacion del monitor Servicio para vallas de la practica 5
//***************************************************************** 
 

#include "Servicio.h"
#include <ctime>
#include <sstream>
#include <iostream>

using namespace std;

/*
 * Implementacion del metodo servicio, que gestiona peticiones de los clientes.
 * Calcula el coste y la hora aproximada de emision del anuncio y devuelve true, si esque queda hueco libre.
 * En caso contrario devuelve false.
 * 
 */
bool Servicio::servicio(Imagen i,int& c,string& t){
    std::unique_lock<std::mutex> lk(m);
    int tiempo = i.getTiempo();
    
    c=tiempo*10;
    
    int espera;
    if(cola.empty()){
        espera=0;
    }
    else{
        if(size>cola.size()){
            for(int i : tiempos){
                espera=espera+i;
            }
        }
        else{
                hueco.wait(lk);
                return false;
        }
    }
    std::time_t result = std::time(0) + espera;
    tm *tap = localtime(&result);
    string name;
    i.getImagen(name);
    stringstream buffer;
    buffer<<"Su anuncio "+name+" sera mostrado aproximadamente a las "<<tap->tm_hour<<":"<<tap->tm_min<<":"<<tap->tm_sec<<endl;
    t=buffer.str();
    cola.push(i);
    tiempos.push_front(tiempo);
    recibe.notify_one();
    return true;
}

/*
 * Implementacion del metodo envio_valla, que gestiona las peticiones de las vallas.
 * Devuelve un objeto de la clase Imagen, si hay elementos en la cola.
 * En caso contrario se queda bloqueado hasta que es notificado de la entrada de un elemento.
 * 
 */
Imagen Servicio::envio_valla(){
    std::unique_lock<std::mutex> lk(m);
    if(cola.empty()){
        recibe.wait(lk);
    }
    Imagen i=cola.front();
    //string envio=i.getImagen();
    //int tiempo = i.getTiempo();
    cola.pop();
    tiempos.pop_back();
    hueco.notify_all();
    return i;
    
}


bool Servicio::listarCola(list<string> &l){
    if(cola.empty()){
        return false;
    }else{
        int espera=0;
        time_t result;
        tm *tap;
        int n=0;
        for(int i : tiempos){
            espera = espera + i;
            result = time(0) + espera;
            tm *tap = localtime(&result);
            string t = to_string(tap->tm_hour)+":"+to_string(tap->tm_min)+":"+to_string(tap->tm_sec);
            l.push_back(t);
            n++;
        }
        l.push_front(to_string(n));
        return true;
    }
}
