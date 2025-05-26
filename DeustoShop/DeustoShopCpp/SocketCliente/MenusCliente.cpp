#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <sstream>
#include "MenusCliente.h"
#include "../Clases/Usuario.h"
#include "../BD/Bd.h"
#include "../Clases/Almacen.h"
#include "../Clases/Utils.h"
#include <set>
using namespace std;
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000


void MenusCliente::cargarDatos() {
    // vaciar listas
    almacenes.clear();
    pedidos.clear();
    productos.clear();
    proveedores.clear();
    subscripciones.clear();
    usuarios.clear();

    // volver a cargar
    almacenes = cargarAlmacenesCSV("../DeustoShopC/Data/almacenes.csv");
    pedidos = cargarPedidosCSV("../DeustoShopC/Data/pedidos.csv");
    productos = cargarProductosCSV("../DeustoShopC/Data/productos.csv");
    proveedores = cargarProveedoresCSV("../DeustoShopC/Data/proveedores.csv");
    subscripciones  = cargarSubscripcionesCSV("../DeustoShopC/Data/Subscripciones.csv");
    usuarios = cargarUsuariosCSV("../DeustoShopC/Data/usuarios.csv");

    // usuarios y contraseñas
    for (size_t i = 0; i < usuarios.size(); i++)
    {
        if (usuariosContrasenas.find((usuarios[i].getNombre_usuario())) == usuariosContrasenas.end())
        {
            usuariosContrasenas[usuarios[i].getNombre_usuario()] = usuarios[i].getContrasena_usuario();
        }
    }   
}

int MenusCliente::mandarAlServidor(const string &mensaje, string &respuesta) {
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

void MenusCliente::mostrarMenuInicial() {
    int opcion = 0;
    Usuario usuario_actual;

    cargarDatos();

    while (true)
    {
        cout << "MENU\n";
        cout << "1) Registrarse\n";
        cout << "2) Iniciar sesion\n";
        cout << "3) Salir\n\n";
        opcion = pedirEntero("Elija una opcion: ");
        if (opcion == -1) continue;

        if (opcion == 1)
        {
            usuario_actual = mostrarMenuRegistro();
            false;
        } else if (opcion == 2)
        {
            /*cout << "\n\nProbando server...\n\n";
            string respuesta;
            if (mandarAlServidor("INICIAR SESION", respuesta) == 0)
            {
                
            }*/

            usuario_actual = mostrarMenuInicioSesion();
            false;
        } else if (opcion == 3)
        {
            cout << "\nCerrando el programa...\n";
            exit(0);
            break;
        }

        mostrarMenuPrincipal(usuario_actual);
    }
}

Usuario MenusCliente::mostrarMenuRegistro() {

    string nombre_usu, contrasena, direccion, email, tipo_subscripcion;
    int codigo_postal, id_usuario, id_subcripcion;


    Usuario nuevoUsuario;

    cout << "\nMENU DE REGISTRO\n" << "Introduzca los siguientes apartados...\n";

    cout << "Nombre de usuario: ";
    cin >> nombre_usu;
    nuevoUsuario.setNombre_usuario(nombre_usu);

    cout << "Contrasenya: ";
    cin >> contrasena;
    nuevoUsuario.setContrasena_usuario(contrasena);

    cout << "Direccion: ";
    cin.ignore(); 
    getline(cin, direccion);
    nuevoUsuario.setDireccion(direccion);

    cout << "Email: ";
    getline(cin, email);
    nuevoUsuario.setContacto_usuario(email);

    cout << "Tipo de Suscripcion ";
    cout << "[";
    for (Subscripcion s : subscripciones) {
        if (s.getTipo() != "Autonomos")
        {
            cout << " " << s.getTipo() << ", ";
        } else {
            cout << " " << s.getTipo();
        }  
    }
    cout << " ]: ";
    cin >> tipo_subscripcion;

    for (Subscripcion s : subscripciones) {
        if (tipo_subscripcion == s.getTipo())
        {
         id_subcripcion = s.getId_subscripcion();   
        }
    }

    nuevoUsuario.setId_subscripcion(id_subcripcion);

    codigo_postal = pedirEntero("Codigo Postal: ");
    if (codigo_postal == -1) {
        cout << "Volviendo al menú anterior..." << endl;
        return Usuario(); 
    }
    nuevoUsuario.setCodigo_postal(codigo_postal);

    nuevoUsuario.setId_usuario(usuarios.size() + 1);
    return nuevoUsuario;
}

Usuario MenusCliente::mostrarMenuInicioSesion() {

    Usuario usuario_actual;
    string nombre;
    string contra;

    while (true)
    {
        cout << "\n\nNombre de usuario: ";
        cin >> nombre;

        if (usuariosContrasenas.find(nombre) == usuariosContrasenas.end())
        {
            cout << endl << "Ese nombre de usuario no esta registrado." << endl;
        } else {
            cout << endl << "Contrasenya: ";
            cin >> contra;

            if (usuariosContrasenas[nombre] != contra)
            {
                cout << endl << "Contrasenya incorrecta" << endl;
            } else {
                cout << endl << "Inicio de sesion correcto" << endl << endl;
                break;
            }
        }
    }
    
    for (Usuario u : usuarios) {
        if (u.getNombre_usuario() == nombre && u.getContrasena_usuario() == contra)
        {
            usuario_actual = u;
        }
    }
    
    return usuario_actual;
}

void MenusCliente::mostrarMenuPrincipal(Usuario usuario_actual) {
    int opcion = 0;

    cout << "\nMENU PRINCIPAL\n";
    cout << "1) Catalogo de productos\n";
    cout << "2) Historial de compras\n";
    cout << "3) Mostrar almacenes\n";
    cout << "4) Mi perfil\n";
    cout << "5) Salir\n";
    cout << "6) Realizar pedido\n";
    cout << "7) Ver cesta\n";
    opcion = pedirEntero("Elija una opcion: ");
    if (opcion == -1) return;

    if (opcion == 1)
    {
        mostrarMenuProductos(usuario_actual);
    } else if (opcion == 2)
    {
        mostrarHistorialCompras(usuario_actual);
    } else if (opcion == 3)
    {
        mostrarAlmacenes(usuario_actual);
    } else if (opcion == 4)
    {
        mostrarMenuMiPerfil(usuario_actual);
    } else if (opcion == 5)
    {
        cout << "\nCerrando sesion...\n\n";
        mostrarMenuInicial();
    }else if( opcion == 6)
    {
    map<int, int> productosPedido = Pedido::realizarPedidoInteractivo(); 
      } else if (opcion == 7) {
        mostrarMenuCesta(usuario_actual);
    } 
}

void MenusCliente::mostrarMenuProductos(Usuario& usuario_actual) {
    mostrarListaProductos()

    char respuesta;
    cout << "\n¿Quieres añadir algún producto a tu cesta? (s/n): ";
    cin >> respuesta;

    while (respuesta == 's' || respuesta == 'S') {
        int id_producto, cantidad;
        cout << "Introduce el ID del producto: ";
        cin >> id_producto;
        cout << "Introduce la cantidad: ";
        cin >> cantidad;

        cesta[id_producto] += cantidad;

        cout << "¿Quieres añadir otro producto? (s/n): ";
        cin >> respuesta;
    }
}
double MenusCliente::obtenerPrecioProducto(int id_producto) {
    for (const auto& prod : listaProductos) { 
        if (prod.getId() == id_producto) {
            return prod.getPrecio();
        }
    }
    return 0.0;
}

void MenusCliente::mostrarMenuCesta(Usuario& usuario_actual) {
    int opcion;
    do {
        cout << "\n--- CESTA DE LA COMPRA ---\n";
        if (cesta.empty()) {
            cout << "La cesta está vacía.\n";
        } else {
            cout << "ID\tCantidad\tPrecio\n";
            for (const auto& par : cesta) {
                int id = par.first;
                int cantidad = par.second;
                double precio = obtenerPrecioProducto(id); // Implementa esta función según tu lógica
                cout << id << "\t" << cantidad << "\t\t" << precio * cantidad << "€\n";
            }
        }
        cout << "\n1) Borrar un producto\n";
        cout << "2) Vaciar cesta\n";
        cout << "3) Confirmar pedido\n";
        cout << "4) Volver al menú principal\n";
        cout << "Elige una opción: ";
        cin >> opcion;

        if (opcion == 1 && !cesta.empty()) {
            int id_borrar;
            cout << "Introduce el ID del producto a borrar: ";
            cin >> id_borrar;
            cesta.erase(id_borrar);
        } else if (opcion == 2) {
            cesta.clear();
        } else if (opcion == 3 && !cesta.empty()) {
          
            Pedido nuevoPedido(
                0, 
                Fecha(), 
                "Pendiente", 
                usuario_actual.getId_usuario(),
                cesta,
                usuario_actual.getDireccion(),
                usuario_actual.getCodigo_postal()
            );
            usuario_actual.agregarPedido(nuevoPedido);
            cesta.clear();
            cout << "¡Pedido realizado!\n";
        }
    } while (opcion != 4);
}





void MenusCliente::mostrarMenuProductos(Usuario usuario_actual) {
    int opcion;
    cout << endl << "MENU DE PRODUCTOS" << endl;
    cout << "1) Mostrar todos los productos\n";
    cout << "2) Mostrar productos de una categoria especifica\n";
    cout << "3) Volver al menu principal\n\n";
    opcion = pedirEntero("Elija una opcion: ");
    if (opcion == -1) return;



    if (opcion == 1)
    {
        mostrarTodosLosProductos(usuario_actual);
    } else if (opcion == 2)
    {
        mostrarProductosPorCategoria(usuario_actual);
    } else if (opcion == 3)
    {
        mostrarMenuPrincipal(usuario_actual);
    }
}

void MenusCliente::mostrarTodosLosProductos(Usuario usuario_actual) {
    int opcion;
    cout << endl << "TODOS LOS PRODUCTOS" << endl;

    for (size_t i = 0; i < productos.size(); i++) {
        cout << productos[i].getId_producto() << productos[i].getNombre_producto() << productos[i].getPrecio() << endl;
    }

    opcion = pedirEntero("\nPulsa 1 para volver al menu principal: ");
    cout << endl;

    if (opcion == 1)
    {
        mostrarMenuPrincipal(usuario_actual);
    }
}
double MenusCliente::obtenerPrecioProducto(int id_producto) {
    // Implementa aquí la lógica real de búsqueda de precio
    return 0.0;
}

void MenusCliente::mostrarProductosPorCategoria(Usuario usuario_actual) {
    int opcion;
    string pregunta;
    string eleccion;
    set<string> categoriasDisponibles = {"JUGUETERIA", "HOGAR", "ROPA", "ELECTRONICA", "ALIMENTACION", "LIBRERIA", "DEPORTE", "OTROS"};

    while (true)
    {
        cout << endl << endl << "Categorias disponibles: \nJUGUETERIA\nHOGAR\nROPA\nELECTRONICA\nALIMENTACION\nLIBRERIA\nDEPORTE\nOTROS\nSeleccione una opcion (mayusculas necesarias): ";
        cin >> eleccion;
        cout << endl;

        if (categoriasDisponibles.find(eleccion) == categoriasDisponibles.end())
        {
            cout << "La opcion es incorrecta, pruebe de nuevo";
        } else {
            for (Producto p : productos) {
            if (p.getCategoria() == eleccion)
            {
                cout << p.getId_producto() << " " << p.getNombre_producto() << " " << p.getPrecio() << endl;
            }
            }

            cout << "\nDesea ver alguna categoria mas? (s/n): ";
            cin >> pregunta;

            if (pregunta == "n")
            {
                break;
            }
        }
    }

    opcion = pedirEntero("\nPulsa 1 para volver al menu principal: ");
    cout << endl;

    if (opcion == 1)
    {
        mostrarMenuPrincipal(usuario_actual);
    }
}

void MenusCliente::mostrarHistorialCompras(Usuario usuario_actual) {
    int opcion;
    cout << endl << endl << "Pedidos de " << usuario_actual.getNombre_usuario() << endl;

    for (size_t i = 0; i < pedidos.size(); i++)
    {
        if (pedidos[i].getId_usuario() == usuario_actual.getId_usuario()) {

            pedidos[i].imprimirInfPedido();
            
            }
    }

    opcion = pedirEntero("\nPulsa 1 para volver al menu principal: ");
    cout << endl;

    if (opcion == 1)
    {
        mostrarMenuPrincipal(usuario_actual);
    }
}

void MenusCliente::mostrarAlmacenes(Usuario usuario_actual) {
    int opcion;
    cout << endl << "ALMACENES" << endl;
    
    for (size_t i = 0; i < almacenes.size(); i++)
    {
        almacenes[i].imprimirAlmacen();
    }

    opcion = pedirEntero("\nPulsa 1 para volver al menu principal: ");
    cout << endl;

    if (opcion == 1)
    {
        mostrarMenuPrincipal(usuario_actual);
    }
    
}

void MenusCliente::mostrarMenuMiPerfil(Usuario usuario_actual) {
    int opcion;

    cout << "\n\nMI PERFIL\n" << "---------" << endl;

    
    usuario_actual.imprimirUsuario(usuario_actual);


    cout <<  endl << "1) Modificar datos" << endl;
    cout << "2) Eliminar perfil" << endl;
    cout << "3) Volver al menu principal" << endl;
    opcion = pedirEntero("\nElija una opcion: ");
    if (opcion == -1) return;

    if (opcion == 1)
    {
        cout << "Modificar datos" << endl;
        mostrarMenuMiPerfil(usuario_actual);

    usuario_actual.modificarUsuarioPorId(usuario_actual);
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