#ifndef PEDIDO_H
#define PEDIDO_H
#include <string>
#include <sstream>
#include <map>
using namespace std;

struct Fecha
{
    int dia;
    int mes;
    int anyo;

    // Parsea formato "YYYY-MM-DD"
    static Fecha parse(const string &s) {
        Fecha f;
        char sep1, sep2;
        istringstream is(s);
        if (!(is >> f.anyo >> sep1 >> f.mes >> sep2 >> f.dia) || sep1 != '-' || sep2 != '-') {
            throw invalid_argument("Formato de fecha inválido: " + s);
        }
        return f;
    }
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
    void imprimirInfPedido(Usuario usuarioactual);
};

#endif // PEDIDO_H