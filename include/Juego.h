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
        Juego(std::string nombre, int tiempoDeJuego, int cantidadDePersonasNecesariasParaArrancar,int precioParaSubir);
        int iniciar();
        int arrancarUnaPasada();
        virtual ~Juego();
    protected:
    private:
    std::string nombre;
    int tiempoDeJuego;
    int cantidadDePersonasEnLaCola;
    int cantidadDePersonasNecesariasParaArrancar;
    int precioParaSubir;

    //Este es el fifo que las Personas usan para ponerse en la cola del Juego.Convencion de nombres.
    FifoLectura cola;

    std::list<std::string> nombresPersonas;

};

#endif // JUEGO_H
