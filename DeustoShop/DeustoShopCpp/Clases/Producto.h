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
    int categoria;
public:
    Producto();
    Producto(
        int id_producto,
        const string &nombre_producto,
        const string &descripcion,
        double precio,
        int id_proveedor,
        int categoria
    );

    int getId_producto() const;
    string getNombre_producto() const;
    string getDescripcion() const;
    double getPrecio() const;
    int getId_proveedor() const;
    int getCategoria() const;
};

#endif // PRODUCTO_H