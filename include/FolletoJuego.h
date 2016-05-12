#ifndef FOLLETOJUEGO_H
#define FOLLETOJUEGO_H

#include <string>

class FolletoJuego
{
    public:
        FolletoJuego();
        FolletoJuego(int precio,std::string nombre);

        int getPrecio() const {return this->precio;}
        std::string getNombre() const {return this->nombre;}

        bool operator<(const FolletoJuego& a)
        {
            return this->precio < a.getPrecio();
        }

        virtual ~FolletoJuego();
    protected:
    private:

    int precio;
    std::string nombre;

};

#endif // FOLLETOJUEGO_H
