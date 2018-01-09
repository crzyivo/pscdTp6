/*
 * Autores: Morón Vidal, Elena; Lafuente, Víctor M.; Escuín González, Iván
 * NIP: 739324; 747325; 684146
 * Fecha: enero 2018
 */

/* 
 * tipo MonitorValla que va a gestionar el funcionamiento de una 
 * varias vallas publicitarias de forma que las operaciones que las
 * modifican se realicen en exlusión mutua, así como el control de 
 * ciertos datos estadísticos (anuncios que se han mostrado, tiempo
 * contratado estimado)
 */

#ifndef MVALLA_HPP
#define MVALLA_HPP

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <string>
#include <queue>
#include "anuncio.hpp"
#include "valla.hpp"

using namespace std;

class MonitorValla{
private:
	queue<Anuncio> esperando;	//Cola que controla las peticiones pendientes de ser atendidas
	int tiempoContratado;		//Tiempo (en segundos) que se ha contratado para mostrar publicidad
	int tiempoMostrado;			//Tiempo total (en segundos) que se han mostrado anuncios
	int imagenesMostradas;		//Número de imágenes que han sido mostradas
	recursive_mutex mtx;        
	condition_variable_any cv;	
public:
	MonitorValla();
	~MonitorValla();
	//Añade a como último elemento de la cola
	void encolar(const Anuncio a);
	//Extrae el primer elemento de la cola y lo asocia
	//a anun el anuncio extraído
	void desencolar(Anuncio& anun);
	//Devuelve el número de elementos que hay en la cola
	int numEnEspera();
	//Devuelve cierto si y solo si hay elementos esperando la cola
	bool hayAnuncios();
	//Asocia a los parámetros los valores que tienen en el momento de consultarse
	void informacion (int& imgMost, int& tpoMost, int& tpoCont);
};

#endif 