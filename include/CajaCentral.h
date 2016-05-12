#ifndef CAJACENTRAL_H
#define CAJACENTRAL_H

#include "LockFile.h"
#include "MemoriaCompartida2.h"

class CajaCentral
{
    public:
        CajaCentral(std::string nombre);
        virtual ~CajaCentral();

        int ingresarDinero(int billetes);
        int getCantidadDeDineroAlmacenada();

    protected:
    private:

    LockFile lock;
    std::string nombre;
    MemoriaCompartida2<int> plata;
};

#endif // CAJACENTRAL_H
