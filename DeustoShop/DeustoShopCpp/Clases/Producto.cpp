#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "Producto.h"

// constructor
Producto::Producto(
    int id_producto,
    const string &nombre_producto,
    const string &descripcion,
    double precio,
    int id_proveedor,
    const string &categoria
) : id_producto(id_producto), nombre_producto(nombre_producto), descripcion(descripcion), precio(precio), id_proveedor(id_proveedor), categoria(categoria) {}


// getters
int Producto::getId_producto() const {
    return id_producto;
}

string Producto::getNombre_producto() const {
    return nombre_producto;
}

string Producto::getDescripcion() const {
    return descripcion;
}

double Producto::getPrecio() const {
    return precio;
}

int Producto::getId_proveedor() const {
    return id_proveedor;
}

string Producto::getCategoria() const {
    return categoria;
}