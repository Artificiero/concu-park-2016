#include "FifoEscritura.h"

FifoEscritura::FifoEscritura(const std::string nombre) : Fifo(nombre) {
}

FifoEscritura::~FifoEscritura() {
}

void FifoEscritura::abrir() {
	fd = open ( nombre.c_str(),O_WRONLY );
	std::cout << "fd del fifo:" << fd << std::endl;
}

ssize_t FifoEscritura::escribir(const void* buffer,const ssize_t buffsize) const {

	int result = write ( fd,buffer,buffsize );
	std::cout << this->nombre << " " << "resultado del write:" << result << std::endl;
	return result;
}
