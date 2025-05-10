#include <stdio.h>
#include "../include/proveedor.h"


int getId_proveedor(Proveedor prov){
	return prov.id_proveedor;
}
const char* getNombre_proveedor(Proveedor prov){
	return prov.nombre;
}
const char* getContacto_proveedor(Proveedor prov){
	return prov.contacto;
}


