

#ifndef INCLUDE_PEDIDO_H_
#define INCLUDE_PEDIDO_H_
#include "usuario.h"
#include "almacen.h"


typedef struct {
	int id_pedido;
	int id_usuario;
	Producto *productos; // Array de productos
	int numProductos; // Número de productos en el pedido
	int *cantidad; // Array de cantidades
	int numCantidad; // Número de cantidades
	char fecha[20];
	int estado; // 0: Pendiente, 1: Enviado, 2: Entregado
	char direccion[50];
	int codigo_postal;
} Pedido;

//Almacen* almacenPedido(Almacen *almacenes, int numAlmacenes, Usuario *usuario,int codigo_postal); esta dando error pero igual es pq lo habeis dejado a medias
int getId_pedido(Pedido pedi);
int getId_usuario_pedido(Pedido pedi);
int getId_producto_pedido(Pedido pedi);
int getcantidad_pedido(Pedido pedi);
const char* fecha_pedido(Pedido pedi);
int estado_pedido(Pedido pedi);
const char* direccion_pedido(Pedido pedi);
int getCodigo_Postal_pedido(Pedido pedi);


#endif /* INCLUDE_PEDIDO_H_ */
