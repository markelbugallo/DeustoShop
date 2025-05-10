
#include <stdlib.h>  // atoi (Parse string to int)
#include <stddef.h>  // NULL
#include "../include/almacen.h"

Zona obtenerZona(int codigoPostal) {
    int prefijo = codigoPostal / 1000; // Obtener los dos primeros dígitos

    if (prefijo == 1 || prefijo == 20 || prefijo == 31 || prefijo == 39 || prefijo == 48) {
        return ZONA_NORTE;
    } else if (prefijo == 8 || prefijo == 17 || prefijo == 25 || prefijo == 43 || prefijo == 50 || prefijo == 22 || prefijo == 44) {
        return ZONA_NORESTE;
    } else if (prefijo == 5 || prefijo == 16 || prefijo == 19 || prefijo == 28 || prefijo == 40 || prefijo == 45 || prefijo == 9 || prefijo == 26 || prefijo == 42 || prefijo == 47) {
        return ZONA_CENTRO;
    } else if (prefijo == 3 || prefijo == 30 || prefijo == 46 || prefijo == 2 || prefijo == 13 || prefijo == 12) {
        return ZONA_SURESTE;
    } else if (prefijo == 4 || prefijo == 7 || prefijo == 8 || prefijo == 11 || prefijo == 14 || prefijo == 18 || prefijo == 21 || prefijo == 23 || prefijo == 29 || prefijo == 35 || prefijo == 38 || prefijo == 41 || prefijo == 51 || prefijo == 52) {
        return ZONA_SUR;
    } else {
        return ZONA_OESTE;
    }
}


Almacen* buscarAlmacenPorCodigoPostal(Almacen *almacenes, int numAlmacenes, int codigoPostal) {
    Zona zona = obtenerZona(codigoPostal);
    for (int i = 0; i < numAlmacenes; i++) {
        int almacenCodigoPostal = atoi(almacenes[i].codigo);
        if (obtenerZona(almacenCodigoPostal) == zona) {
            return &almacenes[i];
        }
    }
    return NULL;
}

int getId_alamacen(Almacen alma){
	return alma.id_almacen;
}
const char* getNombreAlmacen(Almacen alma){
	return alma.nombre;
}
int getcodigoAlmacen(Almacen alma){
	return alma.codigo;
}
const char* getDireccionAlamcen(Almacen alma){
	return alma.direccion;
}
const char* getContactoAlmacen(Almacen alma){
	return alma.contacto;
}
