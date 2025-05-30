#ifndef MENUSCLIENTE_H
#define MENUSCLIENTE_H

#include <winsock2.h>
#include "../Clases/Almacen.h"
#include "../Clases/Pedido.h"
#include "../Clases/Producto.h"
#include "../Clases/Proveedor.h"
#include "../Clases/Subscripcion.h"
#include "../Clases/Usuario.h"
#include <vector>
#include <string>
#include <map>
using namespace std;


class ConexionServidor {
public:
    SOCKET s;
    bool conectada;
    ConexionServidor() : s(INVALID_SOCKET), conectada(false) {}
    bool conectar();
    bool enviar(const std::string& mensaje, std::string& respuesta);
    void cerrar();
};

class MenusCliente
{
private:
    map<int,int> cesta;
    vector<Producto> listaProductos;
public:
    int mandarAlServidor(const std::string &mensaje, std::string &respuesta);
    void mostrarMenuInicial();
    Usuario mostrarMenuRegistro();
    Usuario mostrarMenuInicioSesion();
    void mostrarMenuPrincipal(Usuario usuario_actual);
    void mostrarMenuC(Usuario usuario_actual);
    double obtenerPrecioProducto(int id_producto);
    void mostrarMenuCesta(Usuario& usuario_actual);
    void mostrarMenuProductos(Usuario usuario_actual);
    void mostrarTodosLosProductos(Usuario& usuario_actual);
    void mostrarProductosPorCategoria(Usuario& usuario_actual);
    void realizarPedido(Usuario& usuario_actual);
    void mostrarHistorialCompras(Usuario usuario_actual);
    void mostrarAlmacenes(Usuario usuario_actual);
    void editarPerfil(Usuario& usuario_actual);
    void mostrarMenuMiPerfil(Usuario& usuario_actual);
    void eliminarPerfil(Usuario& usuario_actual);
    static ConexionServidor conexionSesion;
};

#endif // MENUSCLIENTE_H