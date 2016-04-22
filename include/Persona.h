#ifndef PERSONA_H
#define PERSONA_H

#include "Puerta.h"
#include "CajaCentral.h"

class Persona
{
    public:
        Persona(unsigned int presupuesto,std::string nombre,Puerta &puertaEntrada,Puerta &PuertaSalida,CajaCentral &cajaCentral);
        virtual ~Persona();

        int vivir();

        int getPlataRestante() {
            return this->presupuesto;
            }


    protected:
    private:

    unsigned int presupuesto;
    std::string nombre;

    Puerta puertaEntrada;
    Puerta puertaSalida;

    CajaCentral cajaCentral;
};

#endif // PERSONA_H
