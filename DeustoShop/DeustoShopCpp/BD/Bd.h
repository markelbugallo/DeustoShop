#ifndef BD_H
#define BD_H
#include "../Clases/Almacen.h"
#include "../Clases/Producto.h"
#include "../Clases/Pedido.h"
#include <string>
#include <vector>
using namespace std;

vector<Almacen> cargarAlmacenesCSV(const string &fichero);
vector<Pedido> cargarPedidosCSV(const string &fichero);
vector<Producto> cargarProductosCSV(const string &fichero);
#endif // BD_H