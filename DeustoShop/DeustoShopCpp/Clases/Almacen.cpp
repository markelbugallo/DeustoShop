#include "Almacen.h"

// constructor
Almacen::Almacen(const string &nombre,
    int postal,
    int id,
    const string &direccion,
    const string &contacto
) : nombre(nombre), postal(postal), id(id), direccion(direccion), contacto(contacto) {}


// getters
string Almacen::getNombre() const {
    return nombre;
}

int Almacen::getPostal() const {
    return postal;
}

int Almacen::getId() const {
    return id;
}

string Almacen::getDireccion() const {
    return direccion;
}

string Almacen::getContacto() const {
    return contacto;
}