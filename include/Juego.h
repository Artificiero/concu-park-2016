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
#include "Logger.h"
#include <sstream>

class Juego
{
    public:
        Juego(std::string nombre, int tiempoDeJuego, int cantidadDePersonasNecesariasParaArrancar,int precioParaSubir);
        int iniciar();
        int arrancarUnaPasada();
        virtual ~Juego();
        std::string getNombre(){return this->nombre;}
    protected:
    private:
    std::string nombre;
    int tiempoDeJuego;
    int cantidadDePersonasEnLaCola;
    int cantidadDePersonasNecesariasParaArrancar;
    int precioParaSubir;

    std::list<std::string> nombresPersonas;

};

#endif // JUEGO_H
