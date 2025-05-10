

#include "../include/usuario.h"
#include <stdio.h>

// Funciones para obtener los valores de la estructura
int getIdUsuario(Usuario usu) {
    return usu.id_Usuario;
}

const char* getNombreUsuario(Usuario usu) {
    return usu.nombre_usuario;
}

const char* getContrasenyaUsuario(Usuario usu) { // Nombre corregido
	return usu.contrasenya;
}

int getNivel_usuario(Usuario usu) {
    return usu.nivel;
}

const char* getDireccion_usuario(Usuario usu) {
	return usu.direccion;
}

const char* getNumeroTlf_usuario(Usuario usu) {
    return usu.telefono_usuario;
}

int getIdSuscripcion_usuario(Usuario usu) {
    return usu.id_suscripcion;
}

int getCodigoPostal_usuario(Usuario usu) {
	return usu.codigo_postal;
}


