#include "Juego.h"

std::string intToString(int num){
            std::stringstream ss;
            ss << num;
            std::string bytesRead = ss.str();
            return bytesRead;
}

Juego::Juego(std::string nombre, int tiempoDeJuego, int cantNecesariasParaArrancar,int precio):nombre(nombre),tiempoDeJuego(tiempoDeJuego),cantidadDePersonasEnLaCola(0)
                                                                                                ,cantidadDePersonasNecesariasParaArrancar(cantNecesariasParaArrancar),precioParaSubir(precio)
{

}

Juego::~Juego()
{
}

int Juego::iniciar()
{
    Logger logger;
    //Este es el fifo que las Personas usan para ponerse en la cola del Juego.Convencion de nombres.
    FifoLectura cola("/tmp/archivo_fifo_juego_" + this->nombre);

    logger.l("Juego",this->nombre,"se inicio");
    //voy a usar este fifo para recibir los nombres de las personas que quieren subir
    cola.abrir();
    logger.l("Juego",this->nombre,"abrio su cola de espera");

    while(true)
    {
        logger.l("Juego",this->nombre,"nueva iteracion");

        //cada persona que entra me "despierta", y me hace checkear la condicion
        while( this->cantidadDePersonasEnLaCola < this->cantidadDePersonasNecesariasParaArrancar )
        {
            char buffer[sizeof(int)];
            std::memset(buffer, 0, sizeof(buffer) );
            //buffer[sizeof(buffer) - 1] = 0;

            //las personas ENTRAN DE A 1
            //me bloqueo hasta que alguien quiere subir al juego
            logger.l("Juego",this->nombre,"busco personas en mi cola");
            ssize_t bytesLeidos = cola.leer(static_cast<void*>(buffer),sizeof(buffer));
            std::string auxMsj;

            logger.l("Juego",this->nombre,std::string("leyo ") + buffer + std::string(" peso: ")+ intToString(bytesLeidos));

            //aumento cantidad de gente en la cola al entrar alguien, me libero y checkeo condicion
            this->nombresPersonas.push_front(std::string(buffer));
            this->cantidadDePersonasEnLaCola++;
        }
        logger.l("Juego",this->nombre,"hay suficientes personas para correr");
        //cuando hay la SUFICIENTE CANTIDAD DE PERSONAS, arranco.
        //Las demas personas ,que estan esperando para entrar y todavia no entraron,
        //se quedan bloquedas en el write al fifo COLA del juego

        //simulo la vuelta del juego
        this->arrancarUnaPasada();
        //las personas que "subieron" al juego, todavia estan esperando que las liberen

        int i=0;
        //las personas se bajan del juego DE A UNA
        logger.l("Juego",this->nombre,std::string("personas en la lista:")+intToString(this->nombresPersonas.size()));

        for (std::list<std::string>::iterator it=this->nombresPersonas.begin(); it!=this->nombresPersonas.end(); ++it)
        {
                std::string nomPer = *it;
                std::string mensaje = "Fin del Juego" + this->nombre + " para persona " + nomPer;

                //se que el fifo se llama asi por CONVENCION
                std::string nombreCanalDeUnaPersona = "/tmp/archivo_fifo_persona_" + nomPer;

                logger.l("Juego",this->nombre,"iteracion "+intToString(i)+nombreCanalDeUnaPersona);

                FifoEscritura canalPersona (nombreCanalDeUnaPersona);
                canalPersona.abrir();

                logger.l("Juego",this->nombre,"se abrio canal "+nombreCanalDeUnaPersona);

                //la persona esta bloqueada con una lectura a su propio fifo, esperando que el juego escriba
                canalPersona.escribir ( static_cast<const void*>(mensaje.c_str()),mensaje.length() );
                logger.l("Juego",this->nombre,"Escribi en el fifo " +nombreCanalDeUnaPersona);
                canalPersona.cerrar();
                i++;
        }

        //limpio la lista de los nombres de las personas que estaban en el juego, para su proxima pasada
        this->nombresPersonas.clear();
        this->cantidadDePersonasEnLaCola=0;

        //vuelvo a empezar
    }
    cola.cerrar();
    cola.eliminar();

    return 0;
}

int Juego::arrancarUnaPasada()
{
    Logger logger;
    logger.l("Juego",this->nombre,"arranco pasada del juego");
    sleep(this->tiempoDeJuego);
    logger.l("Juego",this->nombre,"fin pasada del juego");
    return 0;
}
