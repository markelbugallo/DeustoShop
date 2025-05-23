#include "Usuario.h"
using namespace std;
#include <iostream>
#include <string>

// constructor vacio
Usuario::Usuario()
    : id_usuario(0),
    nombre_usuario(""),
    contrasena_usuario(""),
    contacto_usuario(""),
    id_subscripcion(0),
    direccion(""),
    codigo_postal(0)
{
}

// con parametros
Usuario::Usuario(
    int id_usuario,
    const string &nombre_usuario, 
    const string &contrasena_usuario, 
    const string &contacto_usuario,
    int id_subscripcion,
    const string &direccion,
    int codigo_postal
) : id_usuario(id_usuario), nombre_usuario(nombre_usuario), contrasena_usuario(contrasena_usuario), contacto_usuario(contacto_usuario), id_subscripcion(id_subscripcion), direccion(direccion), codigo_postal(codigo_postal) {}

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

string Usuario::getContacto_usuario() const {
    return contacto_usuario;
}

int Usuario::getId_subscripcion() const {
    return id_subscripcion;
}

string Usuario::getDireccion() const {
    return direccion;
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

void Usuario::setContacto_usuario(const std::string& contacto) {
    contacto_usuario = contacto;
}

void Usuario::setId_subscripcion(int id_sub) {
    id_subscripcion = id_sub;
}

void Usuario::setDireccion(const std::string& dir) {
    direccion = dir;
}

void Usuario::setCodigo_postal(int cp) {
    codigo_postal = cp;
}
void Usuario::imprimir(Usuario usuarioactual) {
    cout << "ID: " << usuarioactual.getId_usuario() << endl;
    cout << "Nombre de usuario: " << usuarioactual.getNombre_usuario() << endl;
    cout << "Contrasenya: " << usuarioactual.getContrasena_usuario() << endl;
    cout << "Direccion: " << usuarioactual.getDireccion() << endl;
    cout << "Email: " << usuarioactual.getContacto_usuario() << endl;
    cout << "Tipo de subscripcion: " << usuarioactual.getId_subscripcion() << endl;
    cout << "Codigo Postal: " << usuarioactual.getCodigo_postal() << endl;
}