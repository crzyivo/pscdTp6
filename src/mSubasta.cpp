/*
 * Autores: Lafuente, Víctor M.; Escuín González, Iván; Morón Vidal, Elena
 * NIP: 747325; 684146; 739324
 * Fecha: enero 2018
 */

#include "mSubasta.hpp"

monitorSubasta::monitorSubasta(){
	this->numMensaje = 0;
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
//Añade un nuevo pujador a la subasta
void monitorSubasta::anyadirPujador(){
	unique_lock<mutex> lck(this->exclusionDatos);
	this->pujadoresObservando++;
	if(this->pujadoresObservando == 1) this->esperarClientes.notify_all();
}
//Elimina un pujador de la subasta 
void monitorSubasta::quitarPujador(){
	unique_lock<mutex> lck(this->exclusionDatos);
	this->pujadoresObservando--;
        if(this->pujadoresObservando == 0){
            this->cerrarSubasta.notify_all();
        }
}

//Devuelve el número de pujadores que están pujando
int monitorSubasta::numPujadores(){
	unique_lock<mutex> lck(this->exclusionDatos);
	return this->pujadoresObservando;
}
//Inicializa una nueva subasta de tiempoValla 
bool monitorSubasta::iniciarNuevaSubasta(int tiempoValla){
	unique_lock<mutex> lck(this->exclusionDatos);
		this->TiempoAnuncio = tiempoValla;
		this->pujadorMasAlto = -1; //-1 = no hay pujador
		this->posibleGanador = -1;
		this->pujaMasAlta = tiempoValla*CoeficienteTiempo;
		this->precioRequerido = this->pujaMasAlta+CuotaMercado;
		this->pujaMinima = this->pujaMasAlta;
		this->numPujas = 0;
		this->numPujasSend = 0;
		this->numMensaje++;
		this->comenzarS.notify_all();
}
//Devuelve el tiempo que se subastara
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
	if(this->aceptandoPujas){
		return pujaMinima;
	}else{
		return -1;
	}
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
//Devuelve true si y solo si, se ha llegado al minino pedido
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
	this->numMensaje++;
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
	if(!this->aceptandoPujas){
		this->comenzarS.notify_all();
		cerr << "\033[31mNotificando quien Este esperando\033[0m\n";
	}
	this->esperarClientes.notify_all();

	
}
//Despierta el proceso para que pueda cerrar el socket
void monitorSubasta::finSubasta(){
	unique_lock<mutex> lck(this->exclusionDatos);
        while(this->pujadoresObservando>0){
            this->cerrarSubasta.wait(lck);
			this->esperarClientes.notify_all();
        }
}
//Si hay pujadores suficientes, comienza una subasta
bool monitorSubasta::comenzarSubastas(){
	unique_lock<mutex> lck(this->exclusionDatos);
	if(this->pujadoresObservando <1){
		this->esperarClientes.wait(lck);
	}
	if(this->pujadoresObservando >0){ //Si estoy despierto porque hay pujadores
		this->aceptandoPujas = true;
	}
	return this->pujadoresObservando > 0;
}

int monitorSubasta::nMensaje(){
	unique_lock<mutex> lck(this->exclusionDatos);
	return this->numMensaje;
}

bool monitorSubasta::numMenAceptado(int numero){
	unique_lock<mutex> lck(this->exclusionDatos);
	return this->numMensaje <= numero;
	cerr << "\033[31mNotificando quien Este esperando\033[0m\n";
}
//Cierra la subasta
void monitorSubasta::CerrarSocket(){
	unique_lock<mutex> lck(this->exclusionDatos);
	this->cerrarSubasta.notify_all();
}

void monitorSubasta::noAceptarPujas(){
	unique_lock<mutex> lck(this->exclusionDatos);
	cerr << "\033[31mNo se aceptaran mas pujas\033[0m\n";
	this->aceptandoPujas = false;
}

bool monitorSubasta::separaMensaje(string In, string &mensaje){
	unique_lock<mutex> lck(this->exclusionDatos);
	char * mensajesN = strtok( strdup(In.c_str()), ";");
	mensaje = strtok(NULL, ";");
	return this->numMensaje <= atoi(mensajesN);
}