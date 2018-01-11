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
#ifndef ANUNCIO_HPP
#define ANUNCIO_HPP

#define MAX_LONG_URL 500	//Máxima longitud permitida para la URL

#include <iostream>
#include <cstring>
#include <string>

using namespace std;



class Anuncio{
private:
	char URL[500];	//Dirección desde la que tiene que descargarse la imagen
	int tiempo;		//Tiempo en segundos que se va a mostrar la imagen del URL
public:
	Anuncio() = default;
	//Constructor de anuncio
	Anuncio(const char nuevaURL[], int nuevoTiempo);
	//Destructor de anuncio
	~Anuncio();
	//devuelve el tiempo que se va a mostrar el anuncio
	int infoTiempo();
	//asocia a URL la direccion que tiene el anuncio
	void infoURL(char obtenerURL[]);
};

#endif 