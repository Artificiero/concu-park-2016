#include "CajaCentral.h"

CajaCentral::CajaCentral(const std::string nombre): lock(nombre),plata(nombre,'A')
{
    this->nombre = nombre;
}

int CajaCentral::ingresarDinero(int billetes) {

    this->lock.tomarLock();
    //la memora compartida se sincroniza via los locks
    int aux = this->plata.leer();
    aux = aux + billetes;
    this->plata.escribir(aux);
    this->lock.liberarLock();
    return aux;
}

int CajaCentral::getCantidadDeDineroAlmacenada() {
    this->lock.tomarLock();
    int plata = this->plata.leer();
    this->lock.liberarLock();
    return plata;
}

CajaCentral::~CajaCentral()
{
    //dtor
}
