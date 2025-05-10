
#include <stddef.h>  // NULL
#include "../include/pedido.h"
#include "../include/almacen.h"
#include "../include/usuario.h"

Almacen* almacenPedido(Almacen *almacenes, int numAlmacenes, Usuario *usuario, int codigo_postal) {
	int codigo = codigo_postal;

	if (codigo_postal == NULL) {
		codigo = usuario->codigo_postal;
	}
	Almacen *almacen = buscarAlmacenPorCodigoPostal(almacenes, numAlmacenes, codigo);
	if (almacen == NULL) {
		printf("No se encontró un almacén en la zona del código postal %d\n", codigo);
		return NULL;
	}
	return almacen;
}
int getId_pedido(Pedido pedi){
	return pedi.id_pedido ;
}
int getId_usuario_pedido(Pedido pedi){
	return pedi.id_usuario ;
}
int getId_producto_pedido(Pedido pedi){
	return pedi.id_pedido ;
}
int getcantidad_pedido(Pedido pedi){
	return pedi.cantidad;
}
const char* fecha_pedido(Pedido pedi){
	return pedi.fecha;
}
int estado_pedido(Pedido pedi){
	return pedi.estado;
}
const char* direccion_pedido(Pedido pedi){
	return pedi.direccion;
}
int getCodigo_Postal_pedido(Pedido pedi){
	return pedi.codigo_postal;
}
