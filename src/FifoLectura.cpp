#include "FifoLectura.h"

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
    std::cout << "fifo lectura destruido" << fd << std::endl;
}

void FifoLectura::abrir() {
    std::cout << "fifo lectura intenta abrir " << nombre << std::endl;
	fd = open ( nombre.c_str(),O_RDONLY );
	std::cout << "fifo lectura creado " << fd << std::endl;
}

ssize_t FifoLectura::leer(void* buffer,const ssize_t buffsize) const {

    ssize_t result = read ( fd,buffer,buffsize );
    std::cout << "fifolectura - result: " << result << std::endl;
	return result;
}
