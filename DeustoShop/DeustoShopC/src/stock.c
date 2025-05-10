
#include "../include/stock.h"
#include "../include/almacen.h"
#include "../include/producto.h"

int stockAlmacen(Stock *stock, int numStock, Producto *producto, Almacen *almacen) {
	for (int i = 0; i < numStock; i++) {
		if (stock[i].id_producto == producto->id_producto && stock[i].id_almacen == almacen->id_almacen) {
			return stock[i].cantidad;
		}
	}
	return -1; // No se encontró el stock para el producto en el almacén
}

int getId_almacen_stock(Stock sto){
	return sto.id_almacen;
}
int getId_producto_stock(Stock sto){
	return sto.id_producto;
}
int getStock_min_stock(Stock sto){
	return sto.stock_min;
}
int getCantidad_stock(Stock sto){
	return sto.cantidad;
}

