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
    SOCKET comm_socket;
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
    while ( (comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize)) != INVALID_SOCKET) {
        
        cout << "Conexion aceptada." << endl;
        cout << "Conexion entrante desde: " << inet_ntoa(client.sin_addr) << " (" << ntohs(client.sin_port) << ")" << endl;
        cout << "Esperando mensajes entrantes del cliente... " << endl;

        int bytes = recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
        if (bytes == SOCKET_ERROR) {
            cout << "Error al recibir datos: " <<  to_string(WSAGetLastError());
        } else {

            recvBuff[bytes] = '\0';
            cout << "Recibiendo mensaje... " << endl;
            cout << "Datos recibidos:  " << recvBuff << endl;

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
                if (usuarioEncontrado != usuariosContrasenas.end()) {
                    if (usuariosContrasenas[nombre] == contrasena)
                    {
                        for (Usuario u : usuarios) {
                            if (u.getNombre_usuario() == nombre)
                            {
                                id = to_string(u.getId_usuario());
                                contacto = u.getContacto_usuario();
                                id_sub = to_string(u.getId_subscripcion());
                                direccion = u.getDireccion();
                                cp = to_string(u.getCodigo_postal());
                            }    
                        }
                        
                        respuestaServidor = "OK;" + id + ";" + nombre + ";" + contrasena + ";" + contacto + ";" + id_sub + ";" + direccion + ";" + cp;
                    } else {
                        cout << "Contrasena incorrecta" << endl;
                    }
                    
                } else {
                    cout << "Usuario no registrado" << endl;
                    respuestaServidor = "ERROR;Usuario no encontrado o contrasena incorrecta";
                }
                

            } else if (comando == "REGISTRAR_USUARIO") {
                cargarDatos();
                // Formato esperado: REGISTRAR_USUARIO;id;nombre;contrasena;contacto;id_subscripcion;direccion;codigo_postal
                string id, nombre, contrasena, contacto, id_subscripcion, direccion, codigo_postal;
                getline(ss, id, ';');
                getline(ss, nombre, ';');
                getline(ss, contrasena, ';');
                getline(ss, contacto, ';');
                getline(ss, id_subscripcion, ';');
                getline(ss, direccion, ';');
                getline(ss, codigo_postal, ';');

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
            
            if (bytes == SOCKET_ERROR) {
                cout << "Error al enviar datos" << to_string(WSAGetLastError()) << endl;
            } else {
                send(comm_socket, respuestaServidor.c_str(), respuestaServidor.size(), 0);
                cout << "Datos enviados: " << respuestaServidor << endl;
            }
            closesocket(comm_socket);
        }

        if (comm_socket == INVALID_SOCKET) {
            cout << "accept fallo: " + to_string(WSAGetLastError());
            closesocket(conn_socket);
            WSACleanup();
            return 1;
        }
          
    }

    // Cerrar el socket de escucha y limpiar Winsock al terminar el programa
    closesocket(conn_socket);
    WSACleanup();

    return 0;
}
