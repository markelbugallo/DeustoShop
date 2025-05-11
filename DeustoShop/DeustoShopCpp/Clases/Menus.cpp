#include <iostream>
#include "Menus.h"
using namespace std;

void mostrarMenuInicial() {
    int opcion = 0;
    Usuario usuario;

    while (true)
    {
        cout << "MENU\n";
        cout << "1) Registrarse\n";
        cout << "2) Iniciar sesion\n";
        cout << "3) Salir\n\n";
        cout << "Elija una opcion: \n";
        cin >> opcion;

        if (opcion == 1)
        {
            usuario = mostrarMenuRegistro();
            false;
        } else if (opcion == 2)
        {
            cout << "\nMenu de inicio de sesion...\n\n";
            //mostrarMenuPrincipal();
        } else if (opcion == 3)
        {
            cout << "\nCerrando el programa...\n";
            break;
        }

        mostrarMenuPrincipal(usuario);
    }
}

void mostrarMenuPrincipal(Usuario usuario) {
    int opcion = 0;

    while (true)
    {
        cout << "\n\nMENU\n";
        cout << "1) Catalogo de productos\n";
        cout << "2) Historial de compras\n";
        cout << "3) Mi perfil\n";
        cout << "4) Salir\n\n";
        cout << "Elija una opcion: \n";
        cin >> opcion;

        if (opcion == 1)
        {
            cout << "\nMostrar catalogo\n";
            break;
        } else if (opcion == 2)
        {
            cout << "\nMostrar historial de compras\n";
            break;
        } else if (opcion == 3)
        {
            cout << "\nMostrar mi perfil\n";
            break;
        } else if (opcion == 4)
        {
            cout << "\nVolviendo al menu inicial...\n\n";
            break;
        }
    }
}

Usuario mostrarMenuRegistro() {
    string nombre_usu, contrasena, direccion, email;
    int nivel, tipo_subscripcion, codigo_postal;

    Usuario nuevoUsuario;

    cout << "\nMenu de registro\n";
    cout << "Introduzca los siguientes apartados...\n\n";

    cout << "Nombre de usuario: ";
    cin >> nombre_usu;
    nuevoUsuario.setNombre_usuario(nombre_usu);

    cout << "Contrasenya: ";
    cin >> contrasena;
    nuevoUsuario.setContrasena_usuario(contrasena);

    cout << "Nivel: ";
    cin >> nivel;
    nuevoUsuario.setNivel(nivel);

    cout << "Direccion: ";
    cin.ignore(); // Limpiar buffer antes de getline
    getline(cin, direccion);
    nuevoUsuario.setDireccion(direccion);

    cout << "Email: ";
    getline(cin, email);
    nuevoUsuario.setContacto_usuario(email);

    cout << "Tipo de subscripcion: ";
    cin >> tipo_subscripcion;
    nuevoUsuario.setId_subscripcion(tipo_subscripcion);

    cout << "Codigo Postal: ";
    cin >> codigo_postal;
    nuevoUsuario.setCodigo_postal(codigo_postal);

    return nuevoUsuario;
}

