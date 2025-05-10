

#ifndef CODIGO_MAIN_C_
#define CODIGO_MAIN_C_

#include <stdio.h>
#include "../include/bd.h"         // Acceso a abrirBaseDatos y cerrarBaseDatos
#include "../include/menu.h"


int main(void) {

    // Abrir base de datos
	arrancarBD();

    // Llamar al menú principal y pasarle la conexión a la BD
    menu();

    // Cerrar la base de datos antes de salir
    // cerrarBaseDatos(db); La ponemos en el menu para que se cierre segun se sale de la aplicación

    return 0;
    menu();
}


#endif /* CODIGO_MAIN_C_ */
