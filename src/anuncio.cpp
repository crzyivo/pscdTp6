/*
 * Autores: Morón Vidal, Elena; Lafuente, Víctor M.; Escuín González, Iván
 * NIP: 739324; 747325; 684146
 * Fecha: enero 2018
 */

#include "anuncio.hpp"


//Constructor de anuncio. El tiempo debe expresarse en segundos
Anuncio::Anuncio(const char nuevaURL[], int nuevoTiempo) {
    if (strlen(nuevaURL) < MAX_LONG_URL) {
       strcpy(URL, nuevaURL);
       tiempo = nuevoTiempo;
    } else {
        cerr << "Direccion que quiere asignar demasiado larga" << endl;
    }
}
//Destructor de anuncio
Anuncio::~Anuncio() {
}

//devuelve el tiempo que se va a mostrar el anuncio 
int Anuncio::infoTiempo() {
    return this->tiempo;
}
//asocia a URL la direccion que tiene el anuncio
void Anuncio::infoURL(char obtenerURL[]){
    strcpy(obtenerURL, URL);
}
