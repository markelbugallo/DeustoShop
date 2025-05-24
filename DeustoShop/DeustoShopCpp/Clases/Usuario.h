#ifndef USUARIO_H
#define USUARIO_H
#include <string>
#include <vector>
using namespace std;

class Usuario {
private:
    int id_usuario;
    string nombre_usuario;
    string contrasena_usuario;
    string contacto_usuario;
    int id_subscripcion;
    string direccion;
    int codigo_postal;

public:
    Usuario();
    Usuario(
        int id_usuario,
        const string &nombre_usuario, 
        const string &contrasena_usuario, 
        const string &contacto_usuario,
        int id_subscripcion,
        const string &direccion,
        int codigo_postal
    );
    
    // getters
    int getId_usuario() const;
    string getNombre_usuario() const;
    string getContrasena_usuario() const;
    string getContacto_usuario() const;
    int getId_subscripcion() const;
    string getDireccion() const;
    int getCodigo_postal() const;

    // Setters
    void setId_usuario(int id);
    void setNombre_usuario(const string &nombre);
    void setContrasena_usuario(const string &contrasena);
    void setContacto_usuario(const string &contacto);
    void setId_subscripcion(int id_sub);
    void setDireccion(const string &dir);
    void setCodigo_postal(int cp);

    // funciones
    void imprimirUsuario(Usuario usuarioactual);
    // Carga un usuario desde un CSV
      vector<Usuario> cargarUsuariosCSV(const string& filename);
    // Modifica un usuario por ID
    void modificarUsuarioPorId(Usuario& usuarioActual); 
    // Guarda todos los usuarios en un CSV
     void guardarUsuariosCSV(const string& filename, const vector<Usuario>& usuarios);

};

#endif // USUARIO_H

//dwdwdw