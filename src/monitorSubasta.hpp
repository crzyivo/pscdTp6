#ifdef monitorSubasta_HPP
#define monitorSubasta_HPP

#include <iostream>
#include <mutex>
#include <string>

using namespace std;

class monitorSubasta{
    //devuelve el precio inicial marcado
    int comenzarSubasta();
    //reactualiza la proxima minima puja que se aceptara
    //devuelve true si y solo si la puja ha sido superada
    bool pujar(int pujaCliente, int cliente);
    //devuelve la acual puja
    int pujActual();
    //devuelve el socket del actual cliente con una puja mayor
    int pujadorActual();
    //devuelve true si y solo si se pueden conectar clientes
    bool salonAbierto();
    //Cierra todas las posibles subastas nuevas
    bool cerrarSalon();
    //Despierta el proceso para que pueda cerrar el socket
    void finSubasta();

    private:
        int pujadorMasAlto;
        int pujaMasAlta;
        int pujaMinima;
        bool finSubasta;
        condition_variable cv;

};

#endif 