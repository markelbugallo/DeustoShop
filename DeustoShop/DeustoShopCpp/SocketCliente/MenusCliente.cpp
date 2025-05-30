#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <conio.h>
#include <stdio.h>
#include <winsock2.h>
#include "MenusCliente.h"
#include "../Clases/Usuario.h"
#include "../BD/Bd.h"
#include "../Clases/Almacen.h"
#include "../Clases/Utils.h"
#include <set>
#include <algorithm>
#include <iomanip> // Asegúrate de tener este include arriba
#include <fstream> // Añadido para el logging
#include <ctime> // Añadido para la función log
using namespace std;
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

// Definición del miembro estático para la conexión persistente
ConexionServidor MenusCliente::conexionSesion;

// Declaración y definición de la función log al principio del archivo
void log(const std::string& mensaje) {
    std::ofstream logFile("../Log/log.txt", std::ios::app);
    if (!logFile) {
        // Si falla, prueba con la ruta desde DeustoShopCpp
        logFile.open("Log/log.txt", std::ios::app);
    }
    if (!logFile) {
        std::cerr << "No se pudo abrir el archivo de log en ninguna ruta." << std::endl;
        return;
    }
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char fechaHora[32];
    strftime(fechaHora, sizeof(fechaHora), "%Y-%m-%d %H:%M:%S", ltm);
    logFile << "[CLIENTE] [" << fechaHora << "] " << mensaje << std::endl;
}



int MenusCliente::mandarAlServidor(const string &mensaje, string &respuesta) {
    if (this->conexionSesion.conectada) {
        return this->conexionSesion.enviar(mensaje, respuesta) ? 0 : 1;
    } else {
        // Fallback: conexión temporal (por si acaso)
        ConexionServidor temp;
        if (!temp.conectar()) {
            log("Error: No se pudo conectar al servidor (mandarAlServidor)");
            return 1;
        }
        bool ok = temp.enviar(mensaje, respuesta);
        temp.cerrar();
        if (!ok) log("Error: Fallo al enviar mensaje al servidor (mandarAlServidor)");
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
            usuario_actual = mostrarMenuInicioSesion();
            false;
        } else if (opcion == 3)
        {
            log("Cerrando el programa...");
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
    getline(cin >> ws, nombre_usu); // Usar getline para evitar problemas de salto de línea
    nuevoUsuario.setNombre_usuario(nombre_usu);
    cout << "Contrasena: ";
    getline(cin >> ws, contrasena); // Usar getline para evitar problemas de salto de línea
    nuevoUsuario.setContrasena_usuario(contrasena);
    cout << "Direccion: ";
    getline(cin >> ws, direccion);
    nuevoUsuario.setDireccion(direccion);
    cout << "Email: ";
    getline(cin >> ws, email);
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
    getline(cin >> ws, tipo_subscripcion);
    for (Subscripcion s : subscripciones) {
        if (tipo_subscripcion == s.getTipo())
        {
         id_subscripcion = s.getId_subscripcion();   
        }
    }
    nuevoUsuario.setId_subscripcion(id_subscripcion);
    codigo_postal = pedirEntero("Codigo Postal: ");
    if (codigo_postal == -1) {
        log("Registro cancelado: Codigo postal no valido");
        cout << "Volviendo al menu anterior..." << endl;
        return Usuario(); 
    }
    nuevoUsuario.setCodigo_postal(codigo_postal);
    nuevoUsuario.setId_usuario(usuarios.size() + 1);
    ConexionServidor conexion;
    if (!conexion.conectar()) {
        log("Error: No se pudo conectar al servidor (registro)");
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
        log("Usuario registrado correctamente: " + nuevoUsuario.getNombre_usuario());
        cout << "Usuario registrado correctamente.\n";
        conexion.cerrar();
        return nuevoUsuario;
    } else if (respuesta.rfind("ERROR;", 0) == 0) {
        log("Error del servidor al registrar usuario: " + respuesta.substr(6));
        cout << "Error del servidor: " << respuesta.substr(6) << endl;
        mostrarMenuInicial();
        return Usuario();
    } else {
        log("Error al registrar usuario en el servidor");
        cout << "Error al registrar usuario en el servidor.\n";
        conexion.cerrar();
        return Usuario();
    }
}

Usuario MenusCliente::mostrarMenuInicioSesion() {
    Usuario usuario_actual;
    string nombre, contra;
    while (true) {
        cout << "\n\nNombre de usuario: ";
        cin >> nombre;
        cout << endl << "Contrasena: ";
        cin >> contra;

        ConexionServidor conexion;
        if (!conexion.conectar()) {
            log("Error: No se pudo conectar al servidor (inicio de sesion)");
            cout << "No se pudo conectar al servidor." << endl;
            return usuario_actual;
        }

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
                log("Inicio de sesion correcto (servidor) para usuario: " + nombre);
                cout << endl << "Inicio de sesion correcto (servidor)" << endl << endl;
                conexion.cerrar(); // CIERRA LA CONEXIÓN AQUÍ
                break;
            } else if (respuesta == "EXITO") {
                log("Inicio de sesion correcto (servidor) para usuario: " + nombre);
                cout << endl << "Inicio de sesion correcto (servidor)" << endl << endl;
                conexion.cerrar();
                break;
            } else if (respuesta.rfind("ERROR;", 0) == 0) {
                log("Error del servidor al iniciar sesion: " + respuesta.substr(6));
                cout << "Error del servidor: " << respuesta.substr(6) << endl;
                conexion.cerrar();
            } else {
                log("Respuesta inesperada del servidor al iniciar sesion: " + respuesta);
                cout << "Respuesta inesperada del servidor: " << respuesta << endl;
                conexion.cerrar();
            }
        } else {
            log("Error de conexion con el servidor al iniciar sesion");
            cout << "Error de conexion con el servidor." << endl;
            conexion.cerrar();
            mostrarMenuInicial();
            break;
        }
    }
    // NO guardes la conexión como miembro de la clase
    return usuario_actual;
}

void MenusCliente::mostrarMenuPrincipal(Usuario usuario_actual) {
    cargarDatos(); // Recarga usuarios y contraseñas desde el CSV
    int opcion = 0;

    cout << "\nMENU PRINCIPAL\n";
    cout << "1) Realizar pedido\n";
    cout << "2) Historial de compras\n";
    cout << "3) Mostrar almacenes\n";
    cout << "4) Mi perfil\n";
    cout << "5) Salir\n";
    opcion = pedirEntero("Elija una opcion: ");
    if (opcion == -1) return;

    if (opcion == 1) {
        mostrarMenuProductos(usuario_actual);
    } else if (opcion == 2) {
        mostrarHistorialCompras(usuario_actual);
    } else if (opcion == 3) {
        mostrarAlmacenes(usuario_actual);
    } else if (opcion == 4) {
        mostrarMenuMiPerfil(usuario_actual);
    } else if (opcion == 5) {
        log("Cerrando sesion de usuario: " + usuario_actual.getNombre_usuario());
        cout << "\nCerrando sesion...\n\n";
        this->conexionSesion.cerrar();
        mostrarMenuInicial();
    }
}

/*void MenusCliente::mostrarMenuC(Usuario usuario_actual) {
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
}*/
double MenusCliente::obtenerPrecioProducto(int id_producto) {
    double precio = 0.0;
    for (const Producto& prod : productos) { // Usa el vector correcto
        if (prod.getId_producto() == id_producto) {
            precio = prod.getPrecio();
            break; // Salimos del bucle una vez encontrado el producto
        }
    }
  return precio; ; 
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
                cout << id << "\t" << cantidad << "\t\t"
                     << fixed << setprecision(2) << precio * cantidad << " Euros" << endl;
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
            // Obtener fecha actual en formato YYYY-MM-DD
            time_t t = time(nullptr);
            tm* now = localtime(&t);
            char fechaStr[11];
            strftime(fechaStr, sizeof(fechaStr), "%Y-%m-%d", now); // <-- FORMATO CORRECTO

            // Calcular el nuevo id de pedido
            int nuevo_id_pedido = pedidos.size() + 1;

            // Construir mensaje en formato CSV
            stringstream mensaje;
            mensaje << "REALIZAR_PEDIDO;"
                    << nuevo_id_pedido << ";"
                    << fechaStr << ";" // <-- FECHA EN FORMATO CORRECTO
                    << "procesando solicitud" << ";"
                    << usuario_actual.getId_usuario() << ";";

            // Productos
            bool primero = true;
            for (const auto& par : cesta) {
                if (!primero) mensaje << ",";
                mensaje << par.first << ":" << par.second;
                primero = false;
            }
            mensaje << ";" << usuario_actual.getDireccion() << ";"
                    << usuario_actual.getCodigo_postal();

            string respuesta;
            if (mandarAlServidor(mensaje.str(), respuesta) == 0) {
                // Limpiar caracteres de control de la respuesta
                respuesta.erase(remove(respuesta.begin(), respuesta.end(), '\n'), respuesta.end());
                respuesta.erase(remove(respuesta.begin(), respuesta.end(), '\r'), respuesta.end());

                log("DEBUG: Respuesta limpia: '" + respuesta + "'");
                cout << "DEBUG: Respuesta limpia: '" << respuesta << "'" << endl;

                if (respuesta.rfind("OK;", 0) == 0 || respuesta == "OK") {
                    log("Pedido realizado correctamente para usuario: " + usuario_actual.getNombre_usuario());
                    cout << "¡Pedido realizado correctamente!\n";
                    Pedido nuevoPedido(
                        nuevo_id_pedido,
                        Fecha{now->tm_mday, now->tm_mon + 1, now->tm_year + 1900},
                        "procesando solicitud",
                        usuario_actual.getId_usuario(),
                        cesta,
                        usuario_actual.getDireccion(),
                        usuario_actual.getCodigo_postal()
                    );
                    usuario_actual.agregarPedido(nuevoPedido);
                    pedidos.push_back(nuevoPedido);
                    cesta.clear();
                    mostrarMenuPrincipal(usuario_actual);
                    break;
                } else {
                    log("Error al realizar el pedido: " + respuesta);
                    cout << "Error al realizar el pedido: " << respuesta << endl;
                    mostrarMenuPrincipal(usuario_actual);
                    break;
                }
            } else {
                log("Error de conexion con el servidor al realizar el pedido");
                cout << "Error de conexion con el servidor al realizar el pedido." << endl;
                mostrarMenuPrincipal(usuario_actual);
                break;
            }
        } else if (opcion == 4) {
            mostrarMenuPrincipal(usuario_actual);
            break;
        }
    } while (true);
}

//a
void MenusCliente::mostrarMenuProductos(Usuario usuario_actual) {
    int opcion;
    do {
        cout << endl << "MENU DE REALIZAR PEDIDO" << endl;
        cout << "1) Mostrar todos los productos\n";
        cout << "2) Mostrar productos de una categoria especifica\n";
        cout << "3) Anadir producto a la cesta\n";
        cout << "4) Ver cesta\n";
        cout << "5) Volver al menu principal\n\n";
        opcion = pedirEntero("Elija una opcion: ");
        if (opcion == -1) return;

        if (opcion == 1) {
            mostrarTodosLosProductos(usuario_actual);

        } else if (opcion == 2) {
            mostrarProductosPorCategoria(usuario_actual);
        } else if (opcion == 3) {
            // Añadir producto a la cesta
            int id_producto, cantidad;
            cout << "Introduce el ID del producto: ";
            cin >> id_producto;
            cout << "Introduce la cantidad: ";
            cin >> cantidad;
            cesta[id_producto] += cantidad;
            cout << "Producto anadido a la cesta.\n";
        } else if (opcion == 4) {
            mostrarMenuCesta(usuario_actual);
        }
    } while (opcion != 5);

    mostrarMenuPrincipal(usuario_actual);
}

void MenusCliente::mostrarTodosLosProductos(Usuario& usuario_actual) {
    int opcion;
    string pregunta;
    cout << endl << "TODOS LOS PRODUCTOS" << endl;

    for (size_t i = 0; i < productos.size(); i++) {
        cout << productos[i].getId_producto() << "  |  " << productos[i].getNombre_producto() << "  |  " << productos[i].getPrecio() << " Euros" <<endl;
    }
    cout <<("\nDesea realizar un pedido? (s/n): ")<< endl;
    cin >> pregunta;
    cout << endl;
    if (pregunta == "s")
    {
        realizarPedido(usuario_actual);
    } else if (pregunta == "n")
    {
        cout << "Volviendo al menu principal..." << endl;
        mostrarMenuPrincipal(usuario_actual);

    } else {
        cout << "Opcion no valida, volviendo al menu principal..." << endl;
        mostrarMenuPrincipal(usuario_actual);
    }
/*
    opcion = pedirEntero("\nPulsa 1 para volver al menu principal: ");

    cout << endl;

    if (opcion == 1)
    {
        mostrarMenuPrincipal(usuario_actual);
    }
        */


}

void MenusCliente::mostrarProductosPorCategoria(Usuario& usuario_actual) {
    int opcion;
    string respuesta;
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
     
    cout <<("\nDesea realizar un pedido? (s/n): ")<< endl;
    cin >> respuesta;
    cout << endl;
    if (respuesta == "s")
    {
        realizarPedido(usuario_actual);
    } else if (respuesta == "n")
    {
        cout << "Volviendo al menu principal..." << endl;
        mostrarMenuPrincipal(usuario_actual);
    } else {
        cout << "Opcion no valida, volviendo al menu principal..." << endl;
        mostrarMenuPrincipal(usuario_actual);
    }

   // opcion = pedirEntero("\nPulsa 1 para volver al menu principal: ");
 // cout << endl;

    // if (opcion == 1)
    // {
    //     mostrarMenuPrincipal(usuario_actual);
    // }
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
    cout << "3) Contacto\n";
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
    if (mandarAlServidor(mensaje.str(), respuesta) == 0) {
        // Limpia saltos de línea
        respuesta.erase(remove(respuesta.begin(), respuesta.end(), '\n'), respuesta.end());
        respuesta.erase(remove(respuesta.begin(), respuesta.end(), '\r'), respuesta.end());

        log("DEBUG: Respuesta del servidor: " + respuesta);
        cout << "DEBUG: Respuesta del servidor: " << respuesta << endl;

        if (respuesta.rfind("OK;", 0) == 0) {
            // Procesa la respuesta y actualiza usuario_actual
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
            // Actualiza localmente
            for (auto& u : usuarios) {
                if (u.getId_usuario() == usuario_actual.getId_usuario()) {
                    u = usuario_actual;
                    break;
                }
            }
            guardarUsuariosCsv(usuarios);
            log("Perfil actualizado correctamente para usuario: " + usuario_actual.getNombre_usuario());
            cout << "Perfil actualizado correctamente (servidor y local).\n";
        } else if (respuesta.rfind("ERROR;", 0) == 0) {
            log("Error del servidor al actualizar perfil: " + respuesta.substr(6));
            cout << "Error del servidor: " << respuesta.substr(6) << endl;
        } else {
            log("Error al actualizar el perfil en el servidor. Cambios NO guardados localmente.");
            cout << "Error al actualizar el perfil en el servidor. Cambios NO guardados localmente.\n";
        }
    } else {
        log("Error de conexion con el servidor al editar perfil");
        cout << "Error de conexion con el servidor.\n";
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
            log("Perfil eliminado correctamente para usuario: " + usuario_actual.getNombre_usuario());
            // Elimina localmente del vector usuarios
        /*    usuarios.erase(
                remove_if(usuarios.begin(), usuarios.end(),
                    [&](const Usuario& u) { return u.getId_usuario() == usuario_actual.getId_usuario(); }),
                usuarios.end()
            );
            // Elimina del mapa usuariosContrasenas
            usuariosContrasenas.erase(usuario_actual.getNombre_usuario());
            // Guarda el CSV actualizado
            guardarUsuariosCsv(usuarios);
            cout << "Perfil eliminado correctamente.\n";
            // Cierra la conexión de la sesión
            this->conexionSesion.cerrar();
            // Volver al menú inicial tras eliminar*/
            mostrarMenuInicial();
            return;
        } else if (respuesta.rfind("ERROR;", 0) == 0) {
            log("Error del servidor al eliminar perfil: " + respuesta.substr(6));
            cout << "Error del servidor: " << respuesta.substr(6) << endl;
        } else {
            log("Error al eliminar el perfil en el servidor. Respuesta: " + respuesta);
            cout << "Error al eliminar el perfil en el servidor. Respuesta: " << respuesta << endl;
        }
    } else {
        log("Error de conexion con el servidor al eliminar perfil");
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

void MenusCliente::realizarPedido(Usuario& usuario_actual) {
    int id_producto, cantidad;
    char respuesta;
    do {
        cout << "Introduce el ID del producto: ";
        cin >> id_producto;
        cout << "Introduce la cantidad: ";
        cin >> cantidad;
        cesta[id_producto] += cantidad;
        cout << "Quieres anadir otro producto? (s/n): ";
        cin >> respuesta;
    } while (respuesta == 's' || respuesta == 'S');
    mostrarMenuCesta(usuario_actual);
}

string pedirCompra(const string& mensaje) {
    string respuesta;
    cout << mensaje;
    cin >> respuesta;
    if (respuesta == "S") respuesta = "s";
    if (respuesta == "N") respuesta = "n";
    return respuesta;
}

void procesarComando(const string& comando, const string& datos, string& respuestaServidor) {
    stringstream ss(datos);
    if (comando == "REGISTRAR_USUARIO") {
        // ... (código existente para REGISTRAR_USUARIO)
    } else if (comando == "LOGIN") {
        // ... (código existente para LOGIN)
    } else if (comando == "REALIZAR_PEDIDO") {
        string id_usuario_str, direccion, codigo_postal_str, productos_str;
        getline(ss, id_usuario_str, ';');
        getline(ss, direccion, ';');
        getline(ss, codigo_postal_str, ';');
        getline(ss, productos_str, ';');
        int id_usuario = stoi(id_usuario_str);
        int codigo_postal = stoi(codigo_postal_str);
        map<int, int> productos;
        stringstream ssProductos(productos_str);
        string item;
        while (getline(ssProductos, item, ',')) {
            size_t pos = item.find(':');
            if (pos != string::npos) {
                int id_prod = stoi(item.substr(0, pos));
                int cantidad = stoi(item.substr(pos + 1));
                productos[id_prod] = cantidad;
            }
        }
        int id_pedido = pedidos.size() + 1;
        Fecha fecha_actual; // Asegúrate de inicializar la fecha correctamente
        Pedido nuevoPedido(id_pedido, fecha_actual, "Pendiente", id_usuario, productos, direccion, codigo_postal);
        pedidos.push_back(nuevoPedido);
        // guardarPedidosCsv(pedidos); // Si tienes persistencia
        respuestaServidor = "OK;Pedido registrado";
    }
    // ... (otros comandos)
}