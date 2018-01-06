#ifndef MSUBASTA_HPP
#define MSUBASTA_HPP

#define Accepto "Acepto\n"
#define Subo "Subo %d\n"
#define Pasa "Paso\n"
#define SaltarPujas "Salir puja Actual\n"
#define SalirSubasta "Salir de subasta\n"
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <string>

using namespace std;

class monitorSubasta{
private:
	int pujadorMasAlto;
	int pujaMasAlta;
	int pujaMinima;
	bool fin_Subastas;
	recursive_mutex mtx;        
	condition_variable_any cv;	
public:
	monitorSubasta();
	~monitorSubasta();
	//devuelve el precio inicial marcado
	int comenzarSubasta();
	//reactualiza la proxima minima puja que se aceptara
	//devuelve true si y solo si la puja ha sido superada
	bool pujar(int pujaCliente, int cliente);
	//devuelve la acual puja
	int pujaActual();
	//devuelve el socket del actual cliente con una puja mayor
	int PujadorActual();
	//devuelve true si y solo si se pueden conectar clientes
	bool SalonAbierto();
	//Cierra todas las posibles subastas nuevas
	bool CerrarSalon();
	//Despierta el proceso para que pueda cerrar el socket
	void finSubasta();

};

#endif 