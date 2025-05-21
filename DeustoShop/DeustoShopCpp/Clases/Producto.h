#ifndef PRODUCTO_H
#define PRODUCTO_H
#include <string>
using namespace std;

class Producto {
private:
    int id_producto;
    string nombre_producto;
    string descripcion;
    double precio;
    int id_proveedor;
    string categoria;
public:
    Producto();
    Producto(
        int id_producto,
        const string &nombre_producto,
        const string &descripcion,
        double precio,
        int id_proveedor,
        const string &categoria
    );

    int getId_producto() const;
    string getNombre_producto() const;
    string getDescripcion() const;
    double getPrecio() const;
    int getId_proveedor() const;
    string getCategoria() const;
};

#endif // PRODUCTO_H