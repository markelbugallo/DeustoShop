#ifndef MENUSCLIENTE_H
#define MENUSCLIENTE_H

#include "../Clases/Usuario.h"
#include <string>

void mostrarMenuInicial();
Usuario mostrarMenuRegistro();
void mostrarMenuPrincipal(Usuario usuario);
void mostrarProductos(Usuario usuario);
void mostrarHistorialCompras(Usuario usuario);
void mostrarAlmacenes(Usuario usuario);
void mostrarMenuMiPerfil(Usuario usuario);
int mandarAlSevidor(const string &mensaje, string &respuesta);

#endif // MENUSCLIENTE_H