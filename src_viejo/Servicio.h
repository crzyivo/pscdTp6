//*****************************************************************
// File:   Servicio_p.h 
// Author: Ivan Escuin Gonzalez
// Date:   noviembre 2016
// Coms:   Header del monitor Servicio para vallas pequeñas de la practica 4
//         Compilar mediante make
//***************************************************************** 
#ifndef SERVICIOP_H
#define SERVICIOP_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <list>

#include "Imagen.h"



class Servicio{
private:
    std::mutex m;
    std::queue<Imagen> cola;
    std::list<int> tiempos;
    condition_variable hueco;
    condition_variable recibe;
    int size = 20;
    
public:
    bool servicio(Imagen i,int& c,string& t);
    
    Imagen envio_valla();
    
    bool listarCola(list<string> &l);
};
#endif