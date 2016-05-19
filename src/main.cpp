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
using namespace std;

#include <fstream>

int probarFifos(){

	static const int BUFFSIZE = 100;
	static const std::string ARCHIVO_JUEGO = "/tmp/archivo_fifo";

    int numeroDeHijo=0;
    pid_t processId;
    int cantidadDeHijos=10;

    for(int i=0;i<cantidadDeHijos;i++) {
        processId = fork() ;
        if ( processId == 0 ) {
            numeroDeHijo = i;
            std::cout << "Child reporting in: " << numeroDeHijo << std::endl;
            break;
        }
    }

	if ( processId == 0 ) {

        std::stringstream ss;
        ss << numeroDeHijo;
        std::string sonName = ss.str();

        char buff[sizeof(int)+1];
        std::memset(buff, 0, sizeof(buff) );
        strncpy(buff, sonName.c_str(), sizeof(buff));
        buff[sizeof(buff) - 1] = 0;

		FifoEscritura canalJuego ( ARCHIVO_JUEGO );
		canalJuego.abrir();
		std::cout << sonName.c_str() << " :intento entrar al juego(escribir en el fifo):"<< ARCHIVO_JUEGO << std::endl;
		canalJuego.escribir ( static_cast<const void*>(buff),sizeof buff );
        std::cout << sonName << " logre comunicarme con el juego, mande " << sonName.length() << " bytes"<< std::endl;

        static const std::string lecturaPropiaHijo = "/tmp/archivo_fifo" + sonName;

		FifoLectura canalLeerPropioHijo (lecturaPropiaHijo);
		std::cout << sonName << " intento abrir canal propio"<< lecturaPropiaHijo <<  std::endl;
		canalLeerPropioHijo.abrir();
		std::cout << sonName << " canal propio abierto"<< std::endl;

		char buffer[BUFFSIZE];

		std::cout << sonName <<" [Lector] esperando que me libere el juego" << std::endl;
		ssize_t bytesLeidos = canalLeerPropioHijo.leer(static_cast<void*>(buffer),BUFFSIZE);
		std::string mensaje = buffer;
		mensaje.resize ( bytesLeidos );
		std::cout << sonName << " [Lector] Lei el dato del fifo: " << mensaje << std::endl;

		canalLeerPropioHijo.cerrar();
		canalLeerPropioHijo.eliminar();
		std::cout << sonName << " [Lector] Fin del proceso" << std::endl;
		exit ( 0 );
	} else {

		FifoLectura canalJuego ( ARCHIVO_JUEGO );
		canalJuego.abrir();

		int contador=0;
        char buffer[sizeof(int)+1];
		while (contador<cantidadDeHijos) {
            canalJuego.leer(static_cast<void*>(buffer),sizeof(buffer));
            contador++;
            std::cout<< "juego: "<< "leido por el padre en buffer: "<< buffer << std::endl;
		}

		std::cout << "juego: "<< "********se leyeron todos los hijos**********" << std::endl;
		sleep(5);

        for(int i=0;i<cantidadDeHijos;i++) {
            sleep(3);
            std::stringstream ss;
            ss << i;
            std::string mensaje = ss.str();

            std::string lecturaPropiaHijo = "/tmp/archivo_fifo" + mensaje;
            std::cout << i << ")juego: " << lecturaPropiaHijo << " "<< mensaje << std::endl;
            FifoEscritura canalHijo (lecturaPropiaHijo);
            canalHijo.abrir();
            std::cout << "juego: " << "se abrio canal" << lecturaPropiaHijo << std::endl;
            canalHijo.escribir ( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
            std::cout << "juego: "<< "Escribi el mensaje " << mensaje << " en el fifo " << lecturaPropiaHijo << std::endl;
            canalHijo.cerrar();
        }
		wait(NULL);
        std::cout << "juego: "<< "se cerro canal de lectura" << std::endl;
        canalJuego.cerrar ();
		canalJuego.eliminar ();
		std::cout << "juego: "<< "Fin del proceso" << std::endl;
		exit ( 0 );
	}
}

int probarJuegosPersonas(int cantidadDeJuegos,int cantidadPersonas) {

    Puerta      pe("puertaEntrada");
    Puerta      ps("puertaSalida");
    CajaCentral cc("cajaCentral");

    int numeroDeJuego=0;
    pid_t processId;
    std::list<FolletoJuego> cartillaJuegos;
    std::list<pid_t> procesosCreados;

    for(int i=0;i<cantidadDeJuegos;i++) {
        processId = fork() ;
        if ( processId == 0 ) {
            numeroDeJuego = i;
            std::cout << "Juego reporting in: " << numeroDeJuego << std::endl;
            break;
        } else {
            //esto se guarda en el padre
            std::stringstream ss;
            ss << numeroDeJuego;
            std::string nombreJuego = ss.str();

            FolletoJuego fj(1,nombreJuego);
            cartillaJuegos.push_back(fj);
            procesosCreados.push_back(processId);
        }
    }

	if ( processId == 0 ) {
        //estoy en un proceso HIJO, donde voy a crear el juego
        //cada proceso hijo tiene un NUMERODEHIJO al salir del fork
        std::stringstream ss;
        ss << numeroDeJuego;
        std::string nombreJuego = ss.str();
        Juego juego(nombreJuego,1,2,1);

        //aca dentro esta la inteligencia del juego y el loop
        juego.iniciar();

	} else {
        //proceso principal padre
        //ordenado de menor a mayor
        sleep(5);

//        std::list<FifoEscritura*> fifos;
//
//        for(int i=0;i<cantidadDeJuegos;i++){
//
//            std::stringstream ss;
//            ss << i;
//            std::string nombreJuego = ss.str();
//
//            const std::string ARCHIVO_JUEGO = "/tmp/archivo_fifo_juego_" + nombreJuego;
//
//            FifoEscritura* canalJuego = new FifoEscritura( ARCHIVO_JUEGO );
//            fifos.push_front( canalJuego );
//            canalJuego->abrir();
//        }

        cartillaJuegos.sort();

        int numeroDePersona=0;

        for(int i=0;i<cantidadPersonas;i++) {
            processId = fork() ;
            if ( processId == 0 ) {
                numeroDePersona = i;
                std::cout << "Persona reporting in: " << numeroDePersona << std::endl;
                break;
            } else {
                //el padre actualiza esto
                procesosCreados.push_back(processId);
            }
        }

        if ( processId == 0 ) {
            //estoy en un proceso HIJO, donde voy a crear las personas
            //cada proceso hijo tiene un NUMERODEHIJO al salir del fork
            std::stringstream ss;
            ss << numeroDePersona;
            std::string nombrePersona = ss.str();
            Persona persona(1,nombrePersona,pe,ps,cc,cartillaJuegos);
            //aca dentro esta la inteligencia del juego y el loop
            persona.vivir();
        } else {
            for(unsigned int i=0;i<procesosCreados.size();i++){
                wait(NULL);
            }
//            for (std::list<FifoEscritura*>::iterator it=fifos.begin(); it!=fifos.end(); ++it)
//            {
//                delete *it;
//            }
        }
	}

    return 0;
}

int crearJuegos( std::list<FolletoJuego>& cartillaJuegos,std::list<pid_t>& procesosCreados) {

    pid_t processId;

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
        procesosCreados.push_back(processId);
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
    return probarJuegosPersonas(1,20);

    return 0;
}
