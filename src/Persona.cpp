#include "Persona.h"

//se supone la cartilla de juegos ordenadas de menor a mayor por precio
Persona::Persona(unsigned int presupuesto,std::string nombre,Puerta& puertaE,Puerta& PuertaS,std::list<FolletoJuego> cartillaDeJuegos): presupuesto(presupuesto),nombre(nombre),puertaEntrada(puertaE),puertaSalida(PuertaS),cartillaDeJuegos(cartillaDeJuegos)
{
    //ctor
}

Persona::~Persona()
{
    //dtor
}

int Persona::ponerseEnColaDeJuego(std::string nombreJuego)
{
        Logger logger;

        const std::string ARCHIVO_JUEGO = "/tmp/archivo_fifo_juego_" + nombreJuego;

        char buff[sizeof(int)];
        std::memset(buff, 0, sizeof(buff) );
        strncpy(buff, this->nombre.c_str(), sizeof(buff));
        //buff[sizeof(buff) - 1] = 0;

		FifoEscritura canalJuego ( ARCHIVO_JUEGO );
		canalJuego.abrir();

        logger.l("Persona",this->nombre,"intente entrar al juego(escribir en el fifo):"+ARCHIVO_JUEGO );
		canalJuego.escribir ( static_cast<const void*>(buff),sizeof(buff) );
        logger.l("Persona",this->nombre, std::string("Logre comunicarme con el juego ")+nombreJuego );

        static const std::string lecturaPropiaHijo = "/tmp/archivo_fifo_persona_" + this->nombre;

		FifoLectura canalLeerPropioHijo (lecturaPropiaHijo);
        logger.l("Persona",this->nombre, "intento abrir canal propio:"+lecturaPropiaHijo);
		canalLeerPropioHijo.abrir();
        logger.l("Persona",this->nombre, "canal propio abierto:"+lecturaPropiaHijo);

        int BUFFSIZE = 100;
		char buffer[BUFFSIZE];

        logger.l("Persona",this->nombre,"esperando que me libere el juego");
		ssize_t bytesLeidos = canalLeerPropioHijo.leer(static_cast<void*>(buffer),BUFFSIZE);
		std::string mensaje = buffer;
		mensaje.resize ( bytesLeidos );
        logger.l("Persona",this->nombre,"[Lector] Lei el dato del fifo: "+mensaje );

		canalLeerPropioHijo.cerrar();
		canalLeerPropioHijo.eliminar();
        logger.l("Persona",this->nombre,"[Lector] fin proceso" );
		return 0;
}

int Persona::vivir()
{
    std::cout << "persona viviendo" << std::endl;
    CajaCentral cajaCentral;

    Logger logger;

    logger.l("Persona",this->nombre,"Voy a entrar al parque");
    this->puertaEntrada.cruzar();
    logger.l("Persona",this->nombre,"Entre!");

    bool proximoJuegoCuestaMasDeLoQuePuedo = false;

    std::list<FolletoJuego>::iterator it=this->cartillaDeJuegos.begin();

    while ( (this->presupuesto > 0) && (!proximoJuegoCuestaMasDeLoQuePuedo) )
    {
        //veo el siguiente juego en la lista
        FolletoJuego fj = *it ;

        //veo si me puedo subir al siguiente juego
        if ( fj.getPrecio() <= this->getPlataRestante() )
        {
            //pago el juego -> o sea la plata va a la caja central
            this->presupuesto = this->presupuesto - fj.getPrecio();
            cajaCentral.ingresarDinero(fj.getPrecio());
            //entro al juego, con su respectivo tiempo de espera de cola + pasada
            this->ponerseEnColaDeJuego(fj.getNombre());

            it++;
            if( it==this->cartillaDeJuegos.end() )
            {   //si llegue al final de la cartila de juego, vuelvo a comenzar
                it=this->cartillaDeJuegos.begin();
            }

        } else {
            if (this->presupuesto==0)
            {
                //me quede sin plata, me voy
                proximoJuegoCuestaMasDeLoQuePuedo = true;
                logger.l("Persona",this->nombre,"*******me quede sin plata!!!!!!*******");
            }
            else
            {
                //me quede sin plata suficiente para subir al proximo juego,agarro el mas barato
                it=this->cartillaDeJuegos.begin();
            }
        }
    }
    logger.l("Persona",this->nombre,"!!!!!!!Voy a salir del parque!!!!!!!!");
    this->puertaSalida.cruzar();
    logger.l("Persona",this->nombre,"!!!!!!!sali!!!!!!!!");
    return 0;
}
