#include "Juego.h"

Juego::Juego(std::string nombre, int tiempoDeJuego, int cantNecesariasParaArrancar):nombre(nombre),tiempoDeJuego(tiempoDeJuego),cantidadDePersonasEnLaCola(0),cantidadDePersonasNecesariasParaArrancar(cantNecesariasParaArrancar),cola(nombre)
{

}

Juego::~Juego()
{

}

int Juego::iniciar()
{

    //voy a usar este fifo para recibir los nombres de las personas que quieren subir
    this->cola = FifoLectura(nombre);
    this->cola.abrir();

    while(true)
    {

        //cada persona que entra me "despierta", y me hace checkear la condicion
        while( this->cantidadDePersonasEnLaCola < this->cantidadDePersonasNecesariasParaArrancar )
        {
            char buffer[sizeof(int)+1];
            std::memset(buffer, 0, sizeof(buffer) );
            buffer[sizeof(buffer) - 1] = 0;

            //las personas ENTRAN DE A 1
            //me bloqueo hasta que alguien quiere subir al juego
            this->cola.leer(static_cast<void*>(buffer),sizeof(buffer));

            //aumento cantidad de gente en la cola al entrar alguien, me libero y checkeo condicion
            this->nombresPersonas.push_front(std::string(buffer));
            this->cantidadDePersonasEnLaCola++;
        }
        //cuando hay la SUFICIENTE CANTIDAD DE PERSONAS, arranco.
        //Las demas personas ,que estan esperando para entrar y todavia no entraron,
        //se quedan bloquedas en el write al fifo COLA del juego

        //simulo la vuelta del juego
        this->arrancarUnaPasada();
        //las personas que "subieron" al juego, todavia estan esperando que las liberen

        //las personas se bajan del juego DE A UNA
        for (std::list<std::string>::iterator it=this->nombresPersonas.begin(); it!=this->nombresPersonas.end(); ++it)
        {
                std::string nomPer = *it;
                std::string mensaje = "Fin del Juego" + this->nombre + " para " + nomPer;

                //se que el fifo se llama asi por CONVENCION
                std::string nombreCanalDeUnaPersona = "/tmp/archivo_fifo/persona_" + nomPer;
                std::cout << this->nombre << " : " << nombreCanalDeUnaPersona << std::endl;

                FifoEscritura canalPersona (nombreCanalDeUnaPersona);
                canalPersona.abrir();
                std::cout << this->nombre << " : " << "se abrio canal " << nombreCanalDeUnaPersona << std::endl;

                //la persona esta bloqueada con una lectura a su propio fifo, esperando que el juego escriba
                canalPersona.escribir ( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
                std::cout << this->nombre << " : " << "Escribi en el fifo " << nombreCanalDeUnaPersona << std::endl;
                canalPersona.cerrar();
        }

        //limpio la lista de los nombres de las personas que estaban en el juego, para su proxima pasada
        this->nombresPersonas.clear();

        //vuelvo a empezar
    }

    this->cola.cerrar();
    this->cola.eliminar();

    return 0;
}

int Juego::arrancarUnaPasada()
{
    sleep(this->tiempoDeJuego);
    return 0;
}
