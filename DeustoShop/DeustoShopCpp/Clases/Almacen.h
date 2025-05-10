#ifndef ALMACEN_H
#define ALMACEN_H
#include <string>
using namespace std;

class Almacen
{
private:
    string nombre;
    int postal;
    int id;
    string direccion;
    string contacto;

public:
    Almacen();
    Almacen(
        const string &nombre,
        int postal,
        int id,
        const string &direccion,
        const string &contacto
    );

    string getNombre() const;
    int getPostal() const;
    int getId() const;
    string getDireccion() const;
    string getContacto() const;
};

#endif // ALMACEN_H