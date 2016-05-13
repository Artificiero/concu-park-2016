#include "LockFile.h"

LockFile :: LockFile ( const std::string nombre ) {

	this->nombre = nombre;
	this->fl.l_type = F_WRLCK; //lock de escritura
	this->fl.l_whence = SEEK_SET; //principio del file
	this->fl.l_start = 0; //offset
	this->fl.l_len = 0; // longitud 0 == EOF

	int fdAux = open ( this->nombre.c_str(),O_CREAT|O_WRONLY,0777 ); //fd abierto para escritura, crea file si no existe
	if (fdAux >= 0) {
		this->fd = fdAux;
	} else {
        std::cout << "!!!PANICO!!! No se pudo crear el file para el lock: " << nombre << std::endl;
	}
}

int LockFile :: tomarLock () {
	this->fl.l_type = F_WRLCK; //lock de escritura
	//F_SETLKW == Si ya existe el lock sobre el file, espera hasta que se libere
	int fcntlReturnValue = fcntl ( this->fd,F_SETLKW,&(this->fl) );
	if ( fcntlReturnValue < 0 ) {
        std::cout << "!!!PANICO!!! fallo adquirir el lock" << fcntlReturnValue << " " << this->nombre << std::endl;
    }
	return fcntlReturnValue;
}

int LockFile :: liberarLock () {
	this->fl.l_type = F_UNLCK; //setea el tipo para liberar
	//fcntl Acquire a lock (when l_type is F_RDLCK or F_WRLCK) or release a lock (when l_type is F_UNLCK)
    int fcntlReturnValue = fcntl ( this->fd,F_SETLK,&(this->fl) );
    if ( fcntlReturnValue < 0 ) {
        std::cout << "!!!PANICO!!! fallo liberar el lock" << errno << " " << this->nombre << std::endl;
    }
	return fcntlReturnValue;
}

ssize_t LockFile :: escribir ( const void* buffer,const ssize_t buffsize ) const {
	lseek ( this->fd,0,SEEK_END );
	return write ( this->fd,buffer,buffsize );
}

LockFile :: ~LockFile () {
	close ( this->fd );
}
