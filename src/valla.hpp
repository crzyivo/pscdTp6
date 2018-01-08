/*
 * Autores: Morón Vidal, Elena; Lafuente, Víctor M.; Escuín González, Iván
 * NIP: 739324; 747325; 684146
 * Fecha: enero 2018
 */

/* 
 * tipo Valla que va a gestionar toda la información relativa
 * a una valla publicitaria (anuncio que muestra, tamaño de la 
 * valla, imágenes totales que ha mostrado, tiempo total que ha
 * estado en uso)
 */
#ifndef VALLA_HPP
#define VALLA_HPP

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <string>
#include "anuncio.hpp"

using namespace std;

const int MAX_LONG_URL = 500;

class Valla{
private:
	Anuncio mostrado;		//Anuncio que se está mostrando en la valla
	int altura;				//Altura de la ventana (valla publicitaria)
	int anchura;			//Anchura de la ventana (valla publicitaria)
public:
	Valla(const int alto, const int ancho);
	~Valla();
	//Cambia el anuncio que se estaba mostrando por el anuncio a
	void nuevoAnuncio(const Anuncio a);
	//Devuelve la altura de la valla
	int infoAltura();
	//Devuelve la anchura de la valla
	int infoAnchura();
};

#endif 