#include "CajaCentral.h"

CajaCentral::CajaCentral():nombre("cajaCentral"),lock("cajaCentral"),plata("cajaCentral",'A')
{
}

int CajaCentral::ingresarDinero(int billetes) {

    LockFile lock(this->nombre);

    lock.tomarLock();
    //la memora compartida se sincroniza via los locks
    int aux = this->plata.leer();
    aux = aux + billetes;
    this->plata.escribir(aux);

    lock.liberarLock();
    return aux;
}

int CajaCentral::getCantidadDeDineroAlmacenada() {

    LockFile lock(this->nombre);

    lock.tomarLock();
    int plata = this->plata.leer();
    lock.liberarLock();

    return plata;
}

CajaCentral::~CajaCentral()
{
    //dtor
}
