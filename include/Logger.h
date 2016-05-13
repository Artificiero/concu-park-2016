#ifndef LOGGER_H
#define LOGGER_H

#include "LockFile.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <fstream>

class Logger
{
    public:
        Logger();
        Logger(std::string nombre);
        virtual ~Logger();
        int loggear(std::string nombreActor,std::string msj,std::string tipoMensaje="DEBUG");
    protected:
    private:

    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    const std::string currentDateTime() {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y:%m:%d:%X", &tstruct);

        return buf;
    }

    LockFile lockEscritura;
    std::string nombre;
};

#endif // LOGGER_H
