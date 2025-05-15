#ifndef PEDIDO_H
#define PEDIDO_H
#include <string>
#include <map>
using namespace std;

struct Fecha
{
    int dia;
    int mes;
    int anyo;
};


class Pedido
{
private:
    int id_pedido;
    Fecha fecha_pedido;
    string estado_pedido;
    int id_usuario;
    map<int, int> productosCantidades;
    string direccion;
    int codigo_Postal;

public:
    Pedido();
    Pedido(
        int id_pedido,
        Fecha fecha_pedido,
        const string &estado_pedido,
        int id_usuario,
        const map<int, int> productosCantidades,
        const string &direccion,
        int codigo_Postal
    );

    int getId_pedido() const;
    Fecha getFecha_pedido() const;
    string getEstado_pedido() const;
    int getId_usuario() const;
    map<int, int> getProductosCantidades() const;
    string getDireccion() const;
    int getCodigo_Postal() const;
};

#endif // PEDIDO_H