#include "FifoLectura.h"

FifoLectura::FifoLectura(const std::string nombre) : Fifo(nombre) {
}

FifoLectura::~FifoLectura() {
}

void FifoLectura::abrir() {
	fd = open ( nombre.c_str(),O_RDONLY );
}

ssize_t FifoLectura::leer(void* buffer,const ssize_t buffsize) const {

    ssize_t result = read ( fd,buffer,buffsize );
    std::cout << "fifolectura - result: " << result << std::endl;
	return result;
}
