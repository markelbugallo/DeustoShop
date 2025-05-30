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
void guardarUsuariosCsv(const vector<Usuario>& usuarios);
void cargarDatos();


extern map<string, string> usuariosContrasenas;
extern vector<Almacen> almacenes;
extern vector<Pedido> pedidos;
extern vector<Producto> productos;
extern vector<Proveedor> proveedores;
extern vector<Subscripcion> subscripciones;
extern vector<Usuario> usuarios;
void guardarPedidosCsv(const vector<Pedido>& pedidos);
#endif // BD_H