#ifndef PEDIDO_H
#define PEDIDO_H
#include <string>
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
    int id_producto;
    int cantidad;
    string direccion;
    int codigo_Postal;

public:
    Pedido();
    Pedido(
        int id_pedido,
        Fecha fecha_pedido,
        const string &estado_pedido,
        int id_usuario,
        int id_producto,
        int cantidad,
        const string &direccion,
        int codigo_Postal
    );

    int getId_pedido() const;
    Fecha getFecha_pedido() const;
    string getEstado_pedido() const;
    int getId_usuario() const;
    int getId_producto() const;
    int getCantidad() const;
    string getDireccion() const;
    int getCodigo_Postal() const;
};

#endif // PEDIDO_H