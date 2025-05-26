#include "Bd.h"
#include "../Clases/Almacen.h"
#include "../Clases/Producto.h"
#include "../Clases/Pedido.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

vector<Almacen> cargarAlmacenesCSV(const string &fichero) {
    vector<Almacen> almacenes;
    string linea;

    ifstream file(fichero);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo: " << fichero << endl;
        return almacenes;
    }

    while (getline(file, linea)) {
        stringstream ss(linea);
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
            cout << "Error de formato en línea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
        catch (const out_of_range &e) {
            cout << "Valor fuera de rango en línea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
    }
    
    file.close();
    return almacenes;
}

vector<Pedido> cargarPedidosCSV(const string &fichero) {
    vector<Pedido> pedidos;
    string linea;

    ifstream file(fichero);
    if (!file.is_open()) {
        cout << "No se pudo abrir el fichero: " << fichero << endl;
        return pedidos;
    }

    while (getline(file, linea)) {
        stringstream ss(linea);
        string id_pedidoStr, fecha_pedidoStr, estado_pedido, id_usuarioStr, productosCantidadesStr, direccion, codigo_postalStr;

        getline(ss, id_pedidoStr, ';');
        getline(ss, fecha_pedidoStr, ';');
        getline(ss, estado_pedido, ';');
        getline(ss, id_usuarioStr, ';');
        getline(ss, productosCantidadesStr, ';');
        getline(ss, direccion, ';');
        getline(ss, codigo_postalStr, ';');

        try
        {
            int id_pedido = stoi(id_pedidoStr);
            Fecha fecha_pedido = Fecha::parse(fecha_pedidoStr);
            int id_usuario = stoi(id_usuarioStr);
            int codigo_Postal = stoi(codigo_postalStr);

            map<int, int> productosCantidades;
            stringstream mp(productosCantidadesStr);
            string conjunto;

            while (getline(mp, conjunto, ',')) {
                auto pos = conjunto.find(':');
                if (pos == string::npos) continue;
                int idProducto = stoi(conjunto.substr(0, pos));
                int cantidad = stoi(conjunto.substr(pos+1));
                productosCantidades[idProducto] = cantidad;
            }

            Pedido pe(id_pedido, fecha_pedido, estado_pedido, id_usuario, productosCantidades, direccion, codigo_Postal);
            pedidos.push_back(pe);
        
        } catch (const invalid_argument &e) {
            cout << "Error de formato en linea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
        catch (const out_of_range &e) {
            cout << "Valor fuera de rango en linea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
    }
    
    file.close();
    return pedidos;
}

vector<Producto> cargarProductosCSV(const string &fichero) {
    vector<Producto> productos;
    string linea;

    ifstream file(fichero);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo: " << fichero << endl;
        return productos;
    }

    while (getline(file, linea)) {
        stringstream ss(linea);
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
            cout << "Error de formato en linea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
        catch (const out_of_range &e) {
            cout << "Valor fuera de rango en linea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
    }
    
    file.close();
    return productos;
}

vector<Proveedor> cargarProveedoresCSV(const string &fichero) {
    vector<Proveedor> proveedores;
    string linea;

    ifstream file(fichero);
    if(!file.is_open()) {
        cout << "No se pudo abrir el archivo: " << fichero << endl;
        return proveedores;
    }

    while (getline(file, linea)) {
        stringstream ss(linea);
        string id_proveedorStr, nombre_proveedor, contacto_proveedor;

        getline(ss, id_proveedorStr, ';');
        getline(ss, nombre_proveedor, ';');
        getline(ss, contacto_proveedor, ';');

        try
        {
            int id_proveedor = stoi(id_proveedorStr);

            Proveedor prov(id_proveedor, nombre_proveedor, contacto_proveedor);
            proveedores.push_back(prov);
        } catch (const invalid_argument &e) {
            cout << "Error de formato en linea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
        catch (const out_of_range &e) {
            cout << "Valor fuera de rango en linea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
    }

    file.close();
    return proveedores;   
}

vector<Subscripcion> cargarSubscripcionesCSV(const string &fichero) {
    vector<Subscripcion> subscripciones;
    string linea;

    ifstream file(fichero);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo: " << fichero << endl;
        return subscripciones;
    }

    while (getline(file, linea)) {
        stringstream ss(linea);
        string id_subcripcionStr, tipo, descuentoStr;

        getline(ss, id_subcripcionStr, ';');
        getline(ss, tipo, ';');
        getline(ss, descuentoStr, ';');

        try
        {
            int id_subcripcion = stoi(id_subcripcionStr);
            int descuento = stoi(descuentoStr);

            Subscripcion sub(id_subcripcion, tipo, descuento);
            subscripciones.push_back(sub);
        } catch (const invalid_argument &e) {
            cout << "Error de formato en linea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
        catch (const out_of_range &e) {
            cout << "Valor fuera de rango en linea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
    }
    
    file.close();
    return subscripciones;
}

vector<Usuario> cargarUsuariosCSV(const string &fichero) {
    vector<Usuario> usuarios;
    string linea;

    ifstream file(fichero);
    if (!file.is_open()) {
        cout << "No se pudo abrir el archivo: " << fichero << endl;
        return usuarios;
    }

    while (getline(file, linea)) {
        stringstream ss(linea);
        string id_usuarioStr, nombre_usuario, contrasena_usuario, contacto_usuario, id_subscripcionStr, direccion, codigo_postalStr;

        getline(ss, id_usuarioStr, ';');
        getline(ss, nombre_usuario, ';');
        getline(ss, contrasena_usuario, ';');
        getline(ss, contacto_usuario, ';');
        getline(ss, id_subscripcionStr, ';');
        getline(ss, direccion, ';');
        getline(ss, codigo_postalStr, ';');

        try
        {
            int id_usuario = stoi(id_usuarioStr);
            int id_subscripcion = stoi(id_subscripcionStr);
            int codigo_postal = stoi(codigo_postalStr);

            Usuario u(id_usuario, nombre_usuario, contrasena_usuario, contacto_usuario, id_subscripcion, direccion, codigo_postal);
            usuarios.push_back(u);
        } catch (const invalid_argument &e) {
            cout << "Error de formato en linea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
        catch (const out_of_range &e) {
            cout << "Valor fuera de rango en linea: \"" << linea << "\" -> "
                 << e.what() << endl;
        }
    }

    file.close();
    return usuarios;
}
void guardarUsuariosCSV(const string& filename, const vector<Usuario>& usuarios) {
    ofstream file("../DeustoShopC/Data/usuarios.csv");
    file << "id_usuario,nombre_usuario,contrasena_usuario,contacto_usuario,id_subscripcion,direccion,codigo_postal\n";
    for (const auto& u : usuarios) {
        file << u.getId_usuario() << ","
             << u.getNombre_usuario() << ","
             << u.getContrasena_usuario() << ","
             << u.getContacto_usuario() << ","
             << u.getId_subscripcion() << ","
             << u.getDireccion() << ","
             << u.getCodigo_postal() << "\n";
    }
}
map<string, string> usuariosContrasenas;