#ifndef MSUBASTA_HPP
#define MSUBASTA_HPP

#define Accepto "Acepto\n"
#define Pasa "Paso\n"
#define SaltarPujas "Salir puja Actual\n"
#define SalirSubasta "Salir de subasta\n"
#define CuotaMercado 10
#define CoeficienteTiempo 1.25
#define incrementoPuja 5

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <string>

using namespace std;

class monitorSubasta{
private:
	int TiempoAnuncio; //Tiempo en subasta de una valla
	int numMensaje;
	int pujadoresObservando;
	int precioRequerido;	//Precio ha alcanzar
	int pujadorMasAlto;	//Cliente que contiene la puja mas alta en tiempo T (no implica que sea unico)
	int posibleGanador; //Cliente con mayor puja en el tiempo T-1 (no implica que sea unico)
	int pujaMasAlta;	//Puja mas alta recibida
	int pujaMinima;	//Siguiente oferta
	int numPujas;	//Numero de pujas recibidas en el tiempo T
	int numPujasSend;	//Numero de pujas recibidas en el tiempo T-1
	bool primero;		//Primero que se recibe puja (gana ante empate)
	bool aceptandoPujas;
	bool fin_Subastas;	//True si y solo si no salen mas subastas 
	mutex exclusionDatos;        
	condition_variable envioPujas;	//Esperar notificacion de envio
	condition_variable comenzarS;	//ComienzaSubasta
	condition_variable esperarClientes;	//Espera hasta que haya almenos 1 cliente o fin servicio
	condition_variable cerrarSubasta;	//Espera a que no queden clientes para cerrar el socket
public:
	monitorSubasta();
	~monitorSubasta();
	//Inicializa una nueva subasta de tiempoValla 
	bool iniciarNuevaSubasta(int tiempoValla);
	//Devuelve el tiempo que se subastara
	int tiempoSubas();
	//devuelve el precio inicial marcado
	int comenzarSubasta();
	//reactualiza la proxima minima puja que se aceptara
	//devuelve true si y solo si la puja ha sido superada
	void pujar(string mensaje, int cliente);
	//Devuelve true si y solo si, se ha llegado al minino pedido
	bool SubastaAceptada();
	//Devuelve el cardinal de pujadores de la ronda actual
	int nPujas();
	//devuelve la acual puja
	int pujaActual();
	//devuelve el socket del actual cliente con una puja mayor
	int PujadorActual();
	//Despierta a los procesos que estan esperando a enviar una puja
	void enviarPuja();
	//devuelve true si y solo si se pueden conectar clientes
	bool SalonAbierto();
	//True si y solo si hay una subasta en curso
	bool SubastaEnCurso();
	//Cierra todas las posibles subastas nuevas
	bool CerrarSalon();
	//Despierta el proceso para que pueda cerrar el socket
	void finSubasta();
	//
	void anyadirPujador();
	void quitarPujador();
	int numPujadores();
	bool comenzarSubastas();
	int nMensaje();
	bool numMenAceptado(int numero);
	void CerrarSocket();

};

#endif 