#include "Subscripcion.h"

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