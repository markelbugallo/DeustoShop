#include "Almacen.h"
#include <iostream>
using namespace std;

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

void Almacen::imprimirAlmacen() {
    cout << "---------------------------------------------------" << endl;
    cout << "ID Almacen: " << id << endl;
    cout << "Nombre del almacen: " << nombre << endl;
    cout << "Email: " << contacto << endl;
    cout << "Direccion: " << direccion << ", " << postal << endl;
    cout << "---------------------------------------------------" << endl << endl;
}