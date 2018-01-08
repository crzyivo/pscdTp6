#include "mSubasta.hpp"

monitorSubasta::monitorSubasta(){
	this->pujadorMasAlto = -1; //-1 = no hay pujador
	this->posibleGanador = -1;
	this->pujaMasAlta = 0;
	this->pujaMinima = 0;
	this->numPujas = 0;
	this->numPujasSend = 0;
	this->fin_Subastas = false;
}
monitorSubasta::~monitorSubasta(){
	
}
//devuelve el precio inicial marcado de manera que todos los procesos
// comiencen la subasta al mismo tiempo
int monitorSubasta::comenzarSubasta(){
	unique_lock<mutex> lck(this->exclusion);
	this->cv.wait(lck);
	return pujaMinima;
}
//reactualiza la proxima minima puja que se aceptara
void monitorSubasta::pujar(string mensaje, int cliente){
	unique_lock<mutex> lck(this->exclusion);
	if(mensaje == Accepto){	
		this->numPujas++;
		this->pujadorMasAlto = cliente;
	}
}

bool monitorSubasta::SubastaAceptada(){
	return this->pujaMasAlta >= this->precioRequerido;
}
//Devuelve el cardinal de pujadores de la ronda actual
int monitorSubasta::nPujas(){
	unique_lock<mutex> lck(this->exclusion);
	this->cv.wait(lck);
	return this->numPujasSend;
}

//devuelve la acual puja
int monitorSubasta::pujaActual(){
	unique_lock<mutex> lck(this->exclusion);
	return this->pujaMinima;
}
//devuelve el socket del actual cliente con una puja mayor
int monitorSubasta::PujadorActual(){
	unique_lock<mutex> lck(this->exclusion);
	return this->posibleGanador;
}
//Guarda la imagen actual de la subasta en otras variables y avisa que pueden enviar nuevo precio
void monitorSubasta::enviarPuja(){
	unique_lock<mutex> lck(this->exclusion);
	this->numPujasSend = this->numPujas;
	this->numPujas = 0;
	this->pujaMinima = this->pujaMasAlta;
	this->posibleGanador = this->pujadorMasAlto;
	this->cv.notify_all();
}
//devuelve true si y solo si se pueden conectar clientes
bool monitorSubasta::SalonAbierto(){
	unique_lock<mutex> lck(this->exclusion);
	return !fin_Subastas;
}
//Cierra todas las posibles subastas nuevas
bool monitorSubasta::CerrarSalon(){
	unique_lock<mutex> lck(this->exclusion);
	this->fin_Subastas = true;
}
//Despierta el proceso para que pueda cerrar el socket
void monitorSubasta::finSubasta(){
	unique_lock<mutex> lck(this->exclusion);
}
