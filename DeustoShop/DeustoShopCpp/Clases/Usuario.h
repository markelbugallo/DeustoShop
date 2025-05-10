#ifndef USUARIO_H
#define USUARIO_H
#include <string>
using namespace std;

class Usuario {
private:
    int id_usuario;
    string nombre_usuario;
    string contrasena_usuario;
    int nivel;
    string direccion;
    string contacto_usuario;
    int id_subscripcion;
    int codigo_postal;

public:
    Usuario();
    Usuario(
        int id_usuario, 
        const string &nombre_usuario, 
        const string &contrasena_usuario, 
        int nivel, 
        const string &direccion, 
        const string &contacto_usuario, 
        int id_subscripcion, 
        int codigo_postal
    );
    
    int getId_usuario() const;
    string getNombre_usuario() const;
    string getContrasena_usuario() const;
    int getNivel() const;
    string getDireccion() const;
    string getContacto_usuario() const;
    int getId_subscripcion() const;
    int getCodigo_postal() const;
};

#endif // USUARIO_H