#ifndef MENUSCLIENTE_H
#define MENUSCLIENTE_H

#include "../Clases/Almacen.h"
#include "../Clases/Pedido.h"
#include "../Clases/Producto.h"
#include "../Clases/Proveedor.h"
#include "../Clases/Subscripcion.h"
#include "../Clases/Usuario.h"
#include <vector>
#include <string>
using namespace std;

class MenusCliente
{
private:
    vector<Almacen> almacenes;
    vector<Pedido> pedidos;
    vector<Producto> productos;
    vector<Proveedor> proveedores;
    vector<Subscripcion> subscripciones;
    vector<Usuario> usuarios;
public:
    void cargarDatos();
    void mostrarMenuInicial();
    Usuario mostrarMenuRegistro();
    Usuario mostrarMenuInicioSesion();
    void mostrarMenuPrincipal(Usuario usuario);
    void mostrarMenuProductos(Usuario usuario);
    void mostrarTodosLosProductos(Usuario usuario);
    void mostrarProductosPorCategoria(Usuario usuario);
    void mostrarHistorialCompras(Usuario usuario);
    void mostrarAlmacenes(Usuario usuario);
    void mostrarMenuMiPerfil(Usuario usuario);
    int mandarAlServidor(const string &mensaje, string &respuesta);
};

#endif // MENUSCLIENTE_H