/*
 * Autores: Lafuente, Víctor M.; Escuín González, Iván; Morón Vidal, Elena
 * NIP: 747325; 684146; 739324
 * Fecha: enero 2018
 */

#include "valla.hpp"


Valla::Valla(const int alto, const int ancho) {
    altura = alto;
    anchura = ancho;
}
Valla::~Valla() {
}

//Cambia el anuncio que se estaba mostrando por el anuncio a
void Valla::nuevoAnuncio(const Anuncio a) {
    mostrado = a;
}

//Devuelve la altura de la valla
int Valla::infoAltura() {
    return altura;
}

//Devuelve la anchura de la valla
int Valla::infoAnchura() {
    return anchura;
}
