//*****************************************************************
// File:   coutMonitor.h 
// Author: Ivan Escuin Gonzalez
// Date:   diciembre 2016
// Coms:   Header del monitor coutMonitor para gestionar la salida
//          pantalla de forma concurrente.
//         Compilar mediante make
//***************************************************************** 
#ifndef COUT_H
#define COUT_H

#include <mutex>
#include <condition_variable>
#include <iostream>
#include <string>

using namespace std;

class coutMonitor{
private:
    mutex m;
    condition_variable out;
    bool escribiendo = false;
    
public:
    void coutc(string s){
     unique_lock<std::mutex> lk(m);
     if(escribiendo){
        out.wait(lk);
     }
     escribiendo = true;
     cout<<s<<endl;
     escribiendo = false;
     out.notify_one();
    }
};
#endif 
