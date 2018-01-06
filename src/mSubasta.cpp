#include "mSubasta.hpp"

monitorSubasta::monitorSubasta(){
    pujadorMasAlto = 0;
    pujaMasAlta = 0;
    pujaMinima = 0;
    fin_Subastas = false;
}
monitorSubasta::~monitorSubasta(){
    
}
//devuelve el precio inicial marcado de manera que todos los procesos
// comiencen la subasta al mismo tiempo
int monitorSubasta::comenzarSubasta(){
    return pujaMinima;
}
//reactualiza la proxima minima puja que se aceptara
//devuelve true si y solo si la puja ha sido superada
bool monitorSubasta::pujar(string mensaje, int cliente){
    if(mensaje == Accepto){

    }else if(mensaje == Paso){
        
    }else if(mensaje == SaltarPujas){
        
    }else if(mensaje == SalirSubasta){

    }else{
        int puja;
        sscanf("Subo %d\n", puja);
        if(puja > this->pujaActual())
    }
}
//devuelve la acual puja
int monitorSubasta::pujaActual(){

}
//devuelve el socket del actual cliente con una puja mayor
int monitorSubasta::PujadorActual(){

}
//devuelve true si y solo si se pueden conectar clientes
bool monitorSubasta::SalonAbierto(){
    return !fin_Subastas;
}
//Cierra todas las posibles subastas nuevas
bool monitorSubasta::CerrarSalon(){

}
//Despierta el proceso para que pueda cerrar el socket
void monitorSubasta::finSubasta(){

}
