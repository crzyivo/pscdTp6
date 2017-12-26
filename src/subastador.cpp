#include <iostream>
#include <thread>
#include "Socket.hpp"

int puertoSubasta = 32005;

int main(int argc, char * argv[]){
	
	Socket subasta(puertoSubasta);
	
	int sockSubasta = subasta.Bind();
	int error = subasta.Listen();

	while(/**/){
		int cliente = subasta.Accept();
		thread th(&Cliente, subasta);

	}
	
	return 0;
}