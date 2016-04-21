#include "Puerta.h"

Puerta::Puerta(const std::string nombre): lock(nombre),cantidadCruzaron(nombre,'A')
{
    this->nombreDeLaPuerta = nombre;
}

void Puerta::cruzar(){

    this->lock.tomarLock();

    //la memora compartida se sincroniza via los locks
    int aux = this->cantidadCruzaron.leer();
    aux = aux + 1;
    this->cantidadCruzaron.escribir(aux);

    this->lock.liberarLock();
}

std::string Puerta::getNombrePuerta() {
    return this->nombreDeLaPuerta;
}

int Puerta::getCantidadDeGenteQueCruzo() {

    this->lock.tomarLock();
    //la memora compartida se sincroniza via los locks
    int aux = this->cantidadCruzaron.leer();
    this->lock.liberarLock();

    return aux;
}

Puerta::~Puerta(){}
