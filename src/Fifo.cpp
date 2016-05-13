#include "Fifo.h"

Fifo::Fifo(const std::string nombre) : nombre(nombre), fd(-1) {

	int result = mknod ( static_cast<const char*>(nombre.c_str()),S_IFIFO|0666,0 );
	if ( result == 0 )
	{
        std::cout << nombre << " " << "CREADO correctamente:" << result << std::endl;
	} else
	{
        perror("mknod() error");
	}
}

Fifo::~Fifo() {
}

void Fifo::cerrar() {

	int result = close ( fd );
	if ( result == 0 )
	{
        std::cout << "fifo " << nombre << " " << "CERRADO correctamente" << std::endl;
	} else
	{
        perror("mknod() error");
	}
	fd = -1;
}

void Fifo::eliminar() const {

	int result = unlink ( nombre.c_str() );
    if ( result == 0 )
	{
        std::cout << "fifo "<< nombre << " " << "eliminado correctamente" << std::endl;
	} else
	{
        perror("mknod() error");
	}
}
