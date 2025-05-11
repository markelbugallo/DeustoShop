#include "Usuario.h"

// constructor
Usuario::Usuario() : id_usuario(0), nombre_usuario("a"), contrasena_usuario("a"), nivel(0), direccion("a1"), contacto_usuario("a@a"), id_subscripcion(0), codigo_postal(48000) {}

Usuario::Usuario(int id_usuario, 
    const string &nombre_usuario, 
    const string &contrasena_usuario, 
    int nivel, 
    const string &direccion, 
    const string &contacto_usuario, 
    int id_subscripcion, 
    int codigo_postal
) : id_usuario(id_usuario), nombre_usuario(nombre_usuario), contrasena_usuario(contrasena_usuario), nivel(nivel), direccion(direccion), contacto_usuario(contacto_usuario), id_subscripcion(id_subscripcion), codigo_postal(codigo_postal) {}


// getters
int Usuario::getId_usuario() const {
    return id_usuario;
}

string Usuario::getNombre_usuario() const {
    return nombre_usuario;
}

string Usuario::getContrasena_usuario() const {
    return contrasena_usuario;
}

int Usuario::getNivel() const {
    return nivel;
}

string Usuario::getDireccion() const {
    return direccion;
}

string Usuario::getContacto_usuario() const {
    return contacto_usuario;
}

int Usuario::getId_subscripcion() const {
    return id_subscripcion;
}

int Usuario::getCodigo_postal() const {
    return codigo_postal;
}


// Setters
void Usuario::setId_usuario(int id) {
    id_usuario = id;
}

void Usuario::setNombre_usuario(const std::string& nombre) {
    nombre_usuario = nombre;
}

void Usuario::setContrasena_usuario(const std::string& contrasena) {
    contrasena_usuario = contrasena;
}

void Usuario::setNivel(int n) {
    nivel = n;
}

void Usuario::setDireccion(const std::string& dir) {
    direccion = dir;
}

void Usuario::setContacto_usuario(const std::string& contacto) {
    contacto_usuario = contacto;
}

void Usuario::setId_subscripcion(int id_sub) {
    id_subscripcion = id_sub;
}

void Usuario::setCodigo_postal(int cp) {
    codigo_postal = cp;
}
