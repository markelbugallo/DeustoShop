#include "Bd.h"
#include "Almacen.h"
#include "Producto.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

vector<Almacen> cargarAlmacenesCSV(const string &fichero) {
    vector<Almacen> almacenes;
    string line;

    ifstream file(fichero);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo: " << fichero;
        return almacenes;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string id, nombre, contacto, direccion, postal;

        getline(ss, id, ';');
        getline(ss, nombre, ';');
        getline(ss, contacto, ';');
        getline(ss, direccion, ';');
        getline(ss, postal, ';');


        try {
            int idInt = stoi(id);
            int postalInt = stoi(postal);

            Almacen a(idInt, nombre, contacto, direccion, postalInt);
            almacenes.push_back(a);
        }
        catch (const invalid_argument &e) {
            cout << "Error de formato en línea: \"" << line << "\" -> "
                 << e.what() << endl;
        }
        catch (const out_of_range &e) {
            cout << "Valor fuera de rango en línea: \"" << line << "\" -> "
                 << e.what() << endl;
        }
    }
    
    file.close();
    return almacenes;
}

vector<Producto> cargarProductosCSV(const string &fichero) {
    vector<Producto> productos;
    string line;

    ifstream file(fichero);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo: " << fichero;
        return productos;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string id_producto, nombre_producto, descripcion, precio, id_proveedor, categoria;

        getline(ss, id_producto, ';');
        getline(ss, nombre_producto, ';');
        getline(ss, descripcion, ';');
        getline(ss, precio, ';');
        getline(ss, id_proveedor, ';');
        getline(ss, categoria, ';');


        try {
            int idProductoInt = stoi(id_producto);
            double doublePrecio = stod(precio);
            int idProveedorInt = stoi(id_proveedor);

            Producto p(idProductoInt, nombre_producto, descripcion, doublePrecio, idProveedorInt, categoria);
            productos.push_back(p);
        } catch (const invalid_argument &e) {
            cout << "Error de formato en linea: \"" << line << "\" -> "
                 << e.what() << endl;
        }
        catch (const out_of_range &e) {
            cout << "Valor fuera de rango en linea: \"" << line << "\" -> "
                 << e.what() << endl;
        }
    }
    
    file.close();
    return productos;
}
