/*
 * Autores: Morón Vidal, Elena; Lafuente, Víctor M.; Escuín González, Iván
 * NIP: 739324; 747325; 684146
 * Fecha: enero 2018
 */

/* 
 * Fichero que se encarga de lanzar tantos procesos como vallas publicitarias
 * se requieran y gestiona que anuncien los archivos correspondientes
 */

#include <iostream>
#include <string>
#include <thread>
#include <stdlib.h>
#include "ImageDownloader.hpp"
#include "CImg.h"
#include "valla.hpp"
#include "mValla.hpp"
#include "anuncio.hpp"

using namespace std;
using namespace cimg_library;

const int MAX_LONG_NOMBRE_IMG = 100;    //Longitud máxima del nombre de una imagen
MonitorValla gestor();                  //Monitor que asegura desencolar en exclusión mutua
const int NUM_VALLAS = 2;               //Número de vallas que se ofertan

void mostrarImagen (Valla v, const int numValla, MonitorValla gestor) {
    int numImagen = 1;                      //Recuento de imágenes que ha mostrado este proceso
    Anuncio anuncio;                        //Variable que nos permite guardar el anuncio
    char nombreVentana[MAX_LONG_NOMBRE_IMG];    //Nombre de la ventana que representa la valla
    char numV = (char) numValla + '0';      //Pasa el número de valla a caracter
    
    strcpy(nombreVentana, "Valla ");
    strcat(nombreVentana, numV);
    
    while ( gestor.obtenerAnuncio(anuncio) ) {
       
        char numImg = (char) numImagen + '0';   //Pasa el número de imagen a caracter
        char URL[MAX_LONG_URL];                 //Guarda URL que se ha de descargar
        char nombreImg[MAX_LONG_NOMBRE_IMG];    //Nombre que tendrá la imagen que se descargue

        strcpy(nombreImg, "IMG");               
        strcat(nombreImg, numV);   
        strcat(nombreImg, numImg);
        strcat(nombreImg, ".jpg");              //nombreImg = IMG*numValla**numImg*.jpg

        // Creamos el objeto para descargar imágenes
        ImageDownloader downloader;
        anuncio.infoURL(URL);
        downloader.downloadImage(URL, nombreImg);

        // Creamos una valla publicitaria con una imagen
        /*cimg_library::*/CImg<unsigned char> img_principal(nombreImg);
        /*cimg_library::*/CImgDisplay vallaConImg(img_principal.resize(v.infoAnchura(),v.infoAltura()), nombreVentana);
        vallaConImg.resize(v.infoAnchura(),v.infoAltura());
        vallaConImg.move(numValla-1,numValla-1);

        // Mostrar imagen
	    vallaConImg.wait(anuncio.infoTiempo());
        numImagen++;
    }
}

int main() {
    // Tamaños de ventana para las vallas publicitarias
    const int VALLA_WIDTH = 800;
    const int VALLA_HEIGHT = 800;

    Valla vallasDisponibles[NUM_VALLAS];
    thread mostrar[NUM_VALLAS];

    for (int i = 0; i < NUM_VALLAS; i++) {
        vallasDisponibles[i](VALLA_HEIGHT, VALLA_WIDTH);
        mostrar[i] = thread (&mostrarImagen, vallasDisponibles[i], i+1)
    }

    for (int i = 0; i < NUM_VALLAS; i++) {
        mostrar[i].join();
    }

    return 0;
}