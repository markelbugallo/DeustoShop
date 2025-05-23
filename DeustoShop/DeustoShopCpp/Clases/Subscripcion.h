#ifndef SUBSCRIPCION_H
#define SUBSCRIPCION_H
#include <string>
using namespace std;
#include "Usuario.h"

class Subscripcion{
private:
    int id_subscripcion;
    string tipo;
    int descuento;
public:
    Subscripcion();
    Subscripcion(
        int id_subscripcion,
        const string &tipo,
        int descuento
    );

    int getId_subscripcion() const;
    string getTipo() const;
    int getDescuento() const;
    void imprimirInfSubscripcion(Usuario usuarioActual);
       
    
};

#endif // SUBSCRIPCION_H