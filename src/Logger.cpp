#include "Logger.h"

Logger::Logger():lockEscritura("lockEscrituraLogger"),nombre("registroActividades")
{
}

Logger::Logger(std::string nombre):lockEscritura("lockEscrituraLogger"),nombre(nombre)
{
}

Logger::~Logger()
{
}

int Logger::loggear(std::string nombreActor, std::string msj, std::string tipoMensaje)
{
    //muchos procesos van a estar al mismo tiempo escribiendo a un UNICO archivo
    //asi que debo protegerlo con un lock
    this->lockEscritura.tomarLock();

    std::string msjparseado = this->currentDateTime() + "-" + tipoMensaje + "-"+ nombreActor + "-"+ msj;
    std::ofstream myfile;
    myfile.open (this->nombre.c_str(), std::ios::out | std::ios::app);
    myfile << msjparseado << std::endl;
    myfile.close();
    std::cout << msjparseado << std::endl;

    this->lockEscritura.liberarLock();

    return 0;
}
