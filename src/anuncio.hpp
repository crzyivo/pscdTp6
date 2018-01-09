/*
 * Autores: Morón Vidal, Elena; Lafuente, Víctor M.; Escuín González, Iván
 * NIP: 739324; 747325; 684146
 * Fecha: enero 2018
 */

/* 
 * tipo Anuncio que va a gestionar toda la información relativa
 * a un anuncio (imagen que muestra, tiempo que debe
 * mostrarse)
 */
#ifndef VALLA_HPP
#define VALLA_HPP
#define MAX_LONG_URL 500	//Máxima longitud permitida para la URL

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <cstring>
#include <string>
#include "ImageDownloader.hpp"
#include "CImg.h"

using namespace std;
using namespace cimg_library;



class Anuncio{
private:
	char URL[500];	//Dirección desde la que tiene que descargarse la imagen
	int tiempo;	//Número de segundos que se va a mostrar la imagen del URL
public:
	//Constructor de anuncio
	Anuncio(char nuevaURL[], int nuevoTiempo);
	//Destructor de anuncio
	~Anuncio();
// 	//Sobrecargar operador = para poder asignar un anuncio a otro
// 	Anuncio& operator=(const Anuncio& a);
	//devuelve el tiempo que se va a mostrar el anuncio "publi"
	int infoTiempo();
	//asocia a URL la direccion que tiene el anuncio "publi"
	void infoURL(char obtenerURL[]);
};

#endif 