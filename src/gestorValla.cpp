/*
 * Autores: Lafuente, Víctor M.; Escuín González, Iván; Morón Vidal, Elena
 * NIP: 747325; 684146; 739324
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
#include <ctime>
#include "ImageDownloader.hpp"
#include "CImg.h"
#include "valla.hpp"
#include "mValla.hpp"
#include "anuncio.hpp"

using namespace std;
using namespace cimg_library;

const int MAX_LONG_NOMBRE_IMG = 100;    //Longitud máxima del nombre de una imagen
const int NUM_VALLAS = 2;               //Número de vallas que se ofertan
const int VALLA_WIDTH = 400;			//Ancho de las vallas publicitarias
const int VALLA_HEIGHT = 400;			//Alto de las vallas publicitarias

/*
 * Dada una valla y un gestor descarga las imágenes que se obtienen de los anuncios 
 * encolados y las muestra en valla. Si la descarga no se ha realizado correctamente
 * informa al usuario por la salida estándar del error.
 */
void mostrarImagen (Valla v, const int IDvalla, MonitorValla* gestor) {

    Anuncio anuncio;                         //Variable que nos permite guardar el anuncio
	const int alturaV = v.infoAltura();		 //Altura de la valla v
	const int anchuraV = v.infoAnchura();	 //Anchura de la valla v
    char nombreVentana[MAX_LONG_NOMBRE_IMG]; //Nombre de la ventana que representa la valla
    char numV[30];      					 //Identificador de valla como caracter
    sprintf(numV, "%d", IDvalla);
    
    strcpy(nombreVentana, "Valla ");
    strcat(nombreVentana, numV);
    
    CImgDisplay vallaConImg(anchuraV,alturaV,nombreVentana);
    CImg<unsigned char> visu(anchuraV,alturaV,1,3,0);
    
    while (gestor->obtenerAnuncio(anuncio)) {
		
		v.nuevoAnuncio(anuncio);				//Cambia el anuncio que muestra v
	    char URL[MAX_LONG_URL];                 //Guarda URL que se ha de descargar
        char nombreImg[MAX_LONG_NOMBRE_IMG];    //Nombre que tendrá la imagen que se descargue

        strcpy(nombreImg, "IMG");               
        strcat(nombreImg, numV);
        strcat(nombreImg, ".jpg");              //nombreImg = IMG*IDvalla*.jpg

        // Crear el objeto para descargar imágenes
        ImageDownloader downloader;
        anuncio.infoURL(URL);
        if(downloader.downloadImage(URL, nombreImg) < 0){ 
			//La imagen no se ha descargado correctamente
            cerr << "\033[31mError en la descarga en valla " + to_string(IDvalla) + "\033[0m\n";
        }else{

        	// Crear una valla publicitaria con una imagen
            CImg<unsigned char> img_principal(nombreImg);
            vallaConImg.resize(anchuraV,alturaV);
            vallaConImg.move((IDvalla-1)*(anchuraV+50), IDvalla-1);

            // Mostrar imagen
            vallaConImg.display(img_principal);
            this_thread::sleep_for(chrono::seconds(anuncio.infoTiempo()));
            vallaConImg.display(visu);          //Ventana en negro (sin mostrar imagen)
        }
    }
}

/*
 * Se encarga de poner en funcionamiento tantos procesos como vallas haya
 */
void runGestorValla(MonitorValla* gestor) {
    // Tamaños de ventana para las vallas publicitarias

    Valla vallasDisponibles[NUM_VALLAS];
    thread mostrar[NUM_VALLAS];

    for (int i = 0; i < NUM_VALLAS; i++) {
        vallasDisponibles[i]= Valla(VALLA_HEIGHT, VALLA_WIDTH);
        mostrar[i] = thread (&mostrarImagen, vallasDisponibles[i] , i+1, gestor);
    }
	
    for (int i = 0; i < NUM_VALLAS; i++) {
    	mostrar[i].join();
    }
    
}