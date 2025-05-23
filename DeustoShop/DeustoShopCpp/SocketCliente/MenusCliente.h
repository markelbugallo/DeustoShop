#ifndef MENUSCLIENTE_H
#define MENUSCLIENTE_H

#include "../Clases/Usuario.h"
#include "../Clases/Almacen.h"
#include "../Clases/Pedido.h"
#include "../Clases/Producto.h"
#include <vector>
#include <string>
using namespace std;

class MenusCliente
{
private:
    vector<Almacen> almacenes;
    vector<Pedido> pedidos;
    vector<Producto> productos;
public:
    void cargarDatos();
    void mostrarMenuInicial();
    Usuario mostrarMenuRegistro();
    void mostrarMenuPrincipal(Usuario usuario);
    void mostrarProductos(Usuario usuario);
    void mostrarHistorialCompras(Usuario usuario);
    void mostrarAlmacenes(Usuario usuario);
    void mostrarMenuMiPerfil(Usuario usuario);
    int mandarAlServidor(const string &mensaje, string &respuesta);
};

#endif // MENUSCLIENTE_H