#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <winsock2.h>
#include <fstream>
#include <algorithm>
#include "../../DeustoShopCpp/Clases/Usuario.h"
#include "../../DeustoShopCpp/Clases/Pedido.h"
#include "../../DeustoShopCpp/BD/Bd.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

using namespace std;

ofstream logFile("log.txt");

void log(const std::string &message) {
    cout << message << endl;
    logFile << message << endl;
}

/*void guardarPedidosCsv(const vector<Pedido>& pedidos) {
    ofstream file("pedidos.csv");
    for (const Pedido& p : pedidos) {
        file << p.getId_pedido() << ";"
             << p.getFecha_pedido().toString() << ";"
             << p.getEstado_pedido() << ";"
             << p.getId_usuario() << ";";
        bool primero = true;
        for (const auto& par : p.getProductosCantidades()) {
            if (!primero) file << ",";
            file << par.first << ":" << par.second;
            primero = false;
        }
        file << ";" << p.getDireccion() << ";" << p.getCodigo_Postal() << "\n";
    }
    file.close();
}*/

int main() {
    WSADATA wsaData;
    SOCKET conn_socket, comm_socket;
    struct sockaddr_in server, client;
    char sendBuff[512], recvBuff[512];

    cout << "Inicializando Winsock..." << endl;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        log("Fallo en el WSAStartup: " + to_string(WSAGetLastError()));
        return 1;
    }
    cout << "Inicializado." << endl;

    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cout << "No se pudo crear el socket: " << to_string(WSAGetLastError()) << endl;
        WSACleanup();
        return 1;
    }
    cout << "Socket creado." << endl;

    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
        cout << "Bind ha fallado con el codigo de error: " <<  to_string(WSAGetLastError()) << endl;
        closesocket(conn_socket);
        WSACleanup();
        return 1;
    }
    cout << "Bind hecho." << endl;

    if (listen(conn_socket, 5) == SOCKET_ERROR) {
        cout << "Listen fallo: " << to_string(WSAGetLastError()) << endl;
        closesocket(conn_socket);
        WSACleanup();
        return 1;
    }

    cout << "Esperando conexiones..." << endl;
    int stsize = sizeof(struct sockaddr);
    while ((comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize)) != INVALID_SOCKET) {
        cargarDatos(); // Cargar datos antes de aceptar la conexión

        cout << "Conexion aceptada." << endl;
        cout << "Conexion desde: " << inet_ntoa(client.sin_addr) << " (" << ntohs(client.sin_port) << ")" << endl;
        cout << "Esperando mensajes..." << endl;

        int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        if (bytes == SOCKET_ERROR) {
            cout << "Error al recibir: " << to_string(WSAGetLastError()) << endl;
        } else {
            recvBuff[bytes] = '\0';
            string respuestaServidor = "";
            stringstream ss(recvBuff);
            string comando;
            getline(ss, comando, ';');

            if (comando == "LOGIN") {
                cargarDatos();
                string id, nombre, contrasena, contacto, id_sub, direccion, cp;
                getline(ss, nombre, ';');
                getline(ss, contrasena, ';');

                auto usuarioEncontrado = usuariosContrasenas.find(nombre);
                if (usuarioEncontrado != usuariosContrasenas.end() && usuarioEncontrado->second == contrasena) {
                    for (Usuario u : usuarios) {
                        if (u.getNombre_usuario() == nombre) {
                            id = to_string(u.getId_usuario());
                            contacto = u.getContacto_usuario();
                            id_sub = to_string(u.getId_subscripcion());
                            direccion = u.getDireccion();
                            cp = to_string(u.getCodigo_postal());
                            break;
                        }
                    }
                    respuestaServidor = "OK;" + id + ";" + nombre + ";" + contrasena + ";" + contacto + ";" + id_sub + ";" + direccion + ";" + cp;
                } else {
                    respuestaServidor = "ERROR;Usuario no encontrado o contrasena incorrecta";
                }
            } else if (comando == "REGISTRAR_USUARIO") {
                cargarDatos();
                string id, nombre, contrasena, contacto, id_subscripcion, direccion, codigo_postal;
                getline(ss, id, ';'); getline(ss, nombre, ';'); getline(ss, contrasena, ';');
                getline(ss, contacto, ';'); getline(ss, id_subscripcion, ';');
                getline(ss, direccion, ';'); getline(ss, codigo_postal, ';');

                bool existe = false;
                for (const auto& u : usuarios) {
                    if (u.getNombre_usuario() == nombre) {
                        existe = true;
                        break;
                    }
                }
                if (existe) {
                    respuestaServidor = "ERROR;El usuario ya existe";
                } else {
                    Usuario nuevoUsuario(stoi(id), nombre, contrasena, contacto, stoi(id_subscripcion), direccion, stoi(codigo_postal));
                    usuarios.push_back(nuevoUsuario);
                    guardarUsuariosCsv(usuarios);
                    respuestaServidor = "OK;" + id + ";" + nombre + ";" + contrasena + ";" + contacto + ";" + id_subscripcion + ";" + direccion + ";" + codigo_postal;
                }
            } else if (comando == "EDITAR_USUARIO") {
                cargarDatos();
                string id_usuario_str, nombre, contrasena, contacto, id_subscripcion_str, direccion, codigo_postal_str;
                getline(ss, id_usuario_str, ';');
                getline(ss, nombre, ';');
                getline(ss, contrasena, ';');
                getline(ss, contacto, ';');
                getline(ss, id_subscripcion_str, ';');
                getline(ss, direccion, ';');
                getline(ss, codigo_postal_str, ';');

                bool campos_ok = !id_usuario_str.empty() && !nombre.empty() && !contrasena.empty() && !contacto.empty() &&
                                 !id_subscripcion_str.empty() && !direccion.empty() && !codigo_postal_str.empty();

                if (!campos_ok) {
                    respuestaServidor = "ERROR;Campos incompletos";
                } else {
                    try {
                        int id_usuario = stoi(id_usuario_str);
                        int id_subscripcion = stoi(id_subscripcion_str);
                        int codigo_postal = stoi(codigo_postal_str);

                        bool encontrado = false;
                        for (auto& u : usuarios) {
                            if (u.getId_usuario() == id_usuario) {
                                u.setNombre_usuario(nombre);
                                u.setContrasena_usuario(contrasena);
                                u.setContacto_usuario(contacto);
                                u.setId_subscripcion(id_subscripcion);
                                u.setDireccion(direccion);
                                u.setCodigo_postal(codigo_postal);
                                encontrado = true;
                                break;
                            }
                        }

                        if (encontrado) {
                            guardarUsuariosCsv(usuarios);
                            respuestaServidor = "OK;" + id_usuario_str + ";" + nombre + ";" + contrasena + ";" + contacto + ";" + id_subscripcion_str + ";" + direccion + ";" + codigo_postal_str;
                        } else {
                            respuestaServidor = "ERROR;Usuario no encontrado";
                        }
                    } catch (const exception& e) {
                        respuestaServidor = string("ERROR;Formato incorrecto: ") + e.what();
                    }
                }
            } else if (comando == "ELIMINAR_USUARIO") {
                cargarDatos();
                string id_usuario_str;
                getline(ss, id_usuario_str, ';');

                if (id_usuario_str.empty()) {
                    respuestaServidor = "ERROR;ID de usuario vacio";
                } else {
                    try {
                        int id_usuario = stoi(id_usuario_str);
                        auto it = remove_if(usuarios.begin(), usuarios.end(),
                            [id_usuario](const Usuario& u) { return u.getId_usuario() == id_usuario; });

                        if (it != usuarios.end()) {
                            usuarios.erase(it, usuarios.end());
                            guardarUsuariosCsv(usuarios);
                            respuestaServidor = "OK";
                        } else {
                            respuestaServidor = "ERROR;Usuario no encontrado";
                        }
                    } catch (const exception& e) {
                        respuestaServidor = string("ERROR;Formato incorrecto: ") + e.what();
                    }
                }
            } else if (comando == "REALIZAR_PEDIDO") {
                cargarDatos();
                try {
                    string id_pedido_str, fecha_str, estado, id_usuario_str, productos_str, direccion, codigo_postal_str;
                    getline(ss, id_pedido_str, ';');
                    getline(ss, fecha_str, ';');
                    getline(ss, estado, ';');
                    getline(ss, id_usuario_str, ';');
                    getline(ss, productos_str, ';');
                    getline(ss, direccion, ';');
                    getline(ss, codigo_postal_str, ';');

                    // Validación de campos
                    bool campos_ok = !id_pedido_str.empty() && !fecha_str.empty() && !estado.empty() &&
                                     !id_usuario_str.empty() && !productos_str.empty() &&
                                     !direccion.empty() && !codigo_postal_str.empty();

                    if (!campos_ok) {
                        respuestaServidor = "ERROR;Campos incompletos";
                    } else {
                        int id_pedido = stoi(id_pedido_str);
                        int id_usuario = stoi(id_usuario_str);
                        int codigo_postal = stoi(codigo_postal_str);

                        // Parsear productos
                        map<int, int> productos;
                        if (!productos_str.empty()) {
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
                        }

                        // Parsea la fecha (YYYY-MM-DD)
                        int anyo, mes, dia;
                        char sep1, sep2;
                        istringstream is(fecha_str);
                        is >> anyo >> sep1 >> mes >> sep2 >> dia;
                        if (is.fail()) throw runtime_error("Fecha mal formada");

                        Fecha fecha_pedido{dia, mes, anyo};

                        Pedido nuevoPedido(id_pedido, fecha_pedido, estado, id_usuario, productos, direccion, codigo_postal);
                        pedidos.push_back(nuevoPedido);
                        guardarPedidosCsv(pedidos);
                        respuestaServidor = "OK;Pedido registrado";
                    }
                } catch (const exception& e) {
                    cerr << "Excepcion en REALIZAR_PEDIDO: " << e.what() << endl;
                    respuestaServidor = string("ERROR;Formato de pedido incorrecto: ") + e.what();
                }
            } else {
                respuestaServidor = "ERROR;Comando no reconocido";
            }

            send(comm_socket, respuestaServidor.c_str(), respuestaServidor.size(), 0);
            cout << "Respuesta enviada: " << respuestaServidor << endl;
            closesocket(comm_socket);
        }

        if (comm_socket == INVALID_SOCKET) {
            cout << "Accept fallo: " << to_string(WSAGetLastError()) << endl;
            closesocket(conn_socket);
            WSACleanup();
            return 1;
        }
    }

    closesocket(conn_socket);
    WSACleanup();
    return 0;
}