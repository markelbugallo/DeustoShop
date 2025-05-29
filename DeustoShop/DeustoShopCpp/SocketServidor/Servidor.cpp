#include <stdio.h>
#include <iostream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../../DeustoShopCpp/Clases/Usuario.h"
#include "../../DeustoShopCpp/BD/Bd.h"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000
using namespace std;

int main() {

    WSADATA wsaData;
    SOCKET conn_socket;     // socket que escucha
    struct sockaddr_in server;
    struct sockaddr_in client;
    char sendBuff[512], recvBuff[512];

    cout << "Inicializando Winsock..." << endl;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        cout << "Fallo en el WSAStartup: " << to_string(WSAGetLastError()) << endl;
        return 1;
    }
    cout << "Inicializado." << endl;

    // Creacion de socket
    if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cout << "No se pudo crear el socket: " << to_string(WSAGetLastError()) << endl;
        WSACleanup();
        return 1;
    }
    cout << "Socket creado." << endl;
    
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

    //BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR) {
		cout << "Bind ha fallado con el codigo de error: " <<  to_string(WSAGetLastError()) << endl;
		closesocket(conn_socket);
		WSACleanup();
		return 1;
	}
    cout << "Bind done." << endl;

    //LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 5) == SOCKET_ERROR) {
		cout << "Listen failed with error code: " << to_string(WSAGetLastError()) << endl;
		closesocket(conn_socket);
		WSACleanup();
		return 1;
	}

    cout << "Esperando conexiones..." << endl;
    int stsize = sizeof(struct sockaddr);
    while (true) {
        SOCKET comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
        if (comm_socket == INVALID_SOCKET) {
            cout << "Accept failed with error code : " << to_string(WSAGetLastError()) << endl;
            continue;
        }
        cout << "Conexion aceptada." << endl;
        cout << "Conexion entrante desde: " << inet_ntoa(client.sin_addr) << " (" << ntohs(client.sin_port) << ")" << endl;

        // Cada conexión acepta varios mensajes hasta que el cliente cierre la conexión.
        // Si el cliente quiere "cerrar sesión" o "registrar otro usuario", debe abrir una nueva conexión.
        cout << "Esperando mensajes entrantes del cliente... " << endl;
        while (true) {
            int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff) - 1, 0);
            if (bytes <= 0) break; // Cliente cerró la conexión
            recvBuff[bytes] = '\0';
            cout << "Recibiendo mensaje... " << endl;
            cout << "Datos recibidos:  " << recvBuff << endl;

            string respuestaServidor = "";
            stringstream ss(recvBuff);
            string comando;
            getline(ss, comando, ';');
            bool cerrarConexionTrasRespuesta = false;
            if (comando == "LOGIN") {
                string nombre, contrasena;
                getline(ss, nombre, ';');
                getline(ss, contrasena, ';');
                // Buscar usuario en el CSV
                FILE* f = fopen("../../DeustoShopC/data/usuarios.csv", "r");
                if (!f) {
                    respuestaServidor = "ERROR;No se pudo abrir el archivo de usuarios";
                } else {
                    char linea[512];
                    bool encontrado = false;
                    fgets(linea, sizeof(linea), f); // Saltar cabecera
                    while (fgets(linea, sizeof(linea), f)) {
                        stringstream lss(linea);
                        string id, user, pass, contacto, id_sub, direccion, cp;
                        getline(lss, id, ';');
                        getline(lss, user, ';');
                        getline(lss, pass, ';');
                        getline(lss, contacto, ';');
                        getline(lss, id_sub, ';');
                        getline(lss, direccion, ';');
                        getline(lss, cp, ';');
                        // Limpiar salto de línea de cp
                        if (!cp.empty() && (cp.back() == '\n' || cp.back() == '\r')) cp.pop_back();
                        if (!cp.empty() && (cp.back() == '\n' || cp.back() == '\r')) cp.pop_back();
                        if (user == nombre && pass == contrasena) {
                            respuestaServidor = "OK;" + id + ";" + user + ";" + pass + ";" + contacto + ";" + id_sub + ";" + direccion + ";" + cp;
                            encontrado = true;
                            break;
                        }
                    }
                    fclose(f);
                    if (!encontrado) {
                        respuestaServidor = "ERROR;Usuario no encontrado o contraseña incorrecta";
                    }
                }
                cerrarConexionTrasRespuesta = true;
            } else if (comando == "REGISTRAR_USUARIO") {
                // Formato esperado: REGISTRAR_USUARIO;id;nombre;contrasena;contacto;id_subscripcion;direccion;codigo_postal
                string id, nombre, contrasena, contacto, id_subscripcion, direccion, codigo_postal;
                getline(ss, id, ';');
                getline(ss, nombre, ';');
                getline(ss, contrasena, ';');
                getline(ss, contacto, ';');
                getline(ss, id_subscripcion, ';');
                getline(ss, direccion, ';');
                getline(ss, codigo_postal, ';');

                // Cargar usuarios existentes
                vector<Usuario> usuarios = cargarUsuariosCSV("../../DeustoShopC/data/usuarios.csv");
                // Comprobar si el usuario ya existe
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
                    // Crear nuevo usuario y añadirlo a la lista
                    Usuario nuevoUsuario(
                        stoi(id), nombre, contrasena, contacto,
                        stoi(id_subscripcion), direccion, stoi(codigo_postal)
                    );
                    usuarios.push_back(nuevoUsuario);
                    // Guardar todos los usuarios en el CSV
                    guardarUsuariosCsv(usuarios);
                    // Devolver los datos del usuario registrado
                    respuestaServidor = "OK;" + id + ";" + nombre + ";" + contrasena + ";" + contacto + ";" + id_subscripcion + ";" + direccion + ";" + codigo_postal;
                }
                cerrarConexionTrasRespuesta = true;
            } else if (comando == "EDITAR_USUARIO") {
                // Formato esperado: EDITAR_USUARIO;id;nombre;contrasena;contacto;id_subscripcion;direccion;codigo_postal
                string id, nombre, contrasena, contacto, id_subscripcion, direccion, codigo_postal;
                getline(ss, id, ';');
                getline(ss, nombre, ';');
                getline(ss, contrasena, ';');
                getline(ss, contacto, ';');
                getline(ss, id_subscripcion, ';');
                getline(ss, direccion, ';');
                getline(ss, codigo_postal, ';');

                vector<Usuario> usuarios = cargarUsuariosCSV("../../DeustoShopC/data/usuarios.csv");
                bool actualizado = false;
                for (auto& u : usuarios) {
                    if (to_string(u.getId_usuario()) == id) {
                        u.setNombre_usuario(nombre);
                        u.setContrasena_usuario(contrasena);
                        u.setContacto_usuario(contacto);
                        u.setId_subscripcion(stoi(id_subscripcion));
                        u.setDireccion(direccion);
                        u.setCodigo_postal(stoi(codigo_postal));
                        actualizado = true;
                        break;
                    }
                }
                if (!actualizado) {
                    respuestaServidor = "ERROR;Usuario no encontrado";
                } else {
                    guardarUsuariosCsv(usuarios);
                    respuestaServidor = "OK;" + id + ";" + nombre + ";" + contrasena + ";" + contacto + ";" + id_subscripcion + ";" + direccion + ";" + codigo_postal;
                }
                cerrarConexionTrasRespuesta = true;
            } else if (comando == "ELIMINAR_USUARIO") {
                // Formato esperado: ELIMINAR_USUARIO;id
                string id;
                getline(ss, id, ';');
                vector<Usuario> usuarios = cargarUsuariosCSV("../../DeustoShopC/data/usuarios.csv");
                size_t oldSize = usuarios.size();
                usuarios.erase(
                    remove_if(usuarios.begin(), usuarios.end(), [&](const Usuario& u) {
                        return to_string(u.getId_usuario()) == id;
                    }),
                    usuarios.end()
                );
                if (usuarios.size() == oldSize) {
                    respuestaServidor = "ERROR;Usuario no encontrado";
                } else {
                    guardarUsuariosCsv(usuarios);
                    respuestaServidor = "OK";
                }
            } else {
                // Respuesta por defecto para otros comandos
                respuestaServidor = "ERROR;Comando no reconocido";
            }
            send(comm_socket, respuestaServidor.c_str(), respuestaServidor.size(), 0);
            cout << "Datos enviados: " << respuestaServidor << endl;
            if (cerrarConexionTrasRespuesta) break;
        }
        // Cerrar la conexión con el cliente actual
        closesocket(comm_socket);
        cout << "Cliente desconectado. Esperando nueva conexion..." << endl;
    }

    // Cerrar el socket de escucha y limpiar Winsock al terminar el programa
    closesocket(conn_socket);
    WSACleanup();

    return 0;
}
