#ifndef BD_H
#define BD_H
#include "Almacen.h"
#include "Producto.h"
#include <string>
#include <vector>
using namespace std;

vector<Almacen> cargarAlmacenesCSV(const string &fichero);
vector<Producto> cargarProductosCSV(const string &fichero);

#endif // BD_H