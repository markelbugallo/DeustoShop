

#ifndef INCLUDE_STOCK_H_
#define INCLUDE_STOCK_H_
#include "almacen.h"
#include "producto.h"
#include "../external/sqlite3.h"

typedef struct {
	int id_almacen;
	int id_producto;
	int stock_min;
	int cantidad;
} Stock;

int stockAlmacen(Stock *stock, int numStock, Producto *producto, Almacen *almacen);

int getId_almalcen_stock(Stock sto);
int getId_producto_stock(Stock sto);
int getStock_min_stock(Stock sto);
int getCantidad_stock(Stock sto);

#endif /* INCLUDE_STOCK_H_ */
