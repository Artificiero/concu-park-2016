#ifndef PUERTA_H
#define PUERTA_H

#include <iostream>
#include "LockFile.h"
#include "MemoriaCompartida2.h"

class Puerta
{
    public:
        //Permite compartir una puerta entre procesos, si el nombre es el mismo
        Puerta(const std::string nombre);
        virtual ~Puerta();

        std::string getNombrePuerta();

        //solo 1 persona puede cruzar una puerta por ves
        //si la puerta esta ocupada, esa persona va a esperar
        void cruzar();

        int getCantidadDeGenteQueCruzo();

    protected:

    private:
    LockFile lock;
    std::string nombreDeLaPuerta;
    MemoriaCompartida2<int> cantidadCruzaron;

};

#endif // PUERTA_H
