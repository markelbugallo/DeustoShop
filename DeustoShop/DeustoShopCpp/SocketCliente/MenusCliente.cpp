#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <sstream>
#include "MenusCliente.h"
#include "../Clases/Usuario.h"
#include "../BD/Bd.h"
#include "../Clases/Almacen.h"
using namespace std;
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000


int mandarAlServidor(const string &mensaje, string &respuesta) {
    WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "Failed. Error Code : " << to_string(WSAGetLastError()) << endl;
		return 1;
	}

    //SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		cout << "Could not create socket : " << to_string(WSAGetLastError()) << endl;
		WSACleanup();
		return 1;
	}

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

    //CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		cout << "Connection error: " << to_string(WSAGetLastError()) << endl;
		closesocket(s);
		WSACleanup();
		return 1;
	}

    send(s, mensaje.c_str(), mensaje.length(), 0);
    int bytesReceived = recv(s, recvBuff, sizeof(recvBuff), 0);
    recvBuff[bytesReceived] = '\0';
    respuesta = recvBuff;

    // CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();

    return 0;
}

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
            cout << "\n\nProbando server...\n\n";
            string respuesta;
            if (mandarAlServidor("VerPERFIL", respuesta) == 0)
            {
                //mostrarMenuPrincipal();
            }
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
    int tipo_subscripcion, codigo_postal, id_usuario;

    // variable para añadir el usuario nuevo con las variables introducidas por el usuario
    Usuario nuevoUsuario;

    cout << "\nMENU DE REGISTRO\n" << "Introduzca los siguientes apartados...\n";

    cout << "Nombre de usuario: ";
    cin >> nombre_usu;
    nuevoUsuario.setNombre_usuario(nombre_usu);

    cout << "Contrasenya: ";
    cin >> contrasena;
    nuevoUsuario.setContrasena_usuario(contrasena);

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

    // el id se asigna automaticamente te pone +1 al ultimo registrado
    cout << "ID: ";
    cin >> id_usuario;
    nuevoUsuario.setId_usuario(id_usuario);


    return nuevoUsuario;
}

void mostrarMenuPrincipal(Usuario usuario_actual) {
    int opcion = 0;

    while (true)
    {
        cout << "\nMENU PRINCIPAL\n";
        cout << "1) Catalogo de productos\n";
        cout << "2) Historial de compras\n";
        cout << "3) Mostrar almacenes\n";
        cout << "4) Mi perfil\n";
        cout << "5) Salir\n\n";
        cout << "Elija una opcion: ";
        cin >> opcion;

        if (opcion == 1)
        {
            mostrarProductos(usuario_actual);
            break;
        } else if (opcion == 2)
        {
            mostrarHistorialCompras(usuario_actual);
            break;
        } else if (opcion == 3)
        {
            mostrarAlmacenes(usuario_actual);
        } else if (opcion == 4)
        {
            mostrarMenuMiPerfil(usuario_actual);
            break;
        } else if (opcion == 5)
        {
            cout << "\nCerrando sesion...\n\n";
            break;
        }
    }
}

void mostrarProductos(Usuario usuario_actual) {
    int opcion;
    cout << endl << "PRODUCTOS" << endl;

    vector<Producto> productos = cargarProductosCSV("../DeustoShopC/Data/productos.csv");

    for (size_t i = 0; i < productos.size(); i++) {
        cout << productos[i].getId_producto() << productos[i].getNombre_producto() << productos[i].getPrecio() << endl;
    }

    cout << endl << "Pulsa 1 para volver al menu principal: ";
    cin >> opcion;

    cout << endl;

    if (opcion == 1)
    {
        mostrarMenuPrincipal(usuario_actual);
    }
    
}

void mostrarHistorialCompras(Usuario usuario_actual) {
    int opcion;
    cout << endl << endl << "Pedidos de " << usuario_actual.getNombre_usuario() << endl;
    vector<Pedido> pedidos = cargarPedidosCSV("../DeustoShopC/Data/pedidos.csv");

    for (size_t i = 0; i < pedidos.size(); i++)
    {
        if (pedidos[i].getId_usuario() == usuario_actual.getId_usuario()) {

            pedidos[i].imprimirInfPedido();
            
            }
    }

    cout << endl << "Pulsa 1 para volver al menu principal: ";
    cin >> opcion;

    cout << endl;
    if (opcion == 1)
    {
        mostrarMenuPrincipal(usuario_actual);
    }
}

void mostrarAlmacenes(Usuario usuario_actual) {
    int opcion;
    cout << endl << "ALMACENES" << endl;
    vector<Almacen> almacenes = cargarAlmacenesCSV("../DeustoShopC/Data/almacenes.csv");
    
    for (size_t i = 0; i < almacenes.size(); i++)
    {
        almacenes[i].imprimirAlmacen();
    }

    cout << endl << "Pulsa 1 para volver al menu principal: ";
    cin >> opcion;

    cout << endl;
    if (opcion == 1)
    {
        mostrarMenuPrincipal(usuario_actual);
    }
    
}

void mostrarMenuMiPerfil(Usuario usuario_actual) {
    int opcion;

    cout << "\n\nMI PERFIL\n" << "---------" << endl;

    // Visualizar datos del usuario actual
    usuario_actual.imprimirUsuario(usuario_actual);


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