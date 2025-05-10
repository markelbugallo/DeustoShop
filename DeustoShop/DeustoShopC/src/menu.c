

#include <stdio.h>
#include <stdlib.h>
#include "../include/menu.h"
#include <string.h>
#include "../include/producto.h"
#include "../include/bd.h"
#define MAX_LINE 100
#define MAX_Usuario 50
#define MAX_contrasenya 50

sqlite3 *db; // Variable global para la base de datos

int arrancarBD()
{
	// Abrir base de datos
	if (abrirBaseDatos(&db) != SQLITE_OK) {
		printf("No se pudo abrir la base de datos. Terminando programa...\n");
		return 1;
	}

	return 0;
}

int cerrarAplicacion()
{
	printf("\n");
	printf("Se esta cerrado la aplicacion...\n");
	cerrarBaseDatos(db);
	exit(0);
	return 0;
}

int pedirOpcion(int opcionesPosibles) {
	int num;
	char str[MAX_LINE];
	char *p;

	printf("Opcion: ");
	fflush(stdout);
	fgets(str, MAX_LINE, stdin);

	// Elimina el salto de línea si existe
    str[strcspn(str, "\n")] = 0;
	sscanf(str, "%i", &num);

	// Comprobar que la cadena solo contiene dígitos
    for (p = str; *p != '\0'; p++) {
        if (!isdigit(*p)) {
            printf("\nError, solo se permiten numeros.\n");
            return pedirOpcion(opcionesPosibles);
        }
    }

	if(0 < num && num <= opcionesPosibles) {
		return num;
	} else {
		printf("\n");
		printf("Prueba otra vez\n");

		return pedirOpcion(opcionesPosibles);
	}
}

int pedirID() {
	int id;
	char str[MAX_LINE];
	char *p;

	printf("Introduce el id: ");
	fflush(stdout);
	fgets(str, MAX_LINE, stdin);
	sscanf(str, "%i", &id);

	// Elimina el salto de línea si existe
    str[strcspn(str, "\n")] = 0;

	// Elimina el salto de línea si existe
    str[strcspn(str, "\n")] = 0;

    // Comprobar que la cadena solo contiene dígitos
    for (p = str; *p != '\0'; p++) {
        if (!isdigit(*p)) {
            printf("\nError, solo se permiten numeros.\n");
            return pedirID();
        }
    }
	return id;
}

void menuInicio()
{
	printf("MENU DE DEUSTOSHOP\n");
	printf("\n");
	printf("1) Identificarse\n");
	printf("2) Registrarse\n");
	printf("3) Salir\n");
}

void menuIdentificacion() {
	int userCorrecto = 0;
	int passwordCorrecto = 0;
	char str[MAX_LINE];

	printf("\n");
	printf("MENU DE IDENTIFICACION\n");

	do {
		fflush(stdout);
		printf("Nombre de usuario: ");
		fflush(stdout);
		fgets(str, MAX_LINE, stdin);

		// Eliminar el salto de línea al final de la cadena
        str[strcspn(str, "\n")] = 0;

		// Comprobar si la entrada es "admin"
        if (strcmp(str, "admin") == 0) {
            userCorrecto = 1;  // El nombre de usuario es correcto
        } else {
            printf("\nNombre de usuario incorrecto\n\n");
        }

	} while (userCorrecto == 0);

	do {
		fflush(stdout);
		printf("Contrasenya: ");
		fflush(stdout);
		fgets(str, MAX_LINE, stdin);

		// Eliminar el salto de línea al final de la cadena
        str[strcspn(str, "\n")] = 0;

		// Comprobar si la contraseña es "a"
        if (strcmp(str, "a") == 0) {
            passwordCorrecto = 1;  // La contraseña es correcta
        } else {
            printf("\nContrasenya incorrecta\n\n");
        }

	} while (passwordCorrecto == 0);
}

void menuRegistro() {
	char nombreUsuario[MAX_Usuario];
	char contrasenyaUsuario[MAX_contrasenya];
	printf("\n");
	printf("REGISTRO\n");
	printf("Ingrese un nombre de usuario: ");
	fgets(nombreUsuario, MAX_Usuario, stdin);
	nombreUsuario[strcspn(nombreUsuario, "\n")] = 0;

	printf("Ingrese una contrasenya: ");
	fgets(contrasenyaUsuario, MAX_contrasenya, stdin);
	contrasenyaUsuario[strcspn(contrasenyaUsuario, "\n")] = 0;

	//falta hacer que las respuestas se metan en la bd
	printf("\nUsuario registrado con exito.\n");

}

void menuProveedor()
{
	int opcionesPosibles = 5;
	printf("\n");
	printf("MENU DE PROVEEDORES\n");
	printf("1) Si deseas ver todos los proveedores\n");
	printf("2) Si deseas anyadir un proveedor\n");
	printf("3) Si deseas modificar un proveedor\n");
	printf("4) Si deseas eliminar un proveedor\n");
	printf("5) Volver al menu principal\n\n");

	int opcion = pedirOpcion(opcionesPosibles);

	switch (opcion) {
		case 1:
			printf("VER LISTA DE PROVEEDORES\n");
			listarProveedores(db);
			break;

		case 2:
			printf("ANYADIR UN PROVEEDOR\n");
			break;

		case 3:
			printf("MODIFICAR UN PROVEEDOR\n");
			break;

		case 4:
			printf("ELIMINAR UN PROVEEDOR\n");
			int id = pedirID();
			eliminarProveedor(db, id);
			break;

		case 5:
			menuPrincipal();
			break;

	}
}

void menuProducto() {
	int opcionesPosibles = 6;
	printf("\n");
	printf("MENU PRODUCTOS\n");
	printf("1) Si deseas ver todos los productos\n");
	printf("2) Si deseas buscar un porducto\n");
	printf("3) Si deseas anyadir un producto\n");
	printf("4) Si deseas modificar un producto\n");
	printf("5) Si deseas eliminar un producto\n");
	printf("6) Volver al menu principal\n\n");

	int opcion = pedirOpcion(opcionesPosibles);

	switch (opcion) {
		case 1:
			printf("VER TODOS LOS PRODUCTOS\n");
			listarProductos(db);
			break;

		case 2:
			printf("VER UN PRODUCTO\n");
			break;

		case 3:
			printf("ANYADIR UN PRODUCTO\n");
			break;

		case 4:
			printf("MODIFICAR UN PRODUCTO\n");
			break;

		case 5:
			printf("ELIMINAR UN PRODUCTO\n");
			int id = pedirID();
			eliminarProducto(db, id);
			break;

		case 6:
			menuPrincipal();
			break;
	}
}

void menuAlmacen() {
	int opcionesPosibles = 5;
	printf("\n");
	printf("MENU ALMACEN\n");
	printf("1) Si deseas ver todos los almacenes\n");
	printf("2) Si deseas anyadir un almacen\n");
	printf("3) Si deseas modificar un almacen\n");
	printf("4) Si deseas eliminar un almacen\n");
	printf("5) Volver al menu principal\n\n");

	int opcion = pedirOpcion(opcionesPosibles);

	switch (opcion) {
		case 1:
			printf("VER ALMACENES\n");
			listarAlmacenes(db);
			break;

		case 2:
			printf("ANYADIR ALMACEN\n");
			break;

		case 3:
			printf("MODIFICAR ALMACEN\n");
			break;

		case 4:
			printf("ELIMINAR ALMACEN\n");
			int id = pedirID();
			eliminarAlmacen(db, id);
			break;

		case 5:
			menuPrincipal();
			break;
	}
}

void menuUsuario() {
	int opcionesPosibles = 5;
	printf("\n");
	printf("MENU USUARIOS\n");
	printf("1) Si deseas ver todos los usuarios\n");
	printf("2) Si deseas anyadir un usuario\n");
	printf("3) Si deseas modificar un usuario\n");
	printf("4) Si deseas eliminar un usuario\n");
	printf("5) Volver al menu principal\n\n");

	int opcion = pedirOpcion(opcionesPosibles);

	switch (opcion) {
		case 1:
			printf("VER LISTA DE USUARIOS\n");
			listarUsuarios(db);
			break;

		case 2:
			printf("ANYADIR UN USUARIO\n");
			break;

		case 3:
			printf("MODIFICAR UN USUARIO\n");
			break;

		case 4:
			printf("ELIMINAR UN USUARIO\n");
			int id = pedirID();
			eliminarUsuario(db, id);
			break;

		case 5:
			menuPrincipal();
			break;
	}
}

void menuPrincipal() {
	int opcionesPosibles = 5;
	printf("\n");
	printf("MENU PRINCIPAL\n");
	printf("1) Menu de proveedores\n");
	printf("2) Menu de productos\n");
	printf("3) Menu de almacenes\n");
	printf("4) Menu de usuarios\n");
	printf("5) Salir\n");

	int opcion = pedirOpcion(opcionesPosibles);

	switch (opcion) {
		case 1:
			menuProveedor();
			break;

		case 2:
			menuProducto();
			break;

		case 3:
			menuAlmacen();
			break;

		case 4:
			menuUsuario();
			break;

		case 5:
			cerrarAplicacion();
			break;
	}
}

void menu() {
	int funcionarMenuInicio = 1;
	arrancarBD();

	if (tablaVacia(db, "Producto")) {
		cargarCSVProducto(db);
	} else {
		printf("La tabla 'Producto' ya tiene datos. No se cargara el CSV.\n\n");
	}
	
	if (tablaVacia(db, "Proveedor")) {
		cargarCSVProveedor(db);
	} else {
		printf("La tabla 'Proveedor' ya tiene datos. No se cargara el CSV.\n\n");
	}
	

	do {
		menuInicio();
		int opcion = pedirOpcion(3);

		if (opcion == 1) {
			// proceso de logearse
			funcionarMenuInicio = 0;
			menuIdentificacion();
			printf("\n");
		} else if (opcion == 2) {
			// proceso de registro
			funcionarMenuInicio = 0;
			menuRegistro();
			printf("\n");
		} else if (opcion == 3) {
			funcionarMenuInicio = 0;
			cerrarAplicacion();
		} else {
			printf("Prueba otra vez\n");
			printf("\n");
		}

	} while (funcionarMenuInicio == 1);

	menuPrincipal();

}

