#include <iostream>
#include <stdlib.h>
#include "LockFile.h"
#include "Puerta.h"
#include "MemoriaCompartida2.h"
#include "Persona.h"
#include <sstream>
#include "FifoEscritura.h"
#include "FifoLectura.h"
#include <iostream>
#include <sys/wait.h>
#include <cstring>
#include "Juego.h"
#include <map>
#include "Logger.h"
#include <vector>
#include "AdministradorDelParque.h"

using namespace std;

#include <fstream>

int probarJuegosPersonas(std::vector<Juego>& juegos,std::vector<Persona>& personas ) {

    int numeroDeJuego=0;
    pid_t processId;
    std::list<pid_t> procesosCreados;

        for(std::vector<Juego>::iterator it=juegos.begin(); it!=juegos.end(); ++it)
        {
            processId = fork() ;
            if ( processId == 0 ) {
                numeroDeJuego = atoi( ((*it).getNombre()).c_str() ) ;
                std::cout << "Juego reporting in: " << numeroDeJuego << std::endl;
                break;
            } else {
                procesosCreados.push_back(processId);
            }
        }

	if ( processId == 0 ) {
        //estoy en un proceso HIJO
        //aca dentro esta la inteligencia del juego y el loop
        std::cout << "apunto de iniciar juego" << std::endl;
        Juego juego = juegos[numeroDeJuego];
        std::cout << juego.getNombre() << std::endl;
        juego.iniciar();

	} else {
        //proceso principal padre
        //ordenado de menor a mayor
        sleep(3);
        std::cout << "proceso padre continua" << std::endl;

        int numeroDePersona=0;

        for(std::vector<Persona>::iterator it=personas.begin(); it!=personas.end(); ++it)
        {
            processId = fork() ;
            if ( processId == 0 ) {
                numeroDePersona = atoi( ((*it).getNombre()).c_str() ) ;
                std::cout << "Persona reporting in: " << numeroDePersona << std::endl;
                break;
            } else {
                procesosCreados.push_back(processId);
            }
        }

        if ( processId == 0 ) {
            //estoy en un proceso HIJO
            //cada proceso hijo tiene un NUMERODEHIJO al salir del fork

            //aca dentro esta la inteligencia del juego y el loop
            Persona persona = personas[numeroDePersona];
            persona.vivir();
        } else {

            processId = fork() ;

            if ( processId == 0 )
            {
                AdministradorDelParque admin;
                admin.vivir();
            }
            else
            {
                //padre espera
                for(unsigned int i=0;i<procesosCreados.size();i++){
                    wait(NULL);
                }
            }
        }
	}

    return 0;
}

int crearJuegos( std::list<FolletoJuego>& cartillaJuegos,std::vector<Juego>& juegos) {

    //nombre tiempoDeJuego cantNecesariasParaArrancar precio
    std::ifstream juegosFile("Juegos.txt");

    std::string nombreJuego;
    int tiempoDeJuego;
    int cantNecesariasParaArrancar;
    int precio;

    while (juegosFile >> nombreJuego >> tiempoDeJuego >> cantNecesariasParaArrancar >> precio)
    {
        std::cout << "Proceso principal) lei:"<<std::endl;
        std::cout << nombreJuego <<" "<< tiempoDeJuego <<" "<< cantNecesariasParaArrancar <<" "<< precio <<" "<< std::endl;

        //esto se guarda en el padre
        FolletoJuego fj(precio,nombreJuego);
        cartillaJuegos.push_back(fj);
        Juego juego(nombreJuego,tiempoDeJuego,cantNecesariasParaArrancar,precio);
        juegos.push_back(juego);
    }

    return 0;
}

int crearPersonas( std::vector<Persona>& personas,Puerta& puertaEntrada,Puerta& PuertaSalida,std::list<FolletoJuego> fj) {

    //nombre tiempoDeJuego cantNecesariasParaArrancar precio
    std::ifstream personasFile("Personas.txt");

    std::string nombre;
    unsigned int presupuesto;

    while ( personasFile >> nombre >> presupuesto)
    {
        std::cout << "Proceso principal) lei:"<< std::endl;
        std::cout << nombre << " " << presupuesto << std::endl;

        Persona persona(presupuesto,nombre,puertaEntrada,puertaEntrada,fj);
        personas.push_back(persona);
    }

    return 0;
}

int probarLogger(int cantLoggers)
{
    pid_t processId;

    for(int i=0;i<cantLoggers;i++)
    {
        processId = fork() ;
        if ( processId == 0 )
        {
            Logger logger;
            std::stringstream ss;
            ss << i;
            std::string ii = ss.str();

            for(int i=0;i<20;i++)
            {
                logger.l(ii,"xxxxx","WARNING");
            }
            break;
        }
    }

    if (processId != 0)
    {
        for(int i=0;i<cantLoggers;i++)
        {
            wait(NULL);
        }
        Logger logger;
        logger.l("procesoPadre","joine a todos mis hijos","EXITO");
    }

    return 0;
}

int main () {

    Puerta puertaEntrada("entrada");
    Puerta PuertaSalida("salida");
    CajaCentral cj;

    std::list<FolletoJuego> cartillaJuegos;
    std::vector<Juego> juegos;
    std::vector<Persona> personas;

    crearJuegos(cartillaJuegos,juegos);
    cartillaJuegos.sort();
    crearPersonas(personas,puertaEntrada,PuertaSalida,cartillaJuegos);
    std::cout << "cantidad de personas creadas:"<< personas.size() << std::endl;
    std::cout << "cantidad de juegos creados:"<< juegos.size() << std::endl;

    return probarJuegosPersonas(juegos,personas);

    return 0;
}
