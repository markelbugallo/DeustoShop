#include "Proveedor.h"
#include <iostream>
#include <string>
using namespace std;

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
void Proveedor::imprimirInfProveedor() {
    cout << "-------------------------------------------------------------" << endl;
    cout << "ID de proveedor: " << id_proveedor << endl;
    cout << "Nombre de proveedor: " << nombre_proveedor << endl;
    cout << "Contacto de proveedor: " << contacto_proveedor << endl;
    cout << "-------------------------------------------------------------" << endl;
}