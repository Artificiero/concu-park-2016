#include <iostream>
#include <stdlib.h>
#include "LockFile.h"
#include "Puerta.h"
#include "MemoriaCompartida2.h"

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

int main () {

    return pruebaPuerta();
    //return pruebaLock();

    return 0;
}
