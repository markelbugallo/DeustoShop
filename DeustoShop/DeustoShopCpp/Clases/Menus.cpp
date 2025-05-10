#include <iostream>
#include "Menus.h"
using namespace std;

void mostrarMenuInicial() {
    int opcion = 0;

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
            cout << "\nMenu de registro...\n\n";
            mostrarMenuPrincipal();
        } else if (opcion == 2)
        {
            cout << "\nMenu de inicio de sesion...\n\n";
            mostrarMenuPrincipal();
        } else if (opcion == 3)
        {
            cout << "\nCerrando el programa...\n";
            break;
        }
    }
}

void mostrarMenuPrincipal() {
    int opcion = 0;

    while (true)
    {
        cout << "MENU\n";
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

