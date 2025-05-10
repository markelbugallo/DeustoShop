#include "Proveedor.h"

// constructor
Proveedor::Proveedor(
    int id_proveedor,
    const string &nombre_proveedor,
    const string &contacto_proveedor
) : id_proveedor(id_proveedor), nombre_proveedor(nombre_proveedor), contacto_proveedor(contacto_proveedor) {}

// getters
int Proveedor::getId_proveedor() const {
    return id_proveedor;
}

string Proveedor::getNombre_proveedor() const {
    return nombre_proveedor;
}

string Proveedor::getContacto_proveedor() const {
    return contacto_proveedor;
}