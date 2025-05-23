#include "Subscripcion.h"
#include <iostream>
#include <string>
using namespace std;


// constructor
Subscripcion::Subscripcion(
    int id_subscripcion,
    const string &tipo,
    int descuento
) : id_subscripcion(id_subscripcion), tipo(tipo), descuento(descuento) {}

// getters
int Subscripcion::getId_subscripcion() const {
    return id_subscripcion;
}

string Subscripcion::getTipo() const {
    return tipo;
}

int Subscripcion::getDescuento() const {
    return descuento;
}
void Subscripcion::imprimirInfSubscripcion(Usuario usuarioActual) {
    cout << "-------------------------------------------------------------" << endl;
    cout << "ID de subscripcion: " << id_subscripcion << endl;
    cout << "Tipo de subscripcion: " << tipo << endl;
    cout << "Descuento: " << descuento << "%" << endl;
    cout << "-------------------------------------------------------------" << endl;
}