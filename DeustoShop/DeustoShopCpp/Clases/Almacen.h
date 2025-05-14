#ifndef ALMACEN_H
#define ALMACEN_H
#include <string>
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

    int getId() const;
    string getNombre() const;
    string getContacto() const;
    string getDireccion() const;
    int getPostal() const;
    
};

#endif // ALMACEN_H