#include "../include/bd.h"
#include <stdio.h>
#include "../external/sqlite3.h"
#include <string.h>
#include <stdlib.h>
#include <stddef.h>  // NULL
#include "../include/producto.h"
#include "../include/almacen.h"
#include "../include/usuario.h"
#include "../include/stock.h"
#include "../include/proveedor.h"
#include "../include/pedido.h"
#define NOMBRE_BD "./db/DeustoShop.db"
#define RUTA_CSV_PROD "./data/productos_iniciales.csv"
#define RUTA_CSV_PROVEED "./data/proveedores_iniciales.csv"
#define MAX_LINE 1024

// Abre la base de datos y devuelve 0 si todo va bien
int abrirBaseDatos(sqlite3 **db) {
    int rc = sqlite3_open(NOMBRE_BD, db);
    if (rc != SQLITE_OK) {
        printf("Error al abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    return SQLITE_OK;
}

// Cierra la base de datos
void cerrarBaseDatos(sqlite3 *db) {
    if (db != NULL) {
        sqlite3_close(db);
        printf("Base de datos cerrada.\n");
    }
}

int insertarProducto(sqlite3 *db, Producto *p) {
    const char *sql = "INSERT INTO Producto (id_producto, nombre_producto, descripcion, precio, id_proveedor, categoria) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, p->id_producto);
    sqlite3_bind_text(stmt, 2, p->nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, p->descripcion, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, p->precio);
    sqlite3_bind_int(stmt, 5, p->id_proveedor);
    sqlite3_bind_text(stmt, 6, categoriaToString(p->categoria), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al insertar el producto: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int listarProductos(sqlite3 *db) {
    const char *sql = "SELECT id_producto, nombre_producto, descripcion, precio, id_proveedor, categoria FROM Producto;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, - 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("\n--- LISTA DE PRODUCTOS ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID Producto: %d\n", sqlite3_column_int(stmt, 0));
        printf("Nombre Producto: %s\n", sqlite3_column_text(stmt, 1));
        printf("Descripcion: %s\n", sqlite3_column_text(stmt, 2));
        printf("Precio: %.2f\n", sqlite3_column_double(stmt, 3));
        printf("ID Proveedor: %d\n", sqlite3_column_int(stmt, 4));
        printf("Categoria: %s\n", sqlite3_column_text(stmt, 5));
    }

    sqlite3_finalize(stmt);
    return 1;
}

int actualizarProducto(sqlite3 *db, Producto *p) {
    const char *sql = "UPDATE Producto SET nombre_producto = ?, descripcion = ?, precio = ?, id_proveedor = ?, categoria = ? WHERE id_producto = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, p->nombre, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, p->descripcion, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, p->precio);
    sqlite3_bind_int(stmt, 4, p->id_proveedor);
    sqlite3_bind_text(stmt, 5, categoriaToString(p->categoria), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, p->id_producto);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al actualizar el producto: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int eliminarProducto(sqlite3 *db, int id_producto) {
    const char *sql = "DELETE FROM Producto WHERE id_producto = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, id_producto);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al eliminar el producto: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int insertarAlmacen(sqlite3 *db, Almacen *a) {
    const char *sql = "INSERT INTO Almacen (Nombre, Postal, Id, Direccion, Contacto) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, a->nombre, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, a->codigo);
    sqlite3_bind_int(stmt, 3, a->id_almacen);
    sqlite3_bind_text(stmt, 4, a->direccion, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, a->contacto, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al insertar el almacen: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}


int listarAlmacenes(sqlite3 *db) {
    const char *sql = "SELECT Nombre, Postal, Id, Direccion, Contacto FROM Almacen;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("\n--- LISTA DE ALMACENES ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("Nombre: %s\n", sqlite3_column_text(stmt, 0));
        printf("Codigo Postal: %d\n", sqlite3_column_int(stmt, 1));
        printf("ID Almacen: %d\n", sqlite3_column_int(stmt, 2));
        printf("Direccion: %s\n", sqlite3_column_text(stmt, 3));
        printf("Contacto: %s\n", sqlite3_column_text(stmt, 4));
    }

    sqlite3_finalize(stmt);
    return 1;
}


int actualizarAlmacen(sqlite3 *db, Almacen *a) {
    const char *sql = "UPDATE Almacen SET Nombre = ?, Postal = ?, Direccion = ?, Contacto = ? WHERE Id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, a->nombre, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, a->codigo);
    sqlite3_bind_text(stmt, 3, a->direccion, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, a->contacto, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, a->id_almacen);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al actualizar el almacen: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int eliminarAlmacen(sqlite3 *db, int id_almacen) {
    const char *sql = "DELETE FROM Almacen WHERE Id = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, id_almacen);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al eliminar el almacen: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int insertarUsuario(sqlite3 *db, Usuario *u) {
    const char *sql = "INSERT INTO Usuario (id_usuario, nombre_usuario, contrasena_usuario, nivel, direccion, contacto_usuario, id_suscripcion, codigo_postal) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, u->id_Usuario);
    sqlite3_bind_text(stmt, 2, u->nombre_usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, u->contrasenya, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, u->nivel);
    sqlite3_bind_text(stmt, 5, u->direccion, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, u->telefono_usuario);
    sqlite3_bind_int(stmt, 7, u->id_suscripcion);
    sqlite3_bind_int(stmt, 8, u->codigo_postal);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al insertar el usuario: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int listarUsuarios(sqlite3 *db) {
    const char *sql = "SELECT id_usuario, nombre_usuario, contrasena_usuario, nivel, direccion, contacto_usuario, id_suscripcion, codigo_postal FROM Usuario;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("\n--- LISTA DE USUARIOS ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID Usuario: %d\n", sqlite3_column_int(stmt, 0));
        printf("Nombre Usuario: %s\n", sqlite3_column_text(stmt, 1));
        printf("Contrasena usuario: %s\n", sqlite3_column_text(stmt, 2));
        printf("Nivel: %d\n", sqlite3_column_int(stmt, 3));
        printf("Direccion: %s\n", sqlite3_column_text(stmt, 4));
        printf("Contacto usuario: %d\n", sqlite3_column_int(stmt, 5));
        printf("ID Suscripcion: %d\n", sqlite3_column_int(stmt, 6));
        printf("Codigo Postal: %d\n", sqlite3_column_int(stmt, 7));
    }

    sqlite3_finalize(stmt);
    return 1;
}

int actualizarUsuario(sqlite3 *db, Usuario *u) {
    const char *sql = "UPDATE Usuario SET nombre_usuario = ?, contrasena_usuario = ?, nivel = ?, direccion = ?, contacto_usuario = ?, id_suscripcion = ?, codigo_postal = ? WHERE id_usuario = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_text(stmt, 1, u->nombre_usuario, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, u->contrasenya, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, u->nivel);
    sqlite3_bind_text(stmt, 4, u->direccion, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, u->telefono_usuario);
    sqlite3_bind_int(stmt, 6, u->id_suscripcion);
    sqlite3_bind_int(stmt, 7, u->codigo_postal);
    sqlite3_bind_int(stmt, 8, u->id_Usuario);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al actualizar el usuario: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}

int eliminarUsuario(sqlite3 *db, int id_usuario) {
    const char *sql = "DELETE FROM Usuario WHERE id_usuario = ?;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    sqlite3_bind_int(stmt, 1, id_usuario);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al eliminar el usuario: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return 0;
    }

    sqlite3_finalize(stmt);
    return 1;
}



int listarStock(sqlite3 *db) {
    const char *sql = "SELECT Id_Almacen, Id_Producto, Cantidad, Stock_Minimo FROM Stock;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    printf("\n--- LISTA DE STOCK ---\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        printf("ID Almacen: %d\n", sqlite3_column_int(stmt, 0));
        printf("ID Producto: %d\n", sqlite3_column_int(stmt, 1));
        printf("Cantidad: %d\n", sqlite3_column_int(stmt, 2));
        printf("Stock Minimo: %d\n", sqlite3_column_int(stmt, 3));
    }

    sqlite3_finalize(stmt);
    return 1;
}

Usuario* cargarUsuarios(sqlite3 *db, int *numUsuarios) {
    const char *sql = "SELECT id_usuario, nombre_usuario, contrasena_usuario, nivel, direccion, contacto_usuario, id_suscripcion, codigo_postal FROM Usuario;";
    sqlite3_stmt *stmt;
    Usuario *usuarios = NULL;
    *numUsuarios = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        usuarios = (Usuario*) realloc(usuarios, sizeof(Usuario) * (*numUsuarios + 1));
        usuarios[*numUsuarios].id_Usuario = sqlite3_column_int(stmt, 0);
        strncpy(usuarios[*numUsuarios].nombre_usuario, (const char*)sqlite3_column_text(stmt, 1), sizeof(usuarios[*numUsuarios].nombre_usuario) - 1);
        usuarios[*numUsuarios].nombre_usuario[sizeof(usuarios[*numUsuarios].nombre_usuario) - 1] = '\0';
        strncpy(usuarios[*numUsuarios].contrasenya, (const char*)sqlite3_column_text(stmt, 2), sizeof(usuarios[*numUsuarios].contrasenya) - 1);
        usuarios[*numUsuarios].contrasenya[sizeof(usuarios[*numUsuarios].contrasenya) - 1] = '\0';
        usuarios[*numUsuarios].nivel = sqlite3_column_int(stmt, 3);
        strncpy(usuarios[*numUsuarios].direccion, (const char*)sqlite3_column_text(stmt, 4), sizeof(usuarios[*numUsuarios].direccion) - 1);
        usuarios[*numUsuarios].direccion[sizeof(usuarios[*numUsuarios].direccion) - 1] = '\0';
        usuarios[*numUsuarios].telefono_usuario = sqlite3_column_int(stmt, 5);
        usuarios[*numUsuarios].id_suscripcion = sqlite3_column_int(stmt, 6);
        usuarios[*numUsuarios].codigo_postal = sqlite3_column_int(stmt, 7);
        (*numUsuarios)++;
    }

    sqlite3_finalize(stmt);
    return usuarios;
}

Producto* cargarProductos(sqlite3 *db, int *numProductos) {
    const char *sql = "SELECT id_producto, nombre_producto, descripcion, precio, id_proveedor, categoria FROM Producto;";
    sqlite3_stmt *stmt;
    Producto *productos = NULL;
    *numProductos = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        productos = (Producto*) realloc(productos, sizeof(Producto) * (*numProductos + 1));
        productos[*numProductos].id_producto = sqlite3_column_int(stmt, 0);
        strncpy(productos[*numProductos].nombre, (const char*)sqlite3_column_text(stmt, 1), sizeof(productos[*numProductos].nombre) - 1);
        productos[*numProductos].nombre[sizeof(productos[*numProductos].nombre) - 1] = '\0';
        strncpy(productos[*numProductos].descripcion, (const char*)sqlite3_column_text(stmt, 2), sizeof(productos[*numProductos].descripcion) - 1);
        productos[*numProductos].descripcion[sizeof(productos[*numProductos].descripcion) - 1] = '\0';
        productos[*numProductos].precio = sqlite3_column_double(stmt, 3);
        productos[*numProductos].id_proveedor = sqlite3_column_int(stmt, 4);
        productos[*numProductos].categoria = sqlite3_column_int(stmt, 5);
        (*numProductos)++;
    }

    sqlite3_finalize(stmt);
    return productos;
}

Almacen* cargarAlmacenes(sqlite3 *db, int *numAlmacenes) {
    const char *sql = "SELECT Nombre, Postal, Id, Direccion, Contacto FROM Almacen;";
    sqlite3_stmt *stmt;
    Almacen *almacenes = NULL;
    *numAlmacenes = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        almacenes = (Almacen*) realloc(almacenes, sizeof(Almacen) * (*numAlmacenes + 1));
        strncpy(almacenes[*numAlmacenes].nombre, (const char*)sqlite3_column_text(stmt, 0), sizeof(almacenes[*numAlmacenes].nombre) - 1);
        almacenes[*numAlmacenes].nombre[sizeof(almacenes[*numAlmacenes].nombre) - 1] = '\0';
        almacenes[*numAlmacenes].codigo = sqlite3_column_int(stmt, 1);
        almacenes[*numAlmacenes].id_almacen = sqlite3_column_int(stmt, 2);
        strncpy(almacenes[*numAlmacenes].direccion, (const char*)sqlite3_column_text(stmt, 3), sizeof(almacenes[*numAlmacenes].direccion) - 1);
        almacenes[*numAlmacenes].direccion[sizeof(almacenes[*numAlmacenes].direccion) - 1] = '\0';
        strncpy(almacenes[*numAlmacenes].contacto, (const char*)sqlite3_column_text(stmt, 4), sizeof(almacenes[*numAlmacenes].contacto) - 1);
        almacenes[*numAlmacenes].contacto[sizeof(almacenes[*numAlmacenes].contacto) - 1] = '\0';
        (*numAlmacenes)++;
    }

    sqlite3_finalize(stmt);
    return almacenes;
}

Stock* cargarStock(sqlite3 *db, int *numStock) {
	const char *sql = "SELECT Id, Id_Producto, Stock_Minimo, Cantidad FROM Stock;";
    sqlite3_stmt *stmt;
    Stock *stock = NULL;
    *numStock = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        stock = (Stock*) realloc(stock, sizeof(Stock) * (*numStock + 1));
        stock[*numStock].id_almacen = sqlite3_column_int(stmt, 0);
        stock[*numStock].id_producto = sqlite3_column_int(stmt, 1);
        stock[*numStock].stock_min = sqlite3_column_int(stmt, 2);
        stock[*numStock].cantidad = sqlite3_column_int(stmt, 3);
        (*numStock)++;
    }

    sqlite3_finalize(stmt);
    return stock;
}

Proveedor* cargarProveedores(sqlite3 *db, int *numProveedores) {
    const char *sql = "SELECT id_proveedor, nombre_proveedor, contacto_proveedor FROM Proveedor;";
    sqlite3_stmt *stmt;
    Proveedor *proveedores = NULL;
    *numProveedores = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        proveedores = (Proveedor*) realloc(proveedores, sizeof(Proveedor) * (*numProveedores + 1));
        proveedores[*numProveedores].id_proveedor = sqlite3_column_int(stmt, 0);
        strncpy(proveedores[*numProveedores].nombre, (const char*)sqlite3_column_text(stmt, 1), sizeof(proveedores[*numProveedores].nombre) - 1);
        proveedores[*numProveedores].nombre[sizeof(proveedores[*numProveedores].nombre) - 1] = '\0';
        strncpy(proveedores[*numProveedores].contacto, (const char*)sqlite3_column_text(stmt, 2), sizeof(proveedores[*numProveedores].contacto) - 1);
        proveedores[*numProveedores].contacto[sizeof(proveedores[*numProveedores].contacto) - 1] = '\0';
        (*numProveedores)++;
    }

    sqlite3_finalize(stmt);
    return proveedores;
}

void guardarProductos(sqlite3 *db, Producto *productos, int numProductos) {
    for (int i = 0; i < numProductos; i++) {
        actualizarProducto(db, &productos[i]);
    }
}

void guardarAlmacenes(sqlite3 *db, Almacen *almacenes, int numAlmacenes) {
    for (int i = 0; i < numAlmacenes; i++) {
        actualizarAlmacen(db, &almacenes[i]);
    }
}

void guardarStock(sqlite3 *db, Stock *stock, int numStock) {
    for (int i = 0; i < numStock; i++) {
        actualizarStock(db, &stock[i]);
    }
}

void guardarUsuarios(sqlite3 *db, Usuario *usuarios, int numUsuarios) {
    for (int i = 0; i < numUsuarios; i++) {
        actualizarUsuario(db, &usuarios[i]);
    }
}

int insertarProveedor(sqlite3 *db, Proveedor *p) {
	const char *sql = "INSERT INTO Proveedor (id_proveedor, nombre_proveedor, contacto_proveedor) VALUES (?, ?, ?);";
	sqlite3_stmt *stmt;

	if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
		printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
		return 0;
	}

	sqlite3_bind_int(stmt, 1, p->id_proveedor);
	sqlite3_bind_text(stmt, 2, p->nombre, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, p->contacto, -1, SQLITE_STATIC);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		printf("Error al insertar el proveedor: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 0;
	}

	sqlite3_finalize(stmt);
	return 1;
}

int listarProveedores(sqlite3 *db) {
	const char *sql = "SELECT id_proveedor, nombre_proveedor, contacto_proveedor FROM Proveedor;";
	sqlite3_stmt *stmt;

	if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
		printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
		return 0;
	}

	printf("\n--- LISTA DE PROVEEDORES ---\n");
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		printf("ID Proveedor: %d\n", sqlite3_column_int(stmt, 0));
		printf("Nombre: %s\n", sqlite3_column_text(stmt, 1));
		printf("Contacto: %s\n", sqlite3_column_text(stmt, 2));
	}

	sqlite3_finalize(stmt);
	return 1;
}

int actualizarProveedor(sqlite3 *db, Proveedor *p) {
	const char *sql = "UPDATE Proveedor SET nombre_proveedor = ?, contacto_proveedor = ? WHERE id_proveedor = ?;";
	sqlite3_stmt *stmt;

	if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
		printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
		return 0;
	}

	sqlite3_bind_text(stmt, 1, p->nombre, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, p->contacto, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 3, p->id_proveedor);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		printf("Error al actualizar el proveedor: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 0;
	}

	sqlite3_finalize(stmt);
	return 1;
}

int eliminarProveedor(sqlite3 *db, int id_proveedor) {
	const char *sql = "DELETE FROM Proveedor WHERE id_proveedor = ?;";
	sqlite3_stmt *stmt;

	if (sqlite3_prepare_v2(db, sql, strlen(sql) + 1, &stmt, NULL) != SQLITE_OK) {
		printf("Error al preparar la sentencia: %s\n", sqlite3_errmsg(db));
		return 0;
	}

	sqlite3_bind_int(stmt, 1, id_proveedor);

	if (sqlite3_step(stmt) != SQLITE_DONE) {
		printf("Error al eliminar el proveedor: %s\n", sqlite3_errmsg(db));
		sqlite3_finalize(stmt);
		return 0;
	}

	sqlite3_finalize(stmt);
	return 1;
}

void guardarProveedores(sqlite3 *db, Proveedor *proveedores, int numProveedores) {
	for (int i = 0; i < numProveedores; i++) {
		actualizarProveedor(db, &proveedores[i]);
	}
}

int tablaVacia(sqlite3* db, const char* nombreTabla) {
    char sql[128];
    snprintf(sql, sizeof(sql), "SELECT COUNT(*) FROM %s;", nombreTabla);

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        return 0; // Si hay error, asumimos que no está vacía por seguridad
    }

    int vacia = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int cantidad = sqlite3_column_int(stmt, 0);
        vacia = (cantidad == 0);
    }

    sqlite3_finalize(stmt);
    return vacia;
}

void cargarCSVProducto(sqlite3 *db) {
    FILE *f = fopen(RUTA_CSV_PROD, "r");
    if (!f) {
        perror("No se pudo abrir el CSV");
        return;
    }

    char linea[MAX_LINE];
    // Saltarse cabecera
    fgets(linea, MAX_LINE, f);

    while (fgets(linea, MAX_LINE, f)) {
        char *campo;
        int id_producto = 0, id_proveedor = 0;
        double precio = 0.0;
        char nombre[100] = {0}, descripcion[255] = {0}, categoriaTexto[100] = {0};
        int categoria = OTROS; // Por defecto si no encuentra categoría válida

        // Eliminar salto de línea
        linea[strcspn(linea, "\n")] = 0;

        // Separar por comas
        campo = strtok(linea, ",");
        if (campo) sscanf(campo, "%d", &id_producto);

        campo = strtok(NULL, ",");
        if (campo) strncpy(nombre, campo, sizeof(nombre) - 1);

        campo = strtok(NULL, ",");
        if (campo) strncpy(descripcion, campo, sizeof(descripcion) - 1);

        campo = strtok(NULL, ",");
        if (campo) sscanf(campo, "%lf", &precio);

        campo = strtok(NULL, ",");
        if (campo) sscanf(campo, "%d", &id_proveedor);

        campo = strtok(NULL, ",");
        if (campo) strncpy(categoriaTexto, campo, sizeof(categoriaTexto) - 1);

        // Convertir texto de categoría a enum
        if (strcmp(categoriaTexto, "ELECTRONICA") == 0) {
            categoria = ELECTRONICA;
        } else if (strcmp(categoriaTexto, "ALIMENTACION") == 0) {
            categoria = ALIMENTACION;
        } else if (strcmp(categoriaTexto, "LIBRERIA") == 0) {
            categoria = LIBRERIA;
        } else if (strcmp(categoriaTexto, "JUGUETERIA") == 0) {
            categoria = JUGUETERIA;
        } else if (strcmp(categoriaTexto, "ROPA") == 0) {
            categoria = ROPA;
        } else if (strcmp(categoriaTexto, "HOGAR") == 0) {
            categoria = HOGAR;
        } else if (strcmp(categoriaTexto, "DEPORTE") == 0) {
            categoria = DEPORTE;
        } else {
            categoria = OTROS; // Si no coincide, ponemos OTROS
        }

        // Crear consulta SQL
        char sql[MAX_LINE];
        snprintf(sql, sizeof(sql),
            "INSERT INTO Producto (id_producto, nombre_producto, descripcion, precio, id_proveedor, categoria) "
            "VALUES (%d, '%s', '%s', %.2f, %d, %d);",
            id_producto, nombre, descripcion, precio, id_proveedor, categoria
        );

        // Ejecutar
        char *errMsg = NULL;
        if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK) {
            printf("Error insertando: %s\n", errMsg);
            sqlite3_free(errMsg);
        }
    }

    fclose(f);
}

void cargarCSVProveedor(sqlite3 *db) {
    FILE *f = fopen(RUTA_CSV_PROVEED, "r");
    if (!f) {
        perror("No se pudo abrir el CSV");
        return;
    }

    char linea[MAX_LINE];
    // Saltarse cabecera
    fgets(linea, MAX_LINE, f);

    while (fgets(linea, MAX_LINE, f)) {
        char *campo;
        Proveedor proveedor;

        // Eliminar salto de línea
        linea[strcspn(linea, "\n")] = 0;

        // Separar por comas
        campo = strtok(linea, ",");
        if (campo) sscanf(campo, "%d", &proveedor.id_proveedor);

        campo = strtok(NULL, ",");
        if (campo) strncpy(proveedor.nombre, campo, sizeof(proveedor.nombre) - 1);

        campo = strtok(NULL, ",");
        if (campo) strncpy(proveedor.contacto, campo, sizeof(proveedor.contacto) - 1);

        // Crear consulta SQL
        char sql[MAX_LINE];
        snprintf(sql, sizeof(sql),
            "INSERT INTO Proveedor (id_proveedor, nombre_proveedor, contacto_proveedor) "
            "VALUES (%d, '%s', '%s');",
            proveedor.id_proveedor, proveedor.nombre, proveedor.contacto
        );

        // Ejecutar la consulta SQL
        char *errMsg = NULL;
        if (sqlite3_exec(db, sql, NULL, NULL, &errMsg) != SQLITE_OK) {
            sqlite3_free(errMsg);
        }
    }

    fclose(f);
}

void cargarStockAlmacenes(sqlite3 *db, Almacen *almacenes, int numAlmacenes, Producto *productos, int numProductos) {
    // Initialize product and stock arrays in each warehouse
    for (int i = 0; i < numAlmacenes; i++) {
        almacenes[i].productos = NULL;
        almacenes[i].numProductos = 0;
        almacenes[i].stock = NULL;
        almacenes[i].numStock = 0;
    }

    // Query to get stock data
    const char *sql = "SELECT Id_Almacen, Id_Producto, Cantidad FROM Stock;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error preparing stock query: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Iterate through the query results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int idAlmacen = sqlite3_column_int(stmt, 0);
        int idProducto = sqlite3_column_int(stmt, 1);
        int cantidad = sqlite3_column_int(stmt, 2);

        // Find the corresponding warehouse
        for (int j = 0; j < numAlmacenes; j++) {
            if (almacenes[j].id_almacen == idAlmacen) {
                // Find the corresponding product
                for (int k = 0; k < numProductos; k++) {
                    if (productos[k].id_producto == idProducto) {
                        // Add the product to the warehouse's product array
                        almacenes[j].productos = (Producto *) realloc(almacenes[j].productos, sizeof(Producto) * (almacenes[j].numProductos + 1));
                        almacenes[j].productos[almacenes[j].numProductos] = productos[k];
                        almacenes[j].numProductos++;

                        // Add the quantity to the warehouse's stock array
                        almacenes[j].stock = (int *) realloc(almacenes[j].stock, sizeof(int) * (almacenes[j].numStock + 1));
                        almacenes[j].stock[almacenes[j].numStock] = cantidad;
                        almacenes[j].numStock++;

                        break; // Exit the product loop
                    }
                }
                break; // Exit the warehouse loop
            }
        }
    }

    sqlite3_finalize(stmt); // Free query resources
}

void liberarAlmacenes(Almacen *almacenes, int numAlmacenes) {
	for (int i = 0; i < numAlmacenes; i++) {
		free(almacenes[i].productos);
		free(almacenes[i].stock);
	}
	free(almacenes);
}
void liberarProductos(Producto *productos) {
	free(productos);
}
void liberarUsuarios(Usuario *usuarios) {
	free(usuarios);
}
void liberarProveedores(Proveedor *proveedores) {
	free(proveedores);
}










