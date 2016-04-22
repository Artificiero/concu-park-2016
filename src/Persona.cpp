#include "Persona.h"

Persona::Persona(unsigned int presupuesto,std::string nombre,Puerta& puertaE,Puerta& PuertaS,CajaCentral& cajaC): presupuesto(presupuesto),nombre(nombre),puertaEntrada(puertaE),puertaSalida(PuertaS),cajaCentral(cajaC)
{
    //ctor
}

Persona::~Persona()
{
    //dtor
}

int Persona::vivir() {

    std::cout << this->nombre << ":"<< "Voy a entrar al parque" << std::endl;
    this->puertaEntrada.cruzar();
    std::cout << this->nombre << ":"<< "Entre!" << std::endl;

    this->presupuesto = this->presupuesto - 5;
    this->cajaCentral.ingresarDinero(5);

    std::cout << this->nombre << ":"<< "Voy a salir del parque" << std::endl;
    this->puertaSalida.cruzar();
    std::cout << this->nombre << ":"<< "sali!" << std::endl;

    return 0;
}
