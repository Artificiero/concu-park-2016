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
#include <stdlib.h>
#include <sys/wait.h>
#include <cstring>

using namespace std;

int pruebaPuerta() {

    int numeroDeHijo;
    pid_t processId;

    int cantidadDeHijos=50;

    Puerta puerta("puerta");

    for(int i=0;i<cantidadDeHijos;i++) {
        processId = fork() ;
        if ( processId == 0 ) {
            numeroDeHijo = i;
            std::cout << "Child reporting in:" << numeroDeHijo << std::endl;
            break;
        }
    }

    if ( processId != 0 ) {
        std::cout << "en este momento el padre tiene "<< cantidadDeHijos << " hijos" << std::endl;
        puerta.cruzar();
        sleep(5);
        std::cout << "gente que cruzo la puerta(incluye al padre): "<< puerta.getCantidadDeGenteQueCruzo() << std::endl;
        std::cout << "dad says goodbye!" << std::endl;
    } else {
        puerta.cruzar();
        std::cout << "Child says goodbye papa!" << numeroDeHijo << std::endl;
    }
    return 0;

}

int pruebaLock() {

    int numeroDeHijo;
    pid_t processId;
    int cantidadDeHijos=25;

    LockFile lock("lockxxxx");

    for(int i=0;i<cantidadDeHijos;i++) {
        processId = fork() ;
        if ( processId == 0 ) {
            numeroDeHijo = i;
            std::cout << "Child reporting in:" << numeroDeHijo << std::endl;
            break;
        }
    }

    if ( processId != 0 ) {
        std::cout << "en este momento el padre tiene "<< cantidadDeHijos << " hijos" << std::endl;
        lock.tomarLock();
        sleep(10);
        lock.liberarLock();
        std::cout << "dad says goodbye!" << std::endl;
    } else {
        sleep(2);
        lock.tomarLock();
        std::cout << "Child says goodbye papa! " << numeroDeHijo << std::endl;
        sleep(1);
        lock.liberarLock();
    }
    return 0;
}

int probarPersonasViviendo() {

    Puerta      pe("puertaEntrada");
    Puerta      ps("puertaSalida");
    CajaCentral cc("cajaCentral");

    int numeroDeHijo;
    pid_t processId;
    int cantidadDeHijos=25;

    for(int i=0;i<cantidadDeHijos;i++) {
        processId = fork() ;
        if ( processId == 0 ) {
            numeroDeHijo = i;
            std::cout << "procesoHijo:" << numeroDeHijo << std::endl;
            break;
        }
    }

    if ( processId != 0 ) {
        std::cout << "en este momento el padre tiene "<< cantidadDeHijos << " hijos" << std::endl;
        sleep(10);
    } else {

        ostringstream ss;
        ss << numeroDeHijo;
        string str = ss.str();

        const char* c = str.c_str();

        Persona persona( 100, c ,pe,ps,cc);
        persona.vivir();

        return 0;
    }

    cout << "plata en la caja: " << cc.getCantidadDeDineroAlmacenada() << endl;


    return 0;

}

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

int main () {

    //return pruebaPuerta();
    //return pruebaLock();
    //return probarPersonasViviendo();
    return probarFifos();

    return 0;
}
