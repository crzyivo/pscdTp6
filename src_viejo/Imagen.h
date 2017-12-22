//*****************************************************************
// File:   Imagen.h 
// Author: Ivan Escuin Gonzalez
// Date:   noviembre 2016
// Coms:   Clase Imagen de la practica 4
//          Compilar mediante make
//*****************************************************************
#include<string>
#ifndef IMAGEN_H
#define IMAGEN_H

using namespace std;

class Imagen{
private:
    int tiempo;
    bool grande;
    string imagen;
public:
    Imagen(){tiempo = 0;}
    Imagen(int t,bool s,string i){
        tiempo=t;
        grande=s;
        imagen=i;
        
    }
    
    int getTiempo(){
        return tiempo;
    }
    
    bool getTamanyo(){
        return grande;
    }
    
    void getImagen(string& i){
        i=imagen;
    }
};
#endif
