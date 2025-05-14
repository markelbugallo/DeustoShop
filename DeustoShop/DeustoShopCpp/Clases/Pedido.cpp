#include "Pedido.h"

// constructor
Pedido::Pedido(
    int id_pedido,
    Fecha fecha_pedido,
    const string &estado_pedido,
    int id_usuario,
    int id_producto,
    int cantidad,
    const string &direccion,
    int codigo_Postal
) : id_pedido(id_pedido), fecha_pedido(fecha_pedido), estado_pedido(estado_pedido), id_usuario(id_usuario), id_producto(id_producto), cantidad(cantidad), direccion(direccion), codigo_Postal(codigo_Postal) {}

// getters
int Pedido::getId_pedido() const {
    return id_pedido;
}

Fecha Pedido::getFecha_pedido() const {
    return fecha_pedido;
}

string Pedido::getEstado_pedido() const {
    return estado_pedido;
}

int Pedido::getId_usuario() const {
    return id_usuario;
}

int Pedido::getId_producto() const {
    return id_producto;
}

int Pedido::getCantidad() const {
    return cantidad;
}

string Pedido::getDireccion() const {
    return direccion;
}

int Pedido::getCodigo_Postal() const {
    return codigo_Postal;
}
