

#ifndef INCLUDE_PROVEEDOR_H_
#define INCLUDE_PROVEEDOR_H_

typedef struct {
	int id_proveedor;
	char nombre[50];
	char contacto[50];
} Proveedor;

int getId_proveedor(Proveedor prov);
const char* getNombre_proveedor(Proveedor prov);
const char* getContacto_proveedor(Proveedor prov);


#endif /* INCLUDE_PROVEEDOR_H_ */
