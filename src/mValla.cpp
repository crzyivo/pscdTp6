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
}
~MonitorValla() {
}

//Añade a en como último elemento de la cola esperando
void MonitorValla::encolar(const Anuncio a) {
    unique_lock<recursive_mutex> lck(mtx);

    esperando.push(a);
    tiempoContratado += a.infoTiempo();
}

//Extrae el primer elemento de la cola esperando y asocia
//a anun el anuncio extraído
void MonitorValla::desencolar(Anuncio& anun) {
    unique_lock<recursive_mutex> lck(mtx);

    anun = esperando.front();
    tiempoMostrado += anun.infoTiempo();
    imagenesMostradas++;
    esperando.pop();
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