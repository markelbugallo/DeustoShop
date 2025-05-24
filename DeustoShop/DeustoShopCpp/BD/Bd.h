#ifndef BD_H
#define BD_H
#include "../Clases/Almacen.h"
#include "../Clases/Pedido.h"
#include "../Clases/Producto.h"
#include "../Clases/Proveedor.h"
#include "../Clases/Subscripcion.h"
#include "../Clases/Usuario.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

vector<Almacen> cargarAlmacenesCSV(const string &fichero);
vector<Pedido> cargarPedidosCSV(const string &fichero);
vector<Producto> cargarProductosCSV(const string &fichero);
vector<Proveedor> cargarProveedoresCSV(const string &fichero);
vector<Subscripcion> cargarSubscripcionesCSV(const string &fichero);
vector<Usuario> cargarUsuariosCSV(const string &fichero);

// mapa para almacenar usuarios y sus contraseñas para el login
extern map<string, string> usuariosContrasenas;
#endif // BD_H