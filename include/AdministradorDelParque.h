#ifndef ADMINISTRADORDELPARQUE_H
#define ADMINISTRADORDELPARQUE_H

#include "CajaCentral.h"
#include "Logger.h"

class AdministradorDelParque
{
    public:
        AdministradorDelParque();
        virtual ~AdministradorDelParque();
        void vivir(){
            CajaCentral cajaCentral;
            Logger logger;
            while(true)
            {
                logger.l("AdministradorDelParque","administrador","durmiendo","INFO");
                sleep(2);
                int dinero = cajaCentral.getCantidadDeDineroAlmacenada();

                std::stringstream ss;
                ss << dinero;
                std::string moneyz = ss.str();

                logger.l("AdministradorDelParque","administrador","cantidad de plata que vi:" + moneyz,"INFO");
            }
        }
    protected:
    private:
};

#endif // ADMINISTRADORDELPARQUE_H
