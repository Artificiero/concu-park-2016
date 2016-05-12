#include "Persona.h"

//se supone la cartilla de juegos ordenadas de menor a mayor por precio
Persona::Persona(unsigned int presupuesto,std::string nombre,Puerta& puertaE,Puerta& PuertaS,CajaCentral& cajaC,std::list<FolletoJuego> cartillaDeJuegos): presupuesto(presupuesto),nombre(nombre),puertaEntrada(puertaE),puertaSalida(PuertaS),cajaCentral(cajaC),cartillaDeJuegos(cartillaDeJuegos)
{
    //ctor
}

Persona::~Persona()
{
    //dtor
}

int Persona::ponerseEnColaDeJuego(std::string nombreJuego)
{
        const std::string ARCHIVO_JUEGO = "/tmp/archivo_fifo_juego_" + nombreJuego;

        char buff[sizeof(int)];
        std::memset(buff, 0, sizeof(buff) );
        strncpy(buff, this->nombre.c_str(), sizeof(buff));
        //buff[sizeof(buff) - 1] = 0;

		FifoEscritura canalJuego ( ARCHIVO_JUEGO );
		canalJuego.abrir();

		std::cout << "Persona:"<< this->nombre << " :intento entrar al juego(escribir en el fifo):"<< ARCHIVO_JUEGO << std::endl;
		canalJuego.escribir ( static_cast<const void*>(buff),sizeof(buff) );
        std::cout << "Persona:"<< this->nombre << " Logre comunicarme con el juego, mande " << sizeof(buff) << " bytes"<< std::endl;
        canalJuego.cerrar();

        static const std::string lecturaPropiaHijo = "/tmp/archivo_fifo_persona_" + this->nombre;

		FifoLectura canalLeerPropioHijo (lecturaPropiaHijo);
		std::cout << "Persona:"<< this->nombre << " intento abrir canal propio:"<< lecturaPropiaHijo <<  std::endl;
		canalLeerPropioHijo.abrir();
		std::cout << "Persona:"<< this->nombre << " canal propio abierto"<< std::endl;

        int BUFFSIZE = 100;
		char buffer[BUFFSIZE];

		std::cout << "Persona:"<< this->nombre <<" [Lector] esperando que me libere el juego" << std::endl;
		ssize_t bytesLeidos = canalLeerPropioHijo.leer(static_cast<void*>(buffer),BUFFSIZE);
		std::string mensaje = buffer;
		mensaje.resize ( bytesLeidos );
		std::cout << "Persona:"<< this->nombre <<" [Lector] Lei el dato del fifo: " << mensaje << std::endl;

		canalLeerPropioHijo.cerrar();
		canalLeerPropioHijo.eliminar();
		std::cout << "Persona:"<< this->nombre <<" [Lector] Fin del proceso" << std::endl;

		return 0;
}

int Persona::vivir()
{

    std::cout << "Persona:" << this->nombre << ":"<< "Voy a entrar al parque" << std::endl;
    this->puertaEntrada.cruzar();
    std::cout << "Persona:" << this->nombre << ":"<< "Entre!" << std::endl;

    bool proximoJuegoCuestaMasDeLoQuePuedo = false;

    std::list<FolletoJuego>::iterator it=this->cartillaDeJuegos.begin();

    while ( (this->presupuesto > 0) || (!proximoJuegoCuestaMasDeLoQuePuedo) )
    {
        //veo el siguiente juego en la lista
        FolletoJuego fj = *it ;

        //veo si me puedo subir al siguiente juego
        if ( fj.getPrecio() <= this->getPlataRestante() )
        {
            //pago el juego -> o sea la plata va a la caja central
            this->presupuesto = this->presupuesto - fj.getPrecio();
            this->cajaCentral.ingresarDinero(fj.getPrecio());

            //entro al juego, con su respectivo tiempo de espera de cola + pasada
            this->ponerseEnColaDeJuego(fj.getNombre());

            it++;
            if( it==this->cartillaDeJuegos.end() )
            {   //si llegue al final de la cartila de juego, vuelvo a comenzar
                it=this->cartillaDeJuegos.begin();
            }

        } else {
            //me quede sin plata suficiente para subir al proximo juego
            proximoJuegoCuestaMasDeLoQuePuedo = true;
            std::cout << "**************Persona:"<< this->nombre << ":"<< "me quede sin plata!***********" << std::endl;
        }
    }

    std::cout << "Persona:" << this->nombre << ":"<< "!!!!!!!Voy a salir del parque!!!!!!!!" << std::endl;
    this->puertaSalida.cruzar();
    std::cout << "Persona:" << this->nombre << ":"<< "sali!" << std::endl;

    sleep(10);

    return 0;
}
