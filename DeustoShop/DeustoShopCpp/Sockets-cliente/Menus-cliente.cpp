#include <iostream>
#include "Menus-cliente.h"
#include "../Clases/Usuario.h"
using namespace std;

void mostrarMenuInicial() {
    int opcion = 0;
    Usuario usuario_actual;

    while (true)
    {
        cout << "MENU\n";
        cout << "1) Registrarse\n";
        cout << "2) Iniciar sesion\n";
        cout << "3) Salir\n\n";
        cout << "Elija una opcion: ";
        cin >> opcion;

        if (opcion == 1)
        {
            usuario_actual = mostrarMenuRegistro();
            false;
        } else if (opcion == 2)
        {
            cout << "\n\nMenu de inicio de sesion...\n\n";
            //mostrarMenuPrincipal();
        } else if (opcion == 3)
        {
            cout << "\nCerrando el programa...\n";
            break;
        }

        mostrarMenuPrincipal(usuario_actual);
    }
}

Usuario mostrarMenuRegistro() {
    // variables para almacenar la entrada del usuario
    string nombre_usu, contrasena, direccion, email;
    int nivel, tipo_subscripcion, codigo_postal;

    // variable para añadir el usuario nuevo con las variables introducidas por el usuario
    Usuario nuevoUsuario;

    cout << "\nMENU DE REGISTRO\n" << "Introduzca los siguientes apartados...\n";

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

void mostrarMenuPrincipal(Usuario usuario_actual) {
    int opcion = 0;

    while (true)
    {
        cout << "\nMENU PRINCIPAL\n";
        cout << "1) Catalogo de productos\n";
        cout << "2) Historial de compras\n";
        cout << "3) Mi perfil\n";
        cout << "4) Salir\n\n";
        cout << "Elija una opcion: ";
        cin >> opcion;

        if (opcion == 1)
        {
            cout << "\n\nMostrar catalogo\n";
            break;
        } else if (opcion == 2)
        {
            cout << "\n\nMostrar historial de compras\n";
            break;
        } else if (opcion == 3)
        {
            mostrarMenuMiPerfil(usuario_actual);
            break;
        } else if (opcion == 4)
        {
            cout << "\nCerrando sesion...\n\n";
            break;
        }
    }
}

void mostrarMenuMiPerfil(Usuario usuario_actual) {
    int opcion;

    cout << "\n\nMI PERFIL\n" << "---------" << endl;

    // Visualizar datos del usuario actual
    cout << "Nombre de usuario: " << usuario_actual.getNombre_usuario() << endl;
    cout << "Nivel: " << usuario_actual.getNivel() << endl;
    cout << "Direccion: " << usuario_actual.getDireccion() << endl;
    cout << "Email: " << usuario_actual.getContacto_usuario() << endl;
    cout << "Subscripcion: " << usuario_actual.getId_subscripcion() << endl;
    cout << "Codigo Postal: " << usuario_actual.getCodigo_postal() << endl;

    // Funcionalidades
    cout <<  endl << "1) Modificar datos" << endl;
    cout << "2) Eliminar perfil" << endl;
    cout << "3) Volver al menu principal" << endl;
    cout << endl << "Elija una opcion: ";
    cin >> opcion;

    if (opcion == 1)
    {
        cout << "Modificar datos" << endl;
        mostrarMenuMiPerfil(usuario_actual);
    } else if (opcion == 2)
    {
        cout << "Eliminar perfil" << endl;
        mostrarMenuMiPerfil(usuario_actual);
    } else if (opcion == 3)
    {
        cout << "Volviendo al menu principal..." << endl;
        mostrarMenuPrincipal(usuario_actual);
    }
}