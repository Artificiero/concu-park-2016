#ifndef CAJACENTRAL_H
#define CAJACENTRAL_H

#include "LockFile.h"
#include "MemoriaCompartida2.h"
#include <sstream>

class CajaCentral
{
    public:
        CajaCentral();
        virtual ~CajaCentral();

        int ingresarDinero(int billetes);
        int getCantidadDeDineroAlmacenada();

    protected:
    private:
    std::string nombre;
    LockFile lock;
    MemoriaCompartida2<int> plata;
};

#endif // CAJACENTRAL_H
