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
//MonitorValla gestor;

/*
 * Dada una valla y un gestor descarga las imágenes que se obtienen de los anuncios 
 * encolados y las muestra en valla
 */
void mostrarImagen (Valla v, const int numValla, MonitorValla* gestor) {
   
    int numImagen = 1;                      //Recuento de imágenes que ha mostrado este proceso
    Anuncio anuncio;                        //Variable que nos permite guardar el anuncio
    char nombreVentana[MAX_LONG_NOMBRE_IMG];    //Nombre de la ventana que representa la valla
    char numV[30];      //Pasa el número de valla a caracter
    sprintf(numV, "%d", numValla);
    
    strcpy(nombreVentana, "Valla ");
    strcat(nombreVentana, numV);
    
    CImgDisplay vallaConImg(v.infoAnchura(),v.infoAltura(),nombreVentana);

    while (gestor->obtenerAnuncio(anuncio)) {
       
        char numImg[30];
        sprintf(numImg, "%d", numImagen);   //Pasa el número de imagen a caracter
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
        CImg<unsigned char> img_principal(nombreImg);
        vallaConImg.resize(v.infoAnchura(),v.infoAltura());
        vallaConImg.move((numValla-1)*800, (numValla-1)*800);

        // Mostrar imagen
        vallaConImg.display(img_principal);
        this_thread::sleep_for(chrono::milliseconds(anuncio.infoTiempo()));
        numImagen++;
    }
}

/*
 * Se encarga de poner en funcionamiento tantos procesos como vallas haya
 */
void runGestorValla(MonitorValla* gestor) {
    // Tamaños de ventana para las vallas publicitarias
    const int VALLA_WIDTH = 400;
    const int VALLA_HEIGHT = 400;

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

int main() {
    char direccion1[100];
    char direccion2[100];
    char direccion3[100];
    char direccion4[100];
    strcpy(direccion1, "https://i.imgur.com/KwoYkdc.jpg");
    strcpy(direccion2, "https://i.imgur.com/v1YCACF.png");
    strcpy(direccion3, "https://i.imgur.com/1eJ1q0V.jpg");
    strcpy(direccion4, "https://i.imgur.com/xyya1CC.jpg");
    Anuncio anun1(direccion1, 5000);
    Anuncio anun2(direccion2, 5000);
    Anuncio anun3(direccion3, 5000);
    Anuncio anun4(direccion4, 5000);
    MonitorValla gestor;
    gestor.encolar(anun1);
    gestor.encolar(anun2);
    gestor.encolar(anun3);
    gestor.encolar(anun4);
    gestor.finServicio();
    runGestorValla(&gestor);
    return 0;
}