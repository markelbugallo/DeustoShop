#include "Almacen.h"

// constructor
Almacen::Almacen(
    int id,
    const string &nombre,
    const string &contacto,
    const string &direccion,
    int postal
) :  id(id), nombre(nombre), contacto(contacto), direccion(direccion), postal(postal) {}


// getters
int Almacen::getId() const {
    return id;
}

string Almacen::getNombre() const {
    return nombre;
}

string Almacen::getContacto() const {
    return contacto;
}

string Almacen::getDireccion() const {
    return direccion;
}

int Almacen::getPostal() const {
    return postal;
}