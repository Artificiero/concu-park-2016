#include "Juego.h"

Juego::Juego(std::string nombre, int tiempoDeJuego, int cantNecesariasParaArrancar,int precio):nombre(nombre),tiempoDeJuego(tiempoDeJuego),cantidadDePersonasEnLaCola(0)
                                                                                                ,cantidadDePersonasNecesariasParaArrancar(cantNecesariasParaArrancar),precioParaSubir(precio),cola("/tmp/archivo_fifo_juego_" + nombre)
{

}

Juego::~Juego()
{
}

int Juego::iniciar()
{

    //voy a usar este fifo para recibir los nombres de las personas que quieren subir
    this->cola.abrir();

    while(true)
    {
        std::cout << "Juego)"<< this->nombre << "nueva iteracion" << std::endl;
        //cada persona que entra me "despierta", y me hace checkear la condicion
        while( this->cantidadDePersonasEnLaCola < this->cantidadDePersonasNecesariasParaArrancar )
        {
            char buffer[sizeof(int)];
            std::memset(buffer, 0, sizeof(buffer) );
            //buffer[sizeof(buffer) - 1] = 0;

            //las personas ENTRAN DE A 1
            //me bloqueo hasta que alguien quiere subir al juego
            ssize_t bytesLeidos = this->cola.leer(static_cast<void*>(buffer),sizeof(buffer));
            std::cout << "Juego)"<< this->nombre << " leyo "<< buffer << " peso: "<< bytesLeidos << std::endl;

            //aumento cantidad de gente en la cola al entrar alguien, me libero y checkeo condicion
            this->nombresPersonas.push_front(std::string(buffer));
            this->cantidadDePersonasEnLaCola++;
        }
        std::cout << "Juego)"<< this->nombre << "hay suficientes personas para correr" << std::endl;
        //cuando hay la SUFICIENTE CANTIDAD DE PERSONAS, arranco.
        //Las demas personas ,que estan esperando para entrar y todavia no entraron,
        //se quedan bloquedas en el write al fifo COLA del juego

        //simulo la vuelta del juego
        this->arrancarUnaPasada();
        //las personas que "subieron" al juego, todavia estan esperando que las liberen

        int i=0;
        //las personas se bajan del juego DE A UNA
        std::cout << "personas en la lista" << this->nombresPersonas.size() << std::endl;
        for (std::list<std::string>::iterator it=this->nombresPersonas.begin(); it!=this->nombresPersonas.end(); ++it)
        {
                std::string nomPer = *it;
                std::string mensaje = "Fin del Juego" + this->nombre + " para persona " + nomPer;

                //se que el fifo se llama asi por CONVENCION
                std::string nombreCanalDeUnaPersona = "/tmp/archivo_fifo_persona_" + nomPer;
                std::cout << "iteracion:" << i << " Juego)" << this->nombre << " : " << nombreCanalDeUnaPersona << std::endl;

                FifoEscritura canalPersona (nombreCanalDeUnaPersona);
                canalPersona.abrir();
                std::cout << "Juego)"<< this->nombre << " : " << "se abrio canal " << nombreCanalDeUnaPersona << std::endl;

                //la persona esta bloqueada con una lectura a su propio fifo, esperando que el juego escriba
                canalPersona.escribir ( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
                std::cout << "Juego)"<< this->nombre << " : " << "Escribi en el fifo " << nombreCanalDeUnaPersona << std::endl;
                canalPersona.cerrar();
                i++;
        }

        //limpio la lista de los nombres de las personas que estaban en el juego, para su proxima pasada
        this->nombresPersonas.clear();
        this->cantidadDePersonasEnLaCola=0;

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
