#ifndef JUEGO_H
#define JUEGO_H
#include <string>
#include "FifoEscritura.h"
#include "FifoLectura.h"
#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <cstring>
#include <list>

class Juego
{
    public:
        Juego(std::string nombre, int tiempoDeJuego, int cantidadDePersonasNecesariasParaArrancar);
        int iniciar();
        int arrancarUnaPasada();
        virtual ~Juego();
    protected:
    private:
    std::string nombre;
    int tiempoDeJuego;
    int cantidadDePersonasEnLaCola;
    int cantidadDePersonasNecesariasParaArrancar;
    FifoLectura cola;
    std::list<std::string> nombresPersonas;

};

#endif // JUEGO_H
