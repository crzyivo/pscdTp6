#include "mSubasta.hpp"

monitorSubasta::monitorSubasta(){
	this->pujadoresObservando = 0;
	this->pujadorMasAlto = -1; //-1 = no hay pujador
	this->posibleGanador = -1;
	this->pujaMasAlta = 0;
	this->pujaMinima = 0;
	this->numPujas = 0;
	this->numPujasSend = 0;
	this->aceptandoPujas = false;
	this->fin_Subastas = false;
	this->primero = true;
}
monitorSubasta::~monitorSubasta(){
	
}
void monitorSubasta::anyadirPujador(){
	unique_lock<mutex> lck(this->exclusionDatos);
	this->pujadoresObservando++;
}
void monitorSubasta::quitarPujador(){
	unique_lock<mutex> lck(this->exclusionDatos);
	this->pujadoresObservando--;
}
int monitorSubasta::numPujadores(){
	unique_lock<mutex> lck(this->exclusionDatos);
	return this->pujadoresObservando;
}

bool monitorSubasta::iniciarNuevaSubasta(int tiempoValla){
	unique_lock<mutex> lck(this->exclusionDatos);
	if(!this->fin_Subastas && !this->aceptandoPujas){
		this->TiempoAnuncio = tiempoValla;
		this->pujadorMasAlto = -1; //-1 = no hay pujador
		this->posibleGanador = -1;
		this->pujaMasAlta = tiempoValla*CoeficienteTiempo;
		this->precioRequerido = this->pujaMasAlta+CuotaMercado;
		this->pujaMinima = this->pujaMasAlta;
		this->numPujas = 0;
		this->numPujasSend = 0;
		this->aceptandoPujas = true;
		this->comenzarS.notify_all();
	}
}

int monitorSubasta::tiempoSubas(){
	unique_lock<mutex> lck(this->exclusionDatos);
	if(this->aceptandoPujas){
		return this->TiempoAnuncio;
	}
	return -1;
}
//devuelve el precio inicial marcado de manera que todos los procesos
// comiencen la subasta al mismo tiempo
int monitorSubasta::comenzarSubasta(){
	unique_lock<mutex> lck(this->exclusionDatos);
	this->comenzarS.wait(lck);
	return pujaMinima;
}
//reactualiza la proxima minima puja que se aceptara
void monitorSubasta::pujar(string mensaje, int cliente){
	unique_lock<mutex> lck(this->exclusionDatos);
	if(mensaje == Accepto){	
		this->numPujas++;
		if(this->primero) {
			this->pujadorMasAlto = cliente;
			this->primero = false;
		}
	}
}

bool monitorSubasta::SubastaAceptada(){
	unique_lock<mutex> lck(this->exclusionDatos);
	return this->pujaMasAlta >= this->precioRequerido;
}
//Devuelve el cardinal de pujadores de la ronda actual
int monitorSubasta::nPujas(){
	unique_lock<mutex> lck(this->exclusionDatos);
	this->envioPujas.wait(lck);
	return this->numPujasSend;
}

//devuelve la acual puja
int monitorSubasta::pujaActual(){
	unique_lock<mutex> lck(this->exclusionDatos);
	return this->pujaMinima;
}
//devuelve el socket del actual cliente con una puja mayor
int monitorSubasta::PujadorActual(){
	unique_lock<mutex> lck(this->exclusionDatos);
	return this->posibleGanador;
}
//Guarda la imagen actual de la subasta en otras variables y avisa que pueden enviar nuevo precio
void monitorSubasta::enviarPuja(){
	unique_lock<mutex> lck(this->exclusionDatos);
	this->numPujasSend = this->numPujas;
	this->numPujas = 0;
	if(this->numPujasSend > 0){
		this->pujaMasAlta = this->pujaMinima;
		this->pujaMinima += incrementoPuja;
	}else{
		this->aceptandoPujas = false;
	}
	this->posibleGanador = this->pujadorMasAlto;
	this->primero = true;
	this->envioPujas.notify_all();
}
//devuelve true si y solo si se pueden conectar clientes
bool monitorSubasta::SalonAbierto(){
	unique_lock<mutex> lck(this->exclusionDatos);
	return !this->fin_Subastas;
}
//True si y solo si hay una subasta en curso
bool monitorSubasta::SubastaEnCurso(){
	unique_lock<mutex> lck(this->exclusionDatos);
	return this->aceptandoPujas;
}
//Cierra todas las posibles subastas nuevas
bool monitorSubasta::CerrarSalon(){
	unique_lock<mutex> lck(this->exclusionDatos);
	this->fin_Subastas = true;
}
//Despierta el proceso para que pueda cerrar el socket
void monitorSubasta::finSubasta(){
	unique_lock<mutex> lck(this->exclusionDatos);
}
