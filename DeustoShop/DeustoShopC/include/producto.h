
#ifndef INCLUDE_PRODUCTO_H_
#define INCLUDE_PRODUCTO_H_

#include "../external/sqlite3.h"

typedef enum {
    ELECTRONICA,
    ALIMENTACION,
	LIBRERIA,
	JUGUETERIA,
    ROPA,
    HOGAR,
    DEPORTE,
    OTROS
} Categoria;

typedef struct {
    int id_producto;
    char nombre[50];
    char descripcion[100];
    Categoria categoria;
    double precio;
    int id_proveedor;
} Producto;


int getId_producto(Producto pro);
const char* getNombre_prducto(Producto pro);
const char* getDescripcion_producto(Producto pro);
Categoria getCategoria_producto(Producto pro);
const char* categoriaToString_producto(Categoria cat);
double getprecio_producto(Producto pro);
int getId_proveedor_producto(Producto pro);



#endif /* INCLUDE_PRODUCTO_H_ */
