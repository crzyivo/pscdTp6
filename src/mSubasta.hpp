/*
 * Autores: Lafuente, Víctor M.; Escuín González, Iván; Morón Vidal, Elena
 * NIP: 747325; 684146; 739324
 * Fecha: enero 2018
 */

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
#include <cstring>

using namespace std;

class monitorSubasta{
private:
	int TiempoAnuncio; //Tiempo en subasta de una valla
	int numMensaje;		
	int pujadoresObservando;	//Número de pujadores que están esperando a que comience una subasta
	int precioRequerido;	//Precio a alcanzar
	int pujadorMasAlto;	//Cliente que tiene la puja mas alta en tiempo T (no implica que sea unico)
	int posibleGanador; //Cliente con mayor puja en el tiempo T-1 (no implica que sea unico)
	int pujaMasAlta;	//Puja mas alta recibida
	int pujaMinima;	//Siguiente oferta
	int numPujas;	//Numero de pujas recibidas en el tiempo T
	int numPujasSend;	//Numero de pujas recibidas en el tiempo T-1
	bool primero;		//Primero que se recibe puja (gana ante empate)
	bool aceptandoPujas;	//True si y solo si la subasta actual se consieda en curso
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
	//Añade un nuevo pujador a la subasta
	void anyadirPujador();
	//Elimina un pujador de la subasta 
	void quitarPujador();
	//Devuelve el número de pujadores que están pujando
	int numPujadores();
	//Si hay pujadores suficientes, comienza una subasta
	bool comenzarSubastas();
	//
	int nMensaje();
	//
	bool numMenAceptado(int numero);
	//Cierra la subasta
	void CerrarSocket();
	void noAceptarPujas();
	bool separaMensaje(string In, string &mensaje);
	void forzarCierre();
};

#endif 