#ifndef ALMACEN_H
#define ALMACEN_H
#include <string>
#include "Usuario.h"
using namespace std;

class Almacen
{
private:
    int id;
    string nombre;
    string contacto;
    string direccion;
    int postal;

public:
    Almacen();
    Almacen(
        int id,
        const string &nombre,
        const string &contacto,
        const string &direccion,
        int postal
    );

    // getters
    int getId() const;
    string getNombre() const;
    string getContacto() const;
    string getDireccion() const;
    int getPostal() const;

    // funciones
    void imprimirAlmacen();
    
};

#endif // ALMACEN_H