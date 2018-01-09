/*
 * Autores: Morón Vidal, Elena; Lafuente, Víctor M.; Escuín González, Iván
 * NIP: 739324; 747325; 684146
 * Fecha: enero 2018
 */

#include "mValla.hpp"
MonitorValla() {
    tiempoContratado = 0;
    tiempoMostrado = 0;
    imagenesMostradas = 0;
    finPeticiones = false;
}
~MonitorValla() {
}

//Añade a en como último elemento de la cola esperando
void MonitorValla::encolar(const Anuncio a) {
    unique_lock<recursive_mutex> lck(mtx);

    esperando.push(a);
    tiempoContratado += a.infoTiempo();
    cv.notify_one();
}

//Extrae el primer elemento de la cola esperando y asocia
//a anun el anuncio extraído. Devuelve cierto si y solo si 
//ha desencolado el anuncio 
bool MonitorValla::obtenerAnuncio(Anuncio& anun) {
    unique_lock<recursive_mutex> lck(mtx);
    while (esperando.size() < 1 && !finPeticiones) { //Tamaño 0 y la subasta sigue activa
        cv.wait(lck);
    }
    if (esperando.size() > 0) {   //Quedan elementos por extraer
        anun = esperando.front();
        tiempoMostrado += anun.infoTiempo();
        imagenesMostradas++;
        esperando.pop();
        return true;
    } else {  //No quedan elementos por extraer
        return false;
    }
}

//Devuelve el número de elementos que hay esperando
int MonitorValla::numEnEspera() {
    unique_lock<recursive_mutex> lck(mtx);

    return esperando.size();
}

//Asocia a los parámetros los valores que tienen en el momento de consultarse
void  MonitorValla::informacion (int& imgMost, int& tpoMost, int& tpoCont) {
    unique_lock<recursive_mutex> lck(mtx);
   
    imgMost = imagenesMostradas;
    tpoMost = tiempoMostrado;
    tpoCont = tiempoContratado;
}

//FinPeticiones = true
void MonitorValla::finServicio() {
    finPeticiones = true;
}