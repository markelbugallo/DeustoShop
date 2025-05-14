#include "Empleado.h"

Empleado::Empleado(
    int id_empleado,
    const string &nombre,
    int id_almacen,
    const string &rol
) : id_empleado(id_empleado), nombre(nombre), id_almacen(id_almacen), rol(rol) {}


// getters
int Empleado::getId_empleado() const{
    return id_empleado;
}
string Empleado::getNombre() const {
    return nombre;
}
int Empleado::getId_almacen() const {
    return id_almacen;
}
string Empleado::getRol() const {
    return rol;
}