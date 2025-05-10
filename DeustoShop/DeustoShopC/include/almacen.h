
#ifndef INCLUDE_ALMACEN_H_
#define INCLUDE_ALMACEN_H_

#include "../external/sqlite3.h"

typedef struct {
	int id_almacen;
	char nombre[50];
	int codigo;
	char direccion[15];
	char contacto [30];
	Producto *productos; // Array de productos
	int numProductos; // Número de productos en el almacén
	int *stock; // Array de stock
	int numStock; // Número de productos en stock
} Almacen;

typedef enum {
	ZONA_NORTE,
	ZONA_NORESTE,
	ZONA_CENTRO,
	ZONA_SURESTE,
	ZONA_SUR,
	ZONA_OESTE
} Zona;

Zona obtenerZona(int codigoPostal);
Almacen* buscarAlmacenPorCodigoPostal(Almacen *almacenes, int numAlmacenes, int codigoPostal);
int getId_almacen(Almacen alma);
const char* getNombreAlmacen(Almacen alma);
int getcodigoAlmacen(Almacen alma);
const char* getDireccionAlmacen(Almacen alma);
const char* getContactoAlmacen(Almacen alma);

#endif /* INCLUDE_ALMACEN_H_ */
