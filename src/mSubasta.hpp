#ifndef MSUBASTA_HPP
#define MSUBASTA_HPP

#define Accepto "Acepto\n"
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
	int numPujas;
	int numPujasSend;
	bool fin_Subastas;
	mutex exclusion;        
	condition_variable_any cv;	
public:
	monitorSubasta();
	~monitorSubasta();
	//devuelve el precio inicial marcado
	int comenzarSubasta();
	//reactualiza la proxima minima puja que se aceptara
	//devuelve true si y solo si la puja ha sido superada
	void pujar(string mensaje, int cliente);
	//Devuelve el cardinal de pujadores de la ronda actual
	int nPujas();
	//devuelve la acual puja
	int pujaActual();
	//devuelve el socket del actual cliente con una puja mayor
	int PujadorActual();
	//Despierta a los procesos que estan esperando a enviar una puja
	void enviarPuja();
	//Devuelve true si y solo si hay una subasta por encima del precio minimo
	bool SubastaAceptada();
	//devuelve true si y solo si se pueden conectar clientes
	bool SalonAbierto();
	//Cierra todas las posibles subastas nuevas
	bool CerrarSalon();
	//Despierta el proceso para que pueda cerrar el socket
	void finSubasta();

};

#endif 