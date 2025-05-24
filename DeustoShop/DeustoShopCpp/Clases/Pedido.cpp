#include "Pedido.h"
#include <iostream>
#include <string>
using namespace std;
#include "Usuario.h"

// constructor
Pedido::Pedido(
    int id_pedido,
    Fecha fecha_pedido,
    const string &estado_pedido,
    int id_usuario,
    map<int, int> productosCantidades,
    const string &direccion,
    int codigo_Postal
) : id_pedido(id_pedido), fecha_pedido(fecha_pedido), estado_pedido(estado_pedido), id_usuario(id_usuario), productosCantidades(productosCantidades), direccion(direccion), codigo_Postal(codigo_Postal) {}

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

map<int, int> Pedido::getProductosCantidades() const {
    return productosCantidades;
}

string Pedido::getDireccion() const {
    return direccion;
}

int Pedido::getCodigo_Postal() const {
    return codigo_Postal;
}
void Pedido::imprimirInfPedido() const{
    cout << "-------------------------------------------------------------" << endl;
    cout << "Numero de pedido: " << id_pedido << "          "  << "Estado del envio: " << estado_pedido << endl << endl;
    cout << "Id del producto " << "                                  " << "Cantidades" << endl; 
    for (const auto &conjunto : productosCantidades) {
        cout << conjunto.first << "......................................................." << conjunto.second << endl;
    }
    cout << "-------------------------------------------------------------" << endl << endl;
}

map<int, int> Pedido::realizarPedidoInteractivo() {
    map<int, int> productosCantidades;
    int id_producto, cantidad;

    cout << "REALIZAR PEDIDO" << endl;
    cout << "Introduce el ID del producto (0 para terminar): ";
    cin >> id_producto;

    while (id_producto != 0) {
        cout << "Cantidad para el producto " << id_producto << ": ";
        cin >> cantidad;
        if (cantidad > 0) {
            productosCantidades[id_producto] += cantidad;
            cout << "Producto añadido al pedido." << endl;
        } else {
            cout << "Cantidad no válida. Intenta de nuevo." << endl;
        }
        cout << "Introduce el ID del siguiente producto (0 para terminar): ";
        cin >> id_producto;
    }

    return productosCantidades;
}