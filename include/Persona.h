#ifndef PERSONA_H
#define PERSONA_H

#include "Puerta.h"
#include "CajaCentral.h"
#include <list>
#include "FolletoJuego.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include "FifoEscritura.h"
#include "FifoLectura.h"
#include "Logger.h"
#include <sstream>

class Persona
{
    public:
        Persona(unsigned int presupuesto,std::string nombre,Puerta &puertaEntrada,Puerta &PuertaSalida,CajaCentral &cajaCentral,std::list<FolletoJuego> cartillaDeJuegos);
        virtual ~Persona();

        int vivir();

        int getPlataRestante() const {
            return this->presupuesto;
            }

        int ponerseEnColaDeJuego(std::string nombreDelJuego);

    protected:
    private:

    int presupuesto;
    std::string nombre;

    Puerta puertaEntrada;
    Puerta puertaSalida;

    CajaCentral cajaCentral;

    std::list<FolletoJuego> cartillaDeJuegos;
};

#endif // PERSONA_H
