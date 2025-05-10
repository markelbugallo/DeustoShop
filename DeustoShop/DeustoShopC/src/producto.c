#include "../include/producto.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char* categoriaToString(Categoria cat) {
    switch (cat) {
        case ELECTRONICA: return "ELECTRONICA";
        case ALIMENTACION: return "ALIMENTACION";
        case LIBRERIA: return "LIBRERIA";
        case JUGUETERIA: return "JUGUETERIA";
        case ROPA: return "ROPA";
        case HOGAR: return "HOGAR";
        case DEPORTE: return "DEPORTE";
        case OTROS: return "OTROS";
        default: return "DESCONOCIDO";
    }
}

int getId_producto(Producto pro){
	return pro.id_producto;
}
const char* getNombre_producto(Producto pro){
	return pro.nombre;
}
const char* getDescripcion_producto(Producto pro){
	return pro.descripcion;
}
Categoria getCategoria_producto(Producto pro) {
    return pro.categoria;
}
double getprecio_producto(Producto pro){
	return pro.precio;
}
int getId_proveedor_producto(Producto pro){
	return pro.id_proveedor;
}


