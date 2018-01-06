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
	unique_lock<mutex> lck(this.mtx);
	return pujaMinima;
}
//reactualiza la proxima minima puja que se aceptara
//devuelve true si y solo si la puja ha sido superada
bool monitorSubasta::pujar(string mensaje, int cliente){
	unique_lock<mutex> lck(this.mtx);
	if(mensaje == Pasa){

	}else{
		int p;
		sscanf("Acepto %d\n", p);
		if(p >= this.pujaMasAlta){
			if(p>this.pujaMasAlta){
				pujaMasAlta = p;
				numPujas = 1;
			}else{
				this.numPujas++;
			}
		}else{
			
		}
	}
}
//Devuelve el cardinal de pujadores de la ronda actual
int monitorSubasta::nPujas(){
	unique_lock<mutex> lck(this.mtx);
	cv.wait(lck);
	return this-> numPujasSend;
}

//devuelve la acual puja
int monitorSubasta::pujaActual(){
	unique_lock<mutex> lck(this.mtx);
}
//devuelve el socket del actual cliente con una puja mayor
int monitorSubasta::PujadorActual(){
	unique_lock<mutex> lck(this.mtx);
	return this.PujadorActual;
}
void monitorSubasta::enviarPuja(){
	unique_lock<mutex> lck(this.mtx);
	this.numPujasSend = this.numPujas;
	this.numPujas = 0;
	cv.notify_all();
}
//devuelve true si y solo si se pueden conectar clientes
bool monitorSubasta::SalonAbierto(){
	unique_lock<mutex> lck(this.mtx);
	return !fin_Subastas;
}
//Cierra todas las posibles subastas nuevas
bool monitorSubasta::CerrarSalon(){
	unique_lock<mutex> lck(this.mtx);
}
//Despierta el proceso para que pueda cerrar el socket
void monitorSubasta::finSubasta(){
	unique_lock<mutex> lck(this.mtx);
}
