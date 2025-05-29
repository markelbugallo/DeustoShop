#include <iostream>
#include <stdio.h>
#include <winsock2.h>
#include <sstream>
#include "MenusCliente.h"
#include "../Clases/Usuario.h"
#include "../BD/Bd.h" // Asegúrate de incluir esto para guardarUsuariosCsv
#include "../Clases/Almacen.h"
#include "../Clases/Utils.h"
#include <set>
#include <algorithm>
using namespace std;
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

// Clase auxiliar para gestionar la conexión persistente
class ConexionServidor {
public:
    SOCKET s;
    bool conectada;
    ConexionServidor() : s(INVALID_SOCKET), conectada(false) {}
    bool conectar();
    bool enviar(const string& mensaje, string& respuesta);
    void cerrar();
};

// Definición del miembro estático para la conexión persistente
ConexionServidor MenusCliente::conexionSesion;

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
    if (this->conexionSesion.conectada) {
        return this->conexionSesion.enviar(mensaje, respuesta) ? 0 : 1;
    } else {
        // Fallback: conexión temporal (por si acaso)
        ConexionServidor temp;
        if (!temp.conectar()) return 1;
        bool ok = temp.enviar(mensaje, respuesta);
        temp.cerrar();
        return ok ? 0 : 1;
    }
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
     cargarDatos();
    string nombre_usu, contrasena, direccion, email, tipo_subscripcion;
    int codigo_postal, id_usuario, id_subscripcion = 0;
    Usuario nuevoUsuario;
    cout << "\nMENU DE REGISTRO\n" << "Introduzca los siguientes apartados...\n";
    cout << "Nombre de usuario: ";
    cin >> nombre_usu;
    nuevoUsuario.setNombre_usuario(nombre_usu);
    cout << "Contrasena: ";
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
         id_subscripcion = s.getId_subscripcion();   
        }
    }
    nuevoUsuario.setId_subscripcion(id_subscripcion);
    codigo_postal = pedirEntero("Codigo Postal: ");
    if (codigo_postal == -1) {
        cout << "Volviendo al menu anterior..." << endl;
        return Usuario(); 
    }
    nuevoUsuario.setCodigo_postal(codigo_postal);
    nuevoUsuario.setId_usuario(usuarios.size() + 1);
    ConexionServidor conexion;
    if (!conexion.conectar()) {
        cout << "No se pudo conectar al servidor." << endl;
        return Usuario();
    }
    stringstream mensaje;
    mensaje << "REGISTRAR_USUARIO;"
            << nuevoUsuario.getId_usuario() << ";"
            << nuevoUsuario.getNombre_usuario() << ";"
            << nuevoUsuario.getContrasena_usuario() << ";"
            << nuevoUsuario.getContacto_usuario() << ";"
            << nuevoUsuario.getId_subscripcion() << ";"
            << nuevoUsuario.getDireccion() << ";"
            << nuevoUsuario.getCodigo_postal();
    string respuesta;
    if (conexion.enviar(mensaje.str(), respuesta) && respuesta.rfind("OK;", 0) == 0) {
        vector<string> partes;
        stringstream ss(respuesta);
        string item;
        while (getline(ss, item, ';')) partes.push_back(item);
        if (partes.size() >= 8) {
            nuevoUsuario.setId_usuario(stoi(partes[1]));
            nuevoUsuario.setNombre_usuario(partes[2]);
            nuevoUsuario.setContrasena_usuario(partes[3]);
            nuevoUsuario.setContacto_usuario(partes[4]);
            nuevoUsuario.setId_subscripcion(stoi(partes[5]));
            nuevoUsuario.setDireccion(partes[6]);
            nuevoUsuario.setCodigo_postal(stoi(partes[7]));
        }
        usuarios.push_back(nuevoUsuario);
        guardarUsuariosCsv(usuarios);
        cout << "Usuario registrado correctamente.\n";
        conexion.cerrar();
        return nuevoUsuario;
    } else if (respuesta.rfind("ERROR;", 0) == 0) {
        cout << "Error del servidor: " << respuesta.substr(6) << endl;
        conexion.cerrar();
        return Usuario();
    } else {
        cout << "Error al registrar usuario en el servidor.\n";
        conexion.cerrar();
        return Usuario();
    }
}

Usuario MenusCliente::mostrarMenuInicioSesion() {
    Usuario usuario_actual;
    string nombre, contra;
    ConexionServidor conexion;
    if (!conexion.conectar()) {
        cout << "No se pudo conectar al servidor." << endl;
        return usuario_actual;
    }
    while (true) {
        cout << "\n\nNombre de usuario: ";
        cin >> nombre;
        cout << endl << "Contrasena: ";
        cin >> contra;
        stringstream mensaje;
        mensaje << "LOGIN;" << nombre << ";" << contra;
        string respuesta;
        if (conexion.enviar(mensaje.str(), respuesta)) {
            if (respuesta.rfind("OK;", 0) == 0) {
                vector<string> partes;
                stringstream ss(respuesta);
                string item;
                while (getline(ss, item, ';')) partes.push_back(item);
                if (partes.size() >= 8) {
                    usuario_actual.setId_usuario(stoi(partes[1]));
                    usuario_actual.setNombre_usuario(partes[2]);
                    usuario_actual.setContrasena_usuario(partes[3]);
                    usuario_actual.setContacto_usuario(partes[4]);
                    usuario_actual.setId_subscripcion(stoi(partes[5]));
                    usuario_actual.setDireccion(partes[6]);
                    usuario_actual.setCodigo_postal(stoi(partes[7]));
                    bool found = false;
                    for (auto& u : usuarios) {
                        if (u.getId_usuario() == usuario_actual.getId_usuario()) {
                            u = usuario_actual;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        usuarios.push_back(usuario_actual);
                    }
                    guardarUsuariosCsv(usuarios);
                }
                cout << endl << "Inicio de sesion correcto (servidor)" << endl << endl;
                break;
            } else if (respuesta == "EXITO") {
                cout << endl << "Inicio de sesion correcto (servidor)" << endl << endl;
                break;
            } else if (respuesta.rfind("ERROR;", 0) == 0) {
                cout << "Error del servidor: " << respuesta.substr(6) << endl;
            } else {
                cout << "Respuesta inesperada del servidor: " << respuesta << endl;
            }
        } else {
            cout << "Error de conexion con el servidor." << endl;
        }
    }
    // Guardar la conexión persistente para la sesión
    this->conexionSesion = conexion;
    return usuario_actual;
}

void MenusCliente::mostrarMenuPrincipal(Usuario usuario_actual) {
    cargarDatos(); // Recarga usuarios y contraseñas desde el CSV
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
        this->conexionSesion.cerrar();
        mostrarMenuInicial();
    }else if( opcion == 6)
    {
    map<int, int> productosPedido = Pedido::realizarPedidoInteractivo(); 
      } else if (opcion == 7) {
        mostrarMenuCesta(usuario_actual);
    } 
}

void MenusCliente::mostrarMenuC(Usuario usuario_actual) {
    //mostrarListaProductos();

    char respuesta;
    cout << "\n¿Quieres anyadir algun producto a tu cesta? (s/n): ";
    cin >> respuesta;

    while (respuesta == 's' || respuesta == 'S') {
        int id_producto, cantidad;
        cout << "Introduce el ID del producto: ";
        cin >> id_producto;
        cout << "Introduce la cantidad: ";
        cin >> cantidad;

        cesta[id_producto] += cantidad;

        cout << "¿Quieres anyadir otro producto? (s/n): ";
        cin >> respuesta;
    }
}
double MenusCliente::obtenerPrecioProducto(int id_producto) {
    for (Producto prod : productos) { 
        if (prod.getId_producto() == id_producto) {
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
            cout << "La cesta esta vacia.\n";
        } else {
            cout << "ID\tCantidad\tPrecio\n";
            for (const auto& par : cesta) {
                int id = par.first;
                int cantidad = par.second;
                double precio = obtenerPrecioProducto(id);
                cout << id << "\t" << cantidad << "\t\t" << precio * cantidad << "€\n";
            }
        }
        cout << "\n1) Borrar un producto\n";
        cout << "2) Vaciar cesta\n";
        cout << "3) Confirmar pedido\n";
        cout << "4) Volver al menu principal\n";
        cout << "Elige una opcion: ";
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
        cout << productos[i].getId_producto() << "  |  " << productos[i].getNombre_producto() << "  |  " << productos[i].getPrecio() << " Euros" <<endl;
    }

    opcion = pedirEntero("\nPulsa 1 para volver al menu principal: ");
    cout << endl;

    if (opcion == 1)
    {
        mostrarMenuPrincipal(usuario_actual);
    }
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
                cout << p.getId_producto() << "  |  " << p.getNombre_producto() << "  |  " << p.getPrecio() << " Euros" <<endl;
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

void MenusCliente::editarPerfil(Usuario& usuario_actual) {
    int opcion;
    string nuevoValor;
    string nombre_anterior = usuario_actual.getNombre_usuario(); // Guarda el nombre antes de editar
    cout << "\nQUE DESEA EDITAR\n";
    cout << "1) Nombre\n";
    cout << "2) Contrasena\n";
    //cout << "3) Contacto\n";
    //cout << "4) Direccion\n";
    cout << "5) Codigo Postal\n";
    cout << "6) Volver\n";
    opcion = pedirEntero("Seleccione una opcion: ");
    if (opcion == -1 || opcion == 6) return;

    switch (opcion) {
        case 1:
            cout << "Nuevo nombre: ";
            getline(cin >> ws, nuevoValor);
            usuario_actual.setNombre_usuario(nuevoValor);
            break;
        case 2:
            cout << "Nueva contrasena: ";
            getline(cin >> ws, nuevoValor);
            usuario_actual.setContrasena_usuario(nuevoValor);
            break;
        case 3:
            cout << "Nuevo contacto: ";
            getline(cin >> ws, nuevoValor);
            usuario_actual.setContacto_usuario(nuevoValor);
            break;
        case 4:
            cout << "Nueva direccion: ";
            getline(cin >> ws, nuevoValor);
            usuario_actual.setDireccion(nuevoValor);
            break;
        case 5:
            int nuevoCP;
            cout << "Nuevo codigo postal: ";
            cin >> nuevoCP;
            usuario_actual.setCodigo_postal(nuevoCP);
            cin.ignore();
            break;
        default:
            cout << "Opcion no valida.\n";
            return;
    }

    // Enviar actualización al servidor
    stringstream mensaje;
    mensaje << "EDITAR_USUARIO;"
            << usuario_actual.getId_usuario() << ";"
            << usuario_actual.getNombre_usuario() << ";"
            << usuario_actual.getContrasena_usuario() << ";"
            << usuario_actual.getContacto_usuario() << ";"
            << usuario_actual.getId_subscripcion() << ";"
            << usuario_actual.getDireccion() << ";"
            << usuario_actual.getCodigo_postal();

    string respuesta;
    if (mandarAlServidor(mensaje.str(), respuesta) == 0 && respuesta.rfind("OK;", 0) == 0) {
        vector<string> partes;
        stringstream ss(respuesta);
        string item;
        while (getline(ss, item, ';')) partes.push_back(item);
        if (partes.size() >= 8) {
            usuario_actual.setId_usuario(stoi(partes[1]));
            usuario_actual.setNombre_usuario(partes[2]);
            usuario_actual.setContrasena_usuario(partes[3]);
            usuario_actual.setContacto_usuario(partes[4]);
            usuario_actual.setId_subscripcion(stoi(partes[5]));
            usuario_actual.setDireccion(partes[6]);
            usuario_actual.setCodigo_postal(stoi(partes[7]));
        }
        for (auto& u : usuarios) {
            if (u.getId_usuario() == usuario_actual.getId_usuario()) {
                u = usuario_actual;
                break;
            }
        }
        guardarUsuariosCsv(usuarios);
        // ACTUALIZA el mapa de usuariosContrasenas en memoria
        if (nombre_anterior != usuario_actual.getNombre_usuario()) {
            usuariosContrasenas.erase(nombre_anterior);
        }
        usuariosContrasenas[usuario_actual.getNombre_usuario()] = usuario_actual.getContrasena_usuario();
        cout << "Perfil actualizado correctamente (servidor y local).\n";
    } else if (respuesta.rfind("ERROR;", 0) == 0) {
        cout << "Error del servidor: " << respuesta.substr(6) << endl;
    } else {
        cout << "Error al actualizar el perfil en el servidor. Cambios NO guardados localmente.\n";
    }
}

void MenusCliente::eliminarPerfil(Usuario& usuario_actual) {

    stringstream mensaje;
    mensaje << "ELIMINAR_USUARIO;" << usuario_actual.getId_usuario();
    string respuesta;
    if (mandarAlServidor(mensaje.str(), respuesta) == 0) {
        respuesta.erase(remove(respuesta.begin(), respuesta.end(), '\n'), respuesta.end());
        respuesta.erase(remove(respuesta.begin(), respuesta.end(), '\r'), respuesta.end());
        if (respuesta == "OK" || respuesta == "OK;") {
            // Elimina localmente del vector usuarios
            usuarios.erase(
                remove_if(usuarios.begin(), usuarios.end(),
                    [&](const Usuario& u) { return u.getId_usuario() == usuario_actual.getId_usuario(); }),
                usuarios.end()
            );
            // Elimina del mapa usuariosContrasenas
            usuariosContrasenas.erase(usuario_actual.getNombre_usuario());
            // Guarda el CSV actualizado
            guardarUsuariosCsv(usuarios);
            cout << "Perfil eliminado correctamente.\n";
            // Volver al menú principal tras eliminar
            mostrarMenuPrincipal(Usuario());
            return;
        } else if (respuesta.rfind("ERROR;", 0) == 0) {
            cout << "Error del servidor: " << respuesta.substr(6) << endl;
        } else {
            cout << "Error al eliminar el perfil en el servidor. Respuesta: " << respuesta << endl;
        }
    } else {
        cout << "Error de conexion con el servidor.\n";
    }
    // Si llega aquí, no se eliminó correctamente, vuelve al menú de perfil
    mostrarMenuMiPerfil(usuario_actual);
}

// Modifica mostrarMenuMiPerfil para llamar a editarPerfil
void MenusCliente::mostrarMenuMiPerfil(Usuario& usuario_actual) {
    int opcion;
    while (true) {
        cout << "\n\nMI PERFIL\n" << "---------" << endl;
        usuario_actual.imprimirUsuario(usuario_actual);

        cout << endl << "1) Modificar datos" << endl;
        cout << "2) Eliminar perfil" << endl;
        cout << "3) Volver al menu principal" << endl;
        opcion = pedirEntero("\nElija una opcion: ");
        if (opcion == -1) return;

        if (opcion == 1) {
            editarPerfil(usuario_actual);
            // Ya se actualiza usuario_actual, solo refrescamos la vista
            // No reiniciar sesion ni pedir datos antiguos
        } else if (opcion == 2) {
            eliminarPerfil(usuario_actual);
            return;
        } else if (opcion == 3) {
            cout << "Volviendo al menu principal..." << endl;
            mostrarMenuPrincipal(usuario_actual);
            return;
        }
    }
}

// Declaración de la conexión persistente como miembro estático de la clase
ConexionServidor MenusCliente::conexionSesion;

bool ConexionServidor::conectar() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Failed. Error Code : " << to_string(WSAGetLastError()) << endl;
        return false;
    }
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "Could not create socket : " << to_string(WSAGetLastError()) << endl;
        WSACleanup();
        return false;
    }
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    if (connect(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        cout << "Connection error: " << to_string(WSAGetLastError()) << endl;
        closesocket(s);
        WSACleanup();
        return false;
    }
    conectada = true;
    return true;
}
bool ConexionServidor::enviar(const string& mensaje, string& respuesta) {
    if (!conectada) return false;
    char recvBuff[512];
    send(s, mensaje.c_str(), mensaje.length(), 0);
    int bytesReceived = recv(s, recvBuff, sizeof(recvBuff), 0);
    if (bytesReceived <= 0) return false;
    recvBuff[bytesReceived] = '\0';
    respuesta = recvBuff;
    return true;
}
void ConexionServidor::cerrar() {
    if (conectada) {
        closesocket(s);
        WSACleanup();
        conectada = false;
    }
}
