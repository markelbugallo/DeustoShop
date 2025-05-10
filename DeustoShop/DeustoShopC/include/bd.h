

#ifndef INCLUDE_BD_H_
#define INCLUDE_BD_H_
#include "../external/sqlite3.h"
#include "producto.h"
#include "almacen.h"
#include "usuario.h"
#include "stock.h"
#include "proveedor.h"
#include "pedido.h"


int abrirBaseDatos(sqlite3 **db);
void cerrarBaseDatos(sqlite3 *db);

int insertarProducto(sqlite3 *db, Producto *p);
int listarProductos(sqlite3 *db);
int actualizarProducto(sqlite3 *db, Producto *p);
int eliminarProducto(sqlite3 *db, int idProducto);


int insertarAlmacen(sqlite3 *db, Almacen *a);
int listarAlmacenes(sqlite3 *db);
int actualizarAlmacen(sqlite3 *db, Almacen *a);
int eliminarAlmacen(sqlite3 *db, int idAlmacen);

int insertarUsuario(sqlite3 *db, Usuario *u);
int listarUsuarios(sqlite3 *db);
int actualizarUsuario(sqlite3 *db, Usuario *u);
int eliminarUsuario(sqlite3 *db, int idUsuario);

int listarStock(sqlite3 *db);

int listarProveedores(sqlite3 *db);
int insertarProveedor(sqlite3 *db, Proveedor *p);
int eliminarProveedor(sqlite3 *db, int idProveedor);
int actualizarProveedor(sqlite3 *db, Proveedor *p);

Producto* cargarProductos(sqlite3 *db, int *numProductos);
Almacen* cargarAlmacenes(sqlite3 *db, int *numAlmacenes);
Usuario* cargarUsuarios(sqlite3 *db, int *numUsuarios);
Proveedor* cargarProveedores(sqlite3 *db, int *numProveedores);
Pedido* cargarPedidos(sqlite3 *db, int *numPedidos);

void guardarProductos(sqlite3 *db, Producto *productos, int numProductos);
void guardarAlmacenes(sqlite3 *db, Almacen *almacenes, int numAlmacenes);
void guardarUsuarios(sqlite3 *db, Usuario *usuarios, int numUsuarios);
void guardarProveedores(sqlite3 *db, Proveedor *proveedores, int numProveedores);
void guardarPedidos(sqlite3 *db, Pedido *pedidos, int numPedidos);
void cargarCSVProducto(sqlite3 *db);
void cargarCSVProveedor(sqlite3 *db);

void liberarAlmacenes(Almacen *almacenes, int numAlmacenes);
void liberarProductos(Producto *productos);
void liberarUsuarios(Usuario *usuarios);
void liberarProveedores(Proveedor *proveedores);

void cargarStockAlmacenes(sqlite3 *db, Almacen *almacenes, int numAlmacenes, Producto *productos, int numProductos);


#endif /* INCLUDE_BD_H_ */
