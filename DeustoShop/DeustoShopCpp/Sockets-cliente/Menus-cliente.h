#ifndef MENUS_H
#define MENUS_H
#include "../Clases/Usuario.h"
#include <string>

void mostrarMenuInicial();
Usuario mostrarMenuRegistro();
void mostrarMenuPrincipal(Usuario usuario);
void mostrarProductos(Usuario usuario);
void mostrarHistorialCompras(Usuario usuario);
void mostrarMenuMiPerfil(Usuario usuario);
int mandarAlSevidor(const string &mensaje, string &respuesta);

#endif // MENUS_H